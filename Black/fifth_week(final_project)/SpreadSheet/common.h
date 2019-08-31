#pragma once

#include <iosfwd>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <unordered_map>


// Исключение, выбрасываемое при попытке передать в метод некорректную позицию
class InvalidPositionException : public std::out_of_range {
public:
    using std::out_of_range::out_of_range;
};

// Позиция ячейки. Индексация с нуля.
struct Position {
public:
    bool operator==(const Position &rhs) const;

    bool operator<(const Position &rhs) const;

    [[nodiscard]] bool IsValid() const;

    [[nodiscard]] std::string ToString() const;

    static Position FromString(std::string_view str);

    static const int kMaxRows = 16384;
    static const int kMaxCols = 16384;

    int row = 0;
    int col = 0;
};

struct Size {
    int rows = 0;
    int cols = 0;

    bool operator==(const Size &rhs) const;
};

// Описывает ошибки, которые могут возникнуть при вычислении формулы.
class FormulaError {
public:
    enum class Category {
        Ref,  // ссылка на несуществующую ячейку (например, она была удалена)
        Value,  // ячейка не может быть трактована как число
        Div0,  // в результате вычисления возникло деление на ноль
    };

    explicit FormulaError(Category category);

    [[nodiscard]] Category GetCategory() const;

    bool operator==(FormulaError rhs) const;

    [[nodiscard]] std::string_view ToString() const;

private:
    Category category_;
};

std::ostream &operator<<(std::ostream &output, FormulaError fe);

// Исключение, выбрасываемое при попытке задать синтаксически некорректную
// формулу
class FormulaException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Исключение, выбрасываемое при попытке задать формулу, которая приводит к
// циклической зависимости между ячейками
class CircularDependencyException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Исключение, выбрасываемое если вставка строк/столбцов в таблицу приведёт к
// ячейке с позицией больше максимально допустимой
class TableTooBigException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ICell {
public:
    // Либо текст ячейки, либо значение формулы, либо сообщение об ошибке из
    // формулы
    using Value = std::variant<std::string, double, FormulaError>;

    virtual ~ICell() = default;

    // Возвращает видимое значение ячейки.
    // В случае текстовой ячейки это её текст (без экранирующих символов). В
    // случае формулы - числовое значение формулы или сообщение об ошибке.
    virtual Value GetValue() const = 0;

    // Возвращает внутренний текст ячейки, как если бы мы начали её
    // редактирование. В случае текстовой ячейки это её текст (возможно,
    // содержащий экранирующие символы). В случае формулы - её выражение.
    virtual std::string GetText() const = 0;

    // Возвращает список ячеек, которые непосредственно задействованы в данной
    // формуле. Список отсортирован по возрастанию и не содержит повторяющихся
    // ячеек. В случае текстовой ячейки список пуст.
    virtual std::vector<Position> GetReferencedCells() const = 0;
};

inline constexpr char kFormulaSign = '=';
inline constexpr char kEscapeSign = '\'';

// Интерфейс таблицы
class ISheet {
public:
    virtual ~ISheet() = default;

    // Задаёт содержимое ячейки. Если текст начинается со знака "=", то он
    // интерпретируется как формула. Если задаётся синтаксически некорректная
    // формула, то бросается исключение FormulaException и значение ячейки не
    // изменяется. Если задаётся формула, которая приводит к циклической
    // зависимости (в частности, если формула использует текущую ячейку), то
    // бросается исключение CircularDependencyException и значение ячейки не
    // изменяется.
    // Уточнения по записи формулы:
    // * Если текст содержит только символ "=" и больше ничего, то он не считается
    // формулой
    // * Если текст начинается с символа "'" (апостроф), то при выводе значения
    // ячейки методом GetValue() он опускается. Можно использовать, если нужно
    // начать текст со знака "=", но чтобы он не интерпретировался как формула.
    virtual void SetCell(Position pos, std::string text) = 0;

    // Возвращает значение ячейки.
    // Если ячейка пуста, может вернуть nullptr.
    [[nodiscard]] virtual const ICell *GetCell(Position pos) const = 0;

    virtual ICell *GetCell(Position pos) = 0;

    // Очищает ячейку.
    // Последующий вызов GetCell() для этой ячейки вернёт либо nullptr, либо
    // объект с пустым текстом.
    virtual void ClearCell(Position pos) = 0;

    // Вставляет заданное число пустых строк/столбцов перед строкой/столбцом с
    // заданным индексом. Все ссылки из формул обновляются таким образом, чтобы
    // указывать на те же ячейки, что и до вставки.
    // Если вставка привела бы к тому, что какие-то ячейки получат позиции больше
    // максимальной, или индексы ячеек в каких-то формулах вылезут за максимально
    // допустимые, то бросается исключение TableTooBigException и содержимое
    // таблицы не изменяется.
    virtual void InsertRows(int before, int count = 1) = 0;

    virtual void InsertCols(int before, int count = 1) = 0;

    // Удаляет заданное число пустых строк/столбцов, начиная со строки/столбца с
    // заданным индексом. Все ссылки из формул обновляются таким образом, чтобы
    // указывать на те же ячейки, что и до удаления.
    virtual void DeleteRows(int first, int count = 1) = 0;

    virtual void DeleteCols(int first, int count = 1) = 0;

    // Вычисляет размер области, которая участвует в печати.
    // Определяется как ограничивающий прямоугольник всех ячеек с непустым
    // текстом.
    virtual Size GetPrintableSize() const = 0;

    // Выводит всю таблицу в переданный поток. Столбцы разделяются знаком
    // табуляции. После каждой строки выводится символ перевода строки. Для
    // преобразования ячеек в строку используются методы GetValue() или GetText()
    // соответственно. Пустая ячейка представляется пустой строкой в любом случае.
    virtual void PrintValues(std::ostream &output) const = 0;

    virtual void PrintTexts(std::ostream &output) const = 0;
};

// Создаёт готовую к работе пустую таблицу.
std::unique_ptr<ISheet> CreateSheet();
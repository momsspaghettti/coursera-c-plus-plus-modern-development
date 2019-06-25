#pragma once
#include <string>
#include <vector>
#include <memory>


struct Point 
{
	int x;
	int y;
};


struct Size 
{
	int width;
	int height;
};


// Проверяет, содержится ли заданная точка в эллипсе заданного размера
// Считается, что эллипс вписан в прямоугольник с вершинами в точках (0, 0) и
// (size.width, size.height)
inline bool IsPointInEllipse(Point p, Size size) 
{
	// Нормируем координаты точки в диапазон (-1, 1)
	const double x = (p.x + 0.5) / (size.width / 2.0) - 1;
	const double y = (p.y + 0.5) / (size.height / 2.0) - 1;
	
    // Проверяем, лежит ли точка в единичном круге
	return x * x + y * y <= 1;
}


// Изображение. Пиксели это символы.
// Первый индекс (по std::vector) - строки изображения, координата y
// Второй индекс (по std::string) - столбцы изображения, координата x
// Предполагается, что длина всех строк одинакова
using Image = std::vector<std::string>;


// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType
{
    Rectangle, Ellipse
};


// Интерфейс текстуры
class ITexture 
{
public:
	virtual ~ITexture() = default;

	// Возвращает размер хранимого изображения
	virtual Size GetSize() const = 0;
	
    // Возвращает хранимое изображение
	virtual const Image& GetImage() const = 0;
};


// Интерфейс фигуры
class IShape 
{
public:
	virtual ~IShape() = default;

	// Возвращает точную копию фигуры.
	// Если фигура содержит текстуру, то созданная копия содержит ту же самую
	// текстуру. Фигура и её копия совместно владеют этой текстурой.
	virtual std::unique_ptr<IShape> Clone() const = 0;

	virtual void SetPosition(Point) = 0;
	
    virtual Point GetPosition() const = 0;

	virtual void SetSize(Size) = 0;
	
    virtual Size GetSize() const = 0;

	virtual void SetTexture(std::shared_ptr<ITexture>) = 0;
	
    virtual ITexture* GetTexture() const = 0;

	// Рисует фигуру на указанном изображении
	virtual void Draw(Image&) const = 0;
};


// Создаёт фигуру заданного типа. Вам нужно реализовать эту функцию.
std::unique_ptr<IShape> MakeShape(ShapeType shape_type);
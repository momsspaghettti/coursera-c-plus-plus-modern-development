#pragma once
#include <memory>
#include <string>


// Интерфейс, представляющий книгу
class IBook 
{
public:
	virtual ~IBook() = default;

	// Возвращает название книги
	virtual const std::string& GetName() const = 0;

	// Возвращает текст книги как строку.
	// Размером книги считается размер её текста в байтах.
	virtual const std::string& GetContent() const = 0;
};


// Интерфейс, позволяющий распаковывать книги
class IBooksUnpacker 
{
public:
	virtual ~IBooksUnpacker() = default;

	// Распаковывает книгу с указанным названием из хранилища
	virtual std::unique_ptr<IBook> UnpackBook(const std::string& book_name) = 0;
};


// Интерфейс, представляющий кэш
class ICache 
{
public:
	// Настройки кэша
	struct Settings 
    {
		// Максимальный допустимый объём памяти, потребляемый закэшированными
		// объектами, в байтах
		size_t max_memory = 0;
	};

	using BookPtr = std::shared_ptr<const IBook>;

	virtual ~ICache() = default;

	// Возвращает книгу с заданным названием. Если её в данный момент нет
	// в кэше, то предварительно считывает её и добавляет в кэш. Следит за тем,
	// чтобы общий объём считанных книг не превосходил указанного в параметре
	// max_memory. При необходимости удаляет из кэша книги, к которым дольше всего
	// не обращались. Если размер самой книги уже больше max_memory, то оставляет
	// кэш пустым.
	virtual BookPtr GetBook(const std::string& book_name) = 0;
};


// Создаёт объект кэша для заданного распаковщика и заданных настроек
std::unique_ptr<ICache> MakeCache(std::shared_ptr<IBooksUnpacker> books_unpacker, 
	const ICache::Settings& settings);


void TestCache();
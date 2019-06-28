#include "Common.h"
#include "UnitTestsFramework.h"
#include <atomic>
#include <future>
#include <numeric>
#include <random>
#include <sstream>


// Данная реализация интерфейса IBook позволяет отследить объём памяти, в данный
// момент занимаемый всеми загруженными книгами. Для тестирования своей
// программы вы можете написать другую реализацию, которая позволит также
// убедиться, что из кэша выгружаются в первую очередь наименее используемые
// элементы. Собственно, тестирующая система курсеры имеет как раз более
// продвинутую реализацию.
class Book : public IBook
{
public:
	Book(std::string name, std::string content, 
		std::atomic<size_t>& memory_used_by_books)
		: name_(move(name))
		, content_(move(content))
		, memory_used_by_books_(memory_used_by_books)
	{
		memory_used_by_books_ += content_.size();
	}

	~Book() 
    {
		memory_used_by_books_ -= content_.size();
	}

	const std::string& GetName() const override
	{
		return name_;
	}

	const std::string& GetContent() const override 
    {
		return content_;
	}

private:
	std::string name_;
	std::string content_;
	std::atomic<size_t>& memory_used_by_books_;
};


// Данная реализация интерфейса IBooksUnpacker позволяет отследить объём памяти,
// в данный момент занимаемый всеми загруженными книгами и запросить количество
// обращений к методу UnpackBook(). Для тестирования своей программы вы можете
// написать другую реализацию. Собственно, тестирующая система курсеры имеет как
// раз более продвинутую реализацию.
class BooksUnpacker : public IBooksUnpacker 
{
public:
	std::unique_ptr<IBook> UnpackBook(const std::string& book_name) override {
		++unpacked_books_count_;
		return std::make_unique<Book>(
			book_name,
			"Dummy content of the book " + book_name,
			memory_used_by_books_
			);
	}

	size_t GetMemoryUsedByBooks() const
    {
		return memory_used_by_books_;
	}


	int GetUnpackedBooksCount() const 
    {
		return unpacked_books_count_;
	}

private:
	// Шаблонный класс atomic позволяет безопасно использовать скалярный тип из
	// нескольких потоков. В противном случае у нас было бы состояние гонки.
	std::atomic<size_t> memory_used_by_books_ = 0;
	std::atomic<int> unpacked_books_count_ = 0;
};


struct Library 
{
	std::vector<std::string> book_names;
	std::unordered_map<std::string, std::unique_ptr<IBook>> content;
	size_t size_in_bytes = 0;

	explicit Library(std::vector<std::string> a_book_names, IBooksUnpacker& unpacker)
		: book_names(std::move(a_book_names))
	{
		for (const auto& book_name : book_names) 
		{
			auto& book_content = content[book_name];
			book_content = unpacker.UnpackBook(book_name);
			size_in_bytes += book_content->GetContent().size();
		}
	}
};


void TestUnpacker(const Library& lib) 
{
	BooksUnpacker unpacker;
	for (const auto& book_name : lib.book_names)
	{
		auto book = unpacker.UnpackBook(book_name);
		ASSERT_EQUAL(book->GetName(), book_name);
	}
}


void TestMaxMemory(const Library& lib)
{
	auto unpacker = std::make_shared<BooksUnpacker>();
	ICache::Settings settings;
	settings.max_memory = lib.size_in_bytes / 2;
	auto cache = MakeCache(unpacker, settings);

	for (const auto& [name, book] : lib.content) 
	{
		cache->GetBook(name);
		ASSERT(unpacker->GetMemoryUsedByBooks() <= settings.max_memory);
	}
}


void TestCaching(const Library& lib) 
{
	auto unpacker = std::make_shared<BooksUnpacker>();
	ICache::Settings settings;
	settings.max_memory = lib.size_in_bytes;
	auto cache = MakeCache(unpacker, settings);

	// Если запрашивать одну и ту же книгу подряд, то она определённо должна
	// возвращаться из кэша. Заметьте, что этого простого теста вовсе
	// недостаточно, чтобы полностью проверить правильность реализации стратегии
	// замещения элементов в кэше. Для этих целей можете написать тест
	// самостоятельно.
	cache->GetBook(lib.book_names[0]);
	cache->GetBook(lib.book_names[0]);
	cache->GetBook(lib.book_names[0]);
	ASSERT_EQUAL(unpacker->GetUnpackedBooksCount(), 1);
}


void TestSmallCache(const Library& lib) 
{
	auto unpacker = std::make_shared<BooksUnpacker>();
	ICache::Settings settings;
	settings.max_memory =
		unpacker->UnpackBook(lib.book_names[0])->GetContent().size() - 1;
	auto cache = MakeCache(unpacker, settings);

	cache->GetBook(lib.book_names[0]);
	ASSERT_EQUAL(unpacker->GetMemoryUsedByBooks(), size_t(0));
}


void TestAsync(const Library& lib) 
{
	static const int tasks_count = 10;
	static const int trials_count = 10000;

	auto unpacker = std::make_shared<BooksUnpacker>();
	ICache::Settings settings;
	settings.max_memory = lib.size_in_bytes - 1;
	auto cache = MakeCache(unpacker, settings);

	std::vector<std::future<void>> tasks;

	for (int task_num = 0; task_num < tasks_count; ++task_num) 
	{
		tasks.push_back(std::async([&cache, &lib, task_num] 
		{
			std::default_random_engine gen;
			std::uniform_int_distribution<size_t> dis(0, lib.book_names.size() - 1);
			for (int i = 0; i < trials_count; ++i) 
			{
				const auto& book_name = lib.book_names[dis(gen)];
				ASSERT_EQUAL(
					cache->GetBook(book_name)->GetContent(),
					lib.content.find(book_name)->second->GetContent()
				);
			}

			std::stringstream ss;
			ss << "Task #" << task_num << " completed\n";
			std::cout << ss.str();
		}));
	}

	// вызов метода get пробрасывает исключения в основной поток
	for (auto& task : tasks) 
	{
		task.get();
	}
}


void TestCache() 
{
	BooksUnpacker unpacker;
	const Library lib(
		// Названия книг для локального тестирования. В тестирующей системе курсеры
		// будет другой набор, намного больше.
		{
		  "Sherlock Holmes",
		  "Don Quixote",
		  "Harry Potter",
		  "A Tale of Two Cities",
		  "The Lord of the Rings",
		  "Le Petit Prince",
		  "Alice in Wonderland",
		  "Dream of the Red Chamber",
		  "And Then There Were None",
		  "The Hobbit"
		},
		unpacker
	);


#define RUN_CACHE_TEST(tr, f) tr.RunTest([&lib] { f(lib); }, #f)

	TestRunner tr;
	RUN_CACHE_TEST(tr, TestUnpacker);
	RUN_CACHE_TEST(tr, TestMaxMemory);
	RUN_CACHE_TEST(tr, TestCaching);
	RUN_CACHE_TEST(tr, TestSmallCache);
	RUN_CACHE_TEST(tr, TestAsync);

#undef RUN_CACHE_TEST
}
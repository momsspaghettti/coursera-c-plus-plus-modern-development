#include "animals.h"
#include "UnitTestsFramework.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <memory>


using Zoo = std::vector<std::shared_ptr<Animal>>;


// Эта функция получает на вход поток ввода и читает из него описание зверей.
// Если очередное слово этого текста - Tiger, Wolf или Fox, функция должна поместить соответствующего зверя в зоопарк.
// В противном случае она должна прекратить чтение и сгенерировать исключение runtime_error.
Zoo CreateZoo(std::istream& in) 
{
	Zoo zoo;

	std::string name;
	while (in >> name)
	{
		if (name == "Tiger")
		{
			zoo.emplace_back(std::make_shared<Tiger>());
		}
		else if (name == "Wolf")
		{
			zoo.emplace_back(std::make_shared<Wolf>());
		}
		else if (name == "Fox")
		{
			zoo.emplace_back(std::make_shared<Fox>());
		}
		else
		{
			throw std::runtime_error("!");
		}
	}

	return zoo;
}


// Эта функция должна перебрать всех зверей в зоопарке в порядке их создания
// и записать в выходной поток для каждого из них результат работы виртуальной функции voice.
// Разделяйте голоса разных зверей символом перевода строки '\n'.
void Process(const Zoo& zoo, std::ostream& out) 
{
    for (const auto& animal : zoo)
    {
		out << animal->Voice() << '\n';
    }
}


void ZooTest() 
{
	std::istringstream input("Tiger Wolf Fox Tiger");
    std::ostringstream output;
	Process(CreateZoo(input), output);

	const std::string expected =
		"Rrrr\n"
		"Wooo\n"
		"Tyaf\n"
		"Rrrr\n";

	ASSERT_EQUAL(output.str(), expected);
}


void TestZoo() 
{
	TestRunner tr;
	RUN_TEST(tr, ZooTest);
}
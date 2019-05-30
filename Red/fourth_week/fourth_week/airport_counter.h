#pragma once
#include <array>
#include <cstdint>
#include <utility>


// TAirport should be enum with sequential items and last item TAirport::Last_
template <typename TAirport>
class AirportCounter
{
public:
	const static uint32_t N = static_cast<uint32_t>(TAirport::Last_);

	// конструктор по умолчанию: список элементов пока пуст
	AirportCounter() = default;

	// конструктор от диапазона элементов типа TAirport
	template <typename TIterator>
	AirportCounter(TIterator begin, TIterator end);

	// получить количество элементов, равных данному
	size_t Get(TAirport airport) const;

	// добавить данный элемент
	void Insert(TAirport airport);

	// удалить одно вхождение данного элемента
	void EraseOne(TAirport airport);

	// удалить все вхождения данного элемента
	void EraseAll(TAirport airport);

	using Item = std::pair<TAirport, size_t>;
	using Items = std::array<Item, N>;

	// получить некоторый объект, по которому можно проитерироваться,
	// получив набор объектов типа Item - пар (аэропорт, количество),
	// упорядоченных по аэропорту
	Items GetItems() const;

private:
	std::array<size_t, N> airports_to_count_ = {0};

	//void DefaultInitializer();
};


template <typename TAirport>
template <typename TIterator>
AirportCounter<TAirport>::AirportCounter(TIterator begin, TIterator end)
{
    for (auto iter = begin; iter != end; ++iter)
    {
		++airports_to_count_[static_cast<uint32_t>(*iter)];
    }
}


template <typename TAirport>
size_t AirportCounter<TAirport>::Get(TAirport airport) const
{
	return airports_to_count_.at(static_cast<uint32_t>(airport));
}


template <typename TAirport>
void AirportCounter<TAirport>::Insert(TAirport airport)
{
	++airports_to_count_[static_cast<uint32_t>(airport)];
}


template <typename TAirport>
void AirportCounter<TAirport>::EraseOne(TAirport airport)
{
    if (airports_to_count_[static_cast<uint32_t>(airport)] != 0)
    {
		--airports_to_count_[static_cast<uint32_t>(airport)];
    }
}


template <typename TAirport>
void AirportCounter<TAirport>::EraseAll(TAirport airport)
{
	airports_to_count_[static_cast<uint32_t>(airport)] = 0;
}


template <typename TAirport>
typename AirportCounter<TAirport>::Items AirportCounter<TAirport>::GetItems() const
{
	std::array<Item, N> result;
    for (uint32_t i = 0; i < N; ++i)
    {
		result[i] = std::make_pair(static_cast<TAirport>(i), airports_to_count_.at(i));
    }

	return result;
}


void TestAirportCounter();
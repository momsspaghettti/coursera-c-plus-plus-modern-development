#pragma once
#include <algorithm>
#include <iterator>
#include <set>
#include <utility>
#include <vector>


template <typename T>
class PriorityCollection {
public:
	using Id = size_t;

	// Добавить объект с нулевым приоритетом
	// с помощью перемещения и вернуть его идентификатор
	Id Add(T object);

	// Добавить все элементы диапазона [range_begin, range_end)
	// с помощью перемещения, записав выданные им идентификаторы
	// в диапазон [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
		IdOutputIt ids_begin);

	// Определить, принадлежит ли идентификатор какому-либо
	// хранящемуся в контейнере объекту
	bool IsValid(Id id) const;

	// Получить объект по идентификатору
	const T& Get(Id id) const;

	// Увеличить приоритет объекта на 1
	void Promote(Id id);

	// Получить объект с максимальным приоритетом и его приоритет
	std::pair<const T&, int> GetMax() const;

	// Аналогично GetMax, но удаляет элемент из контейнера
	std::pair<T, int> PopMax();

private:
	std::set<std::pair<int, size_t>> priority_set_;
	std::vector<T> data_;
	std::vector<int> checker_;
};


template <typename T>
typename PriorityCollection<T>::Id PriorityCollection<T>::Add(T object)
{
	Id id = data_.size();
	data_.push_back(std::move(object));

	checker_.push_back(0);

	priority_set_.insert({ 0, id });

	return id;
}


template <typename T>
template <typename ObjInputIt, typename IdOutputIt>
void PriorityCollection<T>::Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin)
{
    for (; range_begin != range_end;)
    {
		*(ids_begin++) = Add(std::move(*(range_begin++)));
    }
}


template <typename T>
bool PriorityCollection<T>::IsValid(Id id) const
{
	return id < data_.size() && checker_.at(id) != -1;
}


template <typename T>
const T& PriorityCollection<T>::Get(Id id) const
{
	return data_.at(id);
}


template <typename T>
void PriorityCollection<T>::Promote(Id id)
{
	const auto iter = priority_set_.find({checker_.at(id), id});
	priority_set_.erase(iter);

	++checker_[id];
	priority_set_.insert({ checker_.at(id), id });
}


template <typename T>
std::pair<const T&, int> PriorityCollection<T>::GetMax() const
{
	auto max_pair = *std::prev(priority_set_.end());

	return { data_.at(max_pair.second), max_pair.first };
}


template <typename T>
std::pair<T, int> PriorityCollection<T>::PopMax()
{
	auto max_pair = *std::prev(priority_set_.end());
	priority_set_.erase(std::prev(priority_set_.end()));

	checker_[max_pair.second] = -1;

	auto result = std::make_pair(std::move(data_.at(max_pair.second)), std::move(max_pair.first));

	return result;
}


class StringNonCopyable : public std::string 
{
public:
	using std::string::string;  // Позволяет использовать конструкторы строки
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};


void TestPriorityCollection();
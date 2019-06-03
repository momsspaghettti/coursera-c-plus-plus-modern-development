#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <set>
#include <stdexcept>


template <typename T>
class PriorityCollection {
public:
	using Id = int;

	// Добавить объект с нулевым приоритетом
	// с помощью перемещения и вернуть его идентификатор
	Id Add(T object);

	// Добавить все элементы диапазона [range_begin, range_end)
	// с помощью перемещения, записав выданные им идентификаторы
	// в диапазон [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin);

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
	struct ObjectItem 
    {
		T data;
		int priority = 0;
	};
	static const int NONE_PRIORITY = -1;

	std::vector<ObjectItem> objects;
	std::set<std::pair<int, Id>> sorted_objects;
};


template <typename T>
typename PriorityCollection<T>::Id PriorityCollection<T>::Add(T object)
{
	const Id new_id = objects.size();
	objects.push_back({ std::move(object) });
	sorted_objects.insert({ 0, new_id });
	return new_id;
}


template <typename T>
template <typename ObjInputIt, typename IdOutputIt>
void PriorityCollection<T>::Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin)
{
	while (range_begin != range_end)
	{
		*ids_begin++ = Add(std::move(*range_begin++));
	}
}


template <typename T>
bool PriorityCollection<T>::IsValid(Id id) const
{
	return id >= 0 && id < static_cast<int>(objects.size()) &&
		objects[id].priority != NONE_PRIORITY;
}


template <typename T>
const T& PriorityCollection<T>::Get(Id id) const
{
	return objects[id].data;
}


template <typename T>
void PriorityCollection<T>::Promote(Id id)
{
	auto& item = objects[id];
	const int old_priority = item.priority;
	const int new_priority = ++item.priority;
	sorted_objects.erase({ old_priority, id });
	sorted_objects.insert({ new_priority, id });
}


template <typename T>
std::pair<const T&, int> PriorityCollection<T>::GetMax() const
{
	const auto& item = objects[std::prev(sorted_objects.end())->second];
	return { item.data, item.priority };
}


template <typename T>
std::pair<T, int> PriorityCollection<T>::PopMax()
{
	const auto sorted_objects_it = std::prev(sorted_objects.end());
	auto& item = objects[sorted_objects_it->second];
	sorted_objects.erase(sorted_objects_it);
	const int priority = item.priority;
	item.priority = NONE_PRIORITY;
	return { std::move(item.data), priority };
}


void TestPriorityCollection();
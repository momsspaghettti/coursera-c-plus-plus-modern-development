#pragma once
#include <mutex>


// Реализуйте шаблон Synchronized<T>.
// Метод GetAccess должен возвращать структуру, в которой есть поле T& value.
template <typename T>
class Synchronized {
public:
	explicit Synchronized(T initial = T()) : value_(std::move(initial)){}

	struct Access 
    {
        Access(T& value, std::mutex& mutex) : ref_to_value(value), guard_(mutex){}

		T& ref_to_value;
	private:
		std::lock_guard<std::mutex> guard_;
	};

	Access GetAccess();
private:
	T value_;
	std::mutex mutex_;
};


template <typename T>
typename Synchronized<T>::Access Synchronized<T>::GetAccess()
{
	return { value_, mutex_ };
}


void TestSynchronized();
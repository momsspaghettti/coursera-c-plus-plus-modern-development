#include <iostream>


template<typename T>
class Widget
{
    Widget(T v)
    {
		value = v;
    }

private:
	T value;
};


int main()
{
	Widget w(5);

    return 0;
}
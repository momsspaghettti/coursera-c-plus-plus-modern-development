#pragma once
// «десь будет подключЄн ваш файл с определением шаблонного класса Booking в пространстве имЄн RAII
#include "booking.h"
#include <stdexcept>
#include <string>


class FlightProvider
{
public:
	using BookingId = int;
	using Booking = RAII::Booking<FlightProvider>;
	friend Booking;  // явно разрешаем функци€м класса Booking вызывать private-функции нашего класса FlightProvider

	struct BookingData 
    {
		std::string city_from;
        std::string city_to;
        std::string date;
	};

	Booking Book(const BookingData& data) 
    {
		if (counter >= capacity)
		{
			throw std::runtime_error("Flight overbooking");
		}
		++counter;
		return { this, counter };
	}

private:
	// —крываем эту функцию в private, чтобы еЄ мог позвать только соответствующий friend-класс Booking
	void CancelOrComplete(const Booking& booking) 
    {
		--counter;
	}

public:
	static int capacity;
	static int counter;
};


class HotelProvider 
{
public:
	using BookingId = int;
	using Booking = RAII::Booking<HotelProvider>;
	friend Booking;

	struct BookingData
    {
		std::string city;
		std::string date_from;
		std::string date_to;
	};

	Booking Book(const BookingData& data) 
    {
		if (counter >= capacity)
		{
			throw std::runtime_error("Hotel overbooking");
		}
		++counter;
		return { this, counter };
	}

private:
	void CancelOrComplete(const Booking& booking) 
    {
		--counter;
	}

public:
	static int capacity;
	static int counter;
};
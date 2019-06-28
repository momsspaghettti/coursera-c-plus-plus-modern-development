#pragma once
#include <stdexcept>
#include <string>


class FlightProvider 
{
public:
	using BookingId = int;

	struct BookingData 
    {
		std::string city_from;
        std::string city_to;
        std::string date;
	};

	BookingId Book(const BookingData& data) 
    {
		if (counter >= capacity) 
		{
			throw std::runtime_error("Flight overbooking");
		}
		++counter;
		return counter;
	}

	void Cancel(const BookingId& id) 
    {
		--counter;
		if (counter < 0) 
		{
			throw std::logic_error("Too many flights have been canceled");
		}
	}

	static int capacity;
	static int counter;
};


class HotelProvider
{
public:
	using BookingId = int;

	struct BookingData 
    {
		std::string city;
		std::string date_from;
		std::string date_to;
	};

	BookingId Book(const BookingData& data) 
    {
		if (counter >= capacity) 
		{
			throw std::runtime_error("Hotel overbooking");
		}
		++counter;
		return counter;
	}

	void Cancel(const BookingId& id) 
    {
		--counter;
		if (counter < 0)
		{
			throw std::logic_error("Too many hotels have been canceled");
		}
	}

	static int capacity;
	static int counter;
};
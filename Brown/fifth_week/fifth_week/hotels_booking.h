#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <iostream>


class HotelsManager
{
public:
	struct Booking
	{
		int64_t time;
		std::string hotel_name;
		uint64_t client_id;
		int room_count;
	};

	void Book(const Booking& booking);

	[[nodiscard]] int Clients(const std::string& hotel_name) const;

	[[nodiscard]] int Rooms(const std::string& hotel_name) const;

private:

    struct HotelInformation
    {
		HotelInformation()
		{
			room_count = 0;
		}

		std::unordered_set<uint64_t> client_ids;
		int room_count;
    };

	std::queue<Booking> orders_;
	std::unordered_map <std::string, HotelInformation> hotels_;

	static const int64_t day_ = 86400;

	void remove_old_bookings(const int64_t& current_time);
};


void PerformHotelsBookingQueries(std::istream& input=std::cin, std::ostream& output=std::cout);


void TestHotelsManager();


void TestHotelsBooking();
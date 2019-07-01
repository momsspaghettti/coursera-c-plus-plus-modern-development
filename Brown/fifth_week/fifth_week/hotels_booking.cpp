#include "hotels_booking.h"
#include "UnitTestsFramework.h"


void HotelsManager::remove_old_bookings(const int64_t& current_time)
{
	if (!orders_.empty())
	{
		auto oldest_booking = orders_.front();

		while (oldest_booking.time <= current_time - day_)
		{
			HotelInformation& current_hotel_information = hotels_[oldest_booking.hotel_name];
			current_hotel_information.client_ids.erase(oldest_booking.client_id);
			current_hotel_information.room_count -= oldest_booking.room_count;

			orders_.pop();
			if (orders_.empty())
				break;

			oldest_booking = orders_.front();
		}
	}
}


void HotelsManager::Book(const Booking& booking)
{
	remove_old_bookings(booking.time);

	orders_.push(booking);
	HotelInformation& current_hotel_information = hotels_[booking.hotel_name];

	current_hotel_information.client_ids.insert(booking.client_id);
	current_hotel_information.room_count += booking.room_count;
}


int HotelsManager::Clients(const std::string& hotel_name) const
{
	const auto finder = hotels_.find(hotel_name);

	return finder == hotels_.end() ? 0 : 
    static_cast<int>(finder->second.client_ids.size());
}


int HotelsManager::Rooms(const std::string& hotel_name) const
{
	const auto finder = hotels_.find(hotel_name);

	return finder == hotels_.end() ? 0 : finder->second.room_count;
}


void PerformHotelsBookingQueries(std::istream& input, std::ostream& output)
{
	HotelsManager manager;

	int n;
	input >> n;

	std::string query, hotel_name;
    for (int i = 0; i < n; ++i)
    {
		input >> query;

        if (query == "BOOK")
        {
			uint64_t client_id;
			int64_t time;
			int room_count;

			input >> time >> hotel_name >> client_id >> room_count;

			manager.Book(HotelsManager::Booking{time, hotel_name, client_id, room_count});
        }

        if (query == "CLIENTS")
        {
			input >> hotel_name;

			output << manager.Clients(hotel_name) << '\n';
        }

        if (query == "ROOMS")
        {
			input >> hotel_name;

			output << manager.Rooms(hotel_name) << '\n';
        }
    }
}


void TestHotelsBooking()
{
	TestRunner tr;

	RUN_TEST(tr, TestHotelsManager);
}


void TestHotelsManager()
{
	HotelsManager manager;

	ASSERT_EQUAL(manager.Clients("Marriott"), 0);
	ASSERT_EQUAL(manager.Rooms("Marriott"), 0);

	manager.Book(HotelsManager::Booking{10, "FourSeasons", 1, 2});
	manager.Book(HotelsManager::Booking{ 10, "Marriott", 1, 1 });
	manager.Book(HotelsManager::Booking{ 86409, "FourSeasons", 2, 1});

	ASSERT_EQUAL(manager.Clients("FourSeasons"), 2);
	ASSERT_EQUAL(manager.Rooms("FourSeasons"), 3);
	ASSERT_EQUAL(manager.Clients("Marriott"), 1);

	manager.Book(HotelsManager::Booking{ 86410, "Marriott", 2, 10});

	ASSERT_EQUAL(manager.Rooms("FourSeasons"), 1);
	ASSERT_EQUAL(manager.Rooms("Marriott"), 10);

	manager.Book(HotelsManager::Booking{ 2 * 86410 + 2, "Moscow", 1, 10});

	ASSERT_EQUAL(manager.Clients("Marriott"), 0);
	ASSERT_EQUAL(manager.Clients("FourSeasons"), 0);
	ASSERT_EQUAL(manager.Clients("Moscow"), 1);
	ASSERT_EQUAL(manager.Rooms("Marriott"), 0);
	ASSERT_EQUAL(manager.Rooms("FourSeasons"), 0);
	ASSERT_EQUAL(manager.Rooms("Moscow"), 10);
}
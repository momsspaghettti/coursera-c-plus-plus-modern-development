#include "query.h"
#include "responses.h"
#include "bus_manager.h"


void Start()
{
	int query_count;
	Query q;

	std::cin >> query_count;

	BusManager bm;
	for (int i = 0; i < query_count; ++i) {
		std::cin >> q;
		switch (q.type) {
		case QueryType::NewBus:
			bm.AddBus(q.bus, q.stops);
			break;
		case QueryType::BusesForStop:
			std::cout << bm.GetBusesForStop(q.stop) << std::endl;
			break;
		case QueryType::StopsForBus:
			std::cout << bm.GetStopsForBus(q.bus) << std::endl;
			break;
		case QueryType::AllBuses:
			std::cout << bm.GetAllBuses() << std::endl;
			break;
		}
	}
}


int main() 
{
	Start();

    return 0;
}

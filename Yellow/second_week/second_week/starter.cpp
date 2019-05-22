#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <sstream>


using namespace std;


enum class QueryType
{
	NewBus,
	BusesForStop,
	StopsForBus,
	AllBuses
};


struct Query
{
	QueryType type;
	string bus;
	string stop;
	vector<string> stops;
};


istream& operator >> (istream& is, Query& q)
{
	string query;

	is >> query;

	if (query == "NEW_BUS")
	{
		q.type = QueryType::NewBus;

		is >> q.bus;

		int stop_count;

		is >> stop_count;

		q.stops = vector<string>(stop_count);

		for (string& stop : q.stops)
		{
			is >> stop;
		}
	}

	if (query == "BUSES_FOR_STOP")
	{
		q.type = QueryType::BusesForStop;

		is >> q.stop;
	}

	if (query == "STOPS_FOR_BUS")
	{
		q.type = QueryType::StopsForBus;

		is >> q.bus;
	}

	if (query == "ALL_BUSES")
	{
		q.type = QueryType::AllBuses;
	}

	return is;
}


struct BusesForStopResponse
{
	bool has_stop;

	vector<string> buses;
};


ostream& operator << (ostream& os, const BusesForStopResponse& r)
{
	if (r.has_stop)
	{
		for (const string& bus : r.buses)
		{
			os << bus << ' ';
		}
	}
	else
	{
		os << "No stop";
	}

	return os;
}


struct StopsForBusResponse
{
	bool has_buses;

	string bus;

	map<string, vector<string>> buses_to_stops;
	map<string, vector<string>> stops_to_buses;
};


ostream& operator << (ostream& os, const StopsForBusResponse& r)
{
	ostringstream os_;

	if (!r.has_buses)
	{
		os_ << "No bus";
	}
	else
	{
		for (const string& stop : r.buses_to_stops.at(r.bus))
		{
			os_ << "Stop " << stop << ": ";
			if (r.stops_to_buses.at(stop).size() == 1)
			{
				os_ << "no interchange";
			}
			else
			{
				for (const string& other_bus : r.stops_to_buses.at(stop))
				{
					if (r.bus != other_bus)
					{
						os_ << other_bus << " ";
					}
				}
			}
			os_ << endl;
		}
	}

	string result = os_.str();

	if (result.back() == '\n')
	{
		result.pop_back();
	}

	os << result;

	return os;
}


struct AllBusesResponse
{
	bool empty;

	map<string, vector<string>> buses_to_stops;
};


ostream& operator << (ostream& os, const AllBusesResponse& r)
{
	ostringstream os_;

	if (r.empty)
	{
		os_ << "No buses";
	}
	else
	{
		for (const auto& bus_item : r.buses_to_stops)
		{
			os_ << "Bus " << bus_item.first << ": ";
			for (const string& stop : bus_item.second)
			{
				os_ << stop << " ";
			}
			os_ << endl;
		}
	}

	string result = os_.str();

	if (result.back() == '\n')
	{
		result.pop_back();
	}

	os << result;

	return os;
}


class BusManager {
public:
	void AddBus(const string& bus, const vector<string>& stops) {
		buses_to_stops[bus] = stops;

		for (const string& stop : stops)
		{
			stops_to_buses[stop].push_back(bus);
		}
	}

	BusesForStopResponse GetBusesForStop(const string& stop) const
	{
		try
		{
			return { true, stops_to_buses.at(stop) };
		}
		catch (out_of_range&)
		{
			return { false, {} };
		}
	}

	StopsForBusResponse GetStopsForBus(const string& bus) const
	{
		try
		{
			auto tmp = buses_to_stops.at(bus);
			return { true, bus, buses_to_stops, stops_to_buses };
		}
		catch (out_of_range&)
		{
			return { false, bus, {}, {} };
		}
	}

	AllBusesResponse GetAllBuses() const
	{
		return { buses_to_stops.empty(), buses_to_stops };
	}

private:
	map<string, vector<string>> buses_to_stops, stops_to_buses;
};



int main() {
	int query_count;
	Query q;

	cin >> query_count;

	BusManager bm;
	for (int i = 0; i < query_count; ++i) {
		cin >> q;
		switch (q.type) {
		case QueryType::NewBus:
			bm.AddBus(q.bus, q.stops);
			break;
		case QueryType::BusesForStop:
			cout << bm.GetBusesForStop(q.stop) << endl;
			break;
		case QueryType::StopsForBus:
			cout << bm.GetStopsForBus(q.bus) << endl;
			break;
		case QueryType::AllBuses:
			cout << bm.GetAllBuses() << endl;
			break;
		}
	}

	return 0;
}
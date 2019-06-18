#include "stats_aggregator.h"


namespace StatsAggregators
{
	using namespace std;


	template <typename T>
	ostream& operator << (ostream& os, const optional<T>& v) {
		if (v) {
			os << *v;
		}
		else {
			os << "undefined";
		}
		return os;
	}


	void Composite::Process(int value)
    {
		for (auto& aggr : aggregators)
		{
			aggr->Process(value);
		}
	}

	void Composite::PrintValue(std::ostream& output) const
    {
		for (const auto& aggr : aggregators)
		{
			aggr->PrintValue(output);
			output << '\n';
		}
	}


	void Composite::Add(std::unique_ptr<StatsAggregator> aggr)
    {
		aggregators.push_back(std::move(aggr));
	}


	void Sum::Process(int value)
    {
		sum += value;
	}


	void Sum::PrintValue(std::ostream& out) const
    {
		out << "Sum is " << sum;
	}


	void Min::Process(int value)
    {
		if (!current_min || value < *current_min)
		{
			current_min = value;
		}
	}


	void Min::PrintValue(std::ostream& out) const 
    {
		out << "Min is " << current_min;
	}


	void Max::Process(int value)
    {
		if (!current_max || value > * current_max) 
		{
			current_max = value;
		}
	}


	void Max::PrintValue(std::ostream& out) const
    {
		out << "Max is " << current_max;
	}


	void Average::Process(int value) 
    {
		sum += value;
		++total;
	}


	void Average::PrintValue(std::ostream& out) const 
    {
		out << "Average is ";
		if (total == 0) 
		{
			out << "undefined";
		}
		else
		{
			out << sum / total;
		}
	}


	void Mode::Process(int value)
    {
		int current_count = ++count[value];
		if (!mode || current_count > count[*mode])
		{
			mode = value;
		}
	}


	void Mode::PrintValue(std::ostream& out) const
    {
		out << "Mode is " << mode;
	}
}
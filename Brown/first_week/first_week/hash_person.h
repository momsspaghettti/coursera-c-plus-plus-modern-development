#pragma once
#include <functional>
#include <cstddef>
#include <string>


struct Address 
{
    std::string city, street;
	int building = 0;

	bool operator==(const Address& other) const 
    {
		return city == other.city 
	    && street == other.street 
	    && building == other.building;
	}
};


struct Person 
{
    std::string name;
	int height = 0;
	double weight = 0;
	Address address;

	bool operator==(const Person& other) const 
    {
		return name == other.name 
	    && height == other.height 
	    && weight == other.weight 
	    && address == other.address;
	}
};


struct AddressHasher 
{
    size_t operator()(const Address& address) const
    {
		const size_t a = s_hash_(address.city);
		const size_t b = s_hash_(address.street);
		const size_t c = i_hash_(address.building);
        const size_t x = 2'946'901;

		return (a * x * x + b * x + c);
    }

private:
	std::hash<std::string> s_hash_;
	std::hash<int> i_hash_;
};


struct PersonHasher 
{
    size_t operator()(const Person& person) const
    {
		const size_t a = s_hash_(person.name);
		const size_t b = i_hash_(person.height);
		const size_t c = d_hash_(person.weight);
		const size_t d = a_hash_(person.address);
		const size_t x = 2'946'901;

		return (a * x * x * x + b * x * x + c * x + d);
    }

private:
	std::hash<std::string> s_hash_;
	std::hash<int> i_hash_;
	std::hash<double> d_hash_;
	AddressHasher a_hash_;
};


void TestHashPerson();
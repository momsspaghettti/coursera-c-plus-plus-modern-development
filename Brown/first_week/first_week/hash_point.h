#pragma once
#include <cstddef>
#include <functional>


using CoordType = int;

struct Point3D
{
	CoordType x;
	CoordType y;
	CoordType z;

	bool operator==(const Point3D& other) const
    {
		return x == other.x 
	    && y == other.y 
	    && z == other.z;
	}
};


struct Hasher 
{
public:
	Hasher() = default;

    std::size_t operator()(const Point3D& point) const
	{
		const std::size_t x = 2'946'901;

		const std::size_t a = hash_(point.x);
		const std::size_t b = hash_(point.y);
		const std::size_t c = hash_(point.z);

		return (a * x * x + b * x + c);
	}

private:
	const std::hash<CoordType> hash_;
};


void TestHashPoint();
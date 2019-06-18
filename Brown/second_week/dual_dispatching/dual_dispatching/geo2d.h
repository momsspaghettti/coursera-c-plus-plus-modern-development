#pragma once
#include <cstdint>


namespace geo2d 
{

	struct Point
    {
		int x, y;
	};

	uint64_t DistanceSquared(Point p1, Point p2);

	struct Vector 
    {
		int x, y;

		Vector(int xx, int yy) : x(xx), y(yy) {}

		Vector(Point from, Point to) : x(to.x - from.x), y(to.y - from.y) {}
	};

	int64_t operator * (Vector lhs, Vector rhs);
	int64_t ScalarProduct(Vector lhs, Vector rhs);

	struct Segment 
    {
		Point p1, p2;
	};

	class Rectangle 
    {
	public:
		Rectangle(Point p1, Point p2);

		int Left() const { return x_left; }
		int Right() const { return x_right; }
		int Top() const { return y_top; }
		int Bottom() const { return y_bottom; }

		Point BottomLeft() const { return { x_left, y_bottom }; }
		Point BottomRight() const { return { x_right, y_bottom }; }
		Point TopRight() const { return { x_right, y_top }; }
		Point TopLeft() const { return { x_left, y_top }; }

	private:
		int x_left, x_right;
		int y_bottom, y_top;
	};

	struct Circle 
    {
		Point center;
		uint32_t radius;
	};

	bool Collide(Point p, Point q);
	bool Collide(Point p, Segment s);
	bool Collide(Point p, Rectangle r);
	bool Collide(Point p, Circle c);
	bool Collide(Rectangle r, Point p);
	bool Collide(Rectangle r, Segment s);
	bool Collide(Rectangle r1, Rectangle r2);
	bool Collide(Rectangle r, Circle c);
	bool Collide(Segment s, Point p);
	bool Collide(Segment s1, Segment s2);
	bool Collide(Segment s, Rectangle r);
	bool Collide(Segment s, Circle c);
	bool Collide(Circle c, Point p);
	bool Collide(Circle c, Rectangle r);
	bool Collide(Circle c, Segment s);
	bool Collide(Circle c1, Circle c2);

}
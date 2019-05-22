#include "Figure.h"
#include <iomanip>
#include <iostream>
#include <vector>


Triangle::Triangle(const int& a, const int& b, const int& c) : name_("TRIANGLE"), a_(a), b_(b), c_(c)
{
	calculate_perimeter();
	calculate_area();
}

std::string Triangle::Name()
{
	return name_;
}

double Triangle::Perimeter()
{
	return perimeter_;
}

double Triangle::Area()
{
	return area_;
}

void Triangle::calculate_perimeter()
{
	perimeter_ = a_ + b_ + c_;
}

void Triangle::calculate_area()
{
	const double p = perimeter_ / 2;

	area_ = sqrt(p * (p - a_) * (p - b_) * (p - c_));
}


Rect::Rect(const int& width, const int& height) : name_("RECT"), width_(width), height_(height)
{
	calculate_perimeter();
	calculate_area();
}

std::string Rect::Name()
{
	return name_;
}

double Rect::Perimeter()
{
	return perimeter_;
}

double Rect::Area()
{
	return area_;
}

void Rect::calculate_perimeter()
{
	perimeter_ = 2 * (width_ + height_);
}

void Rect::calculate_area()
{
	area_ = width_ * height_;
}


Circle::Circle(const int& r) : name_("CIRCLE"), r_(r)
{
	calculate_perimeter();
	calculate_area();
}

std::string Circle::Name()
{
	return name_;
}

double Circle::Perimeter()
{
	return perimeter_;
}

double Circle::Area()
{
	return area_;
}

void Circle::calculate_perimeter()
{
	perimeter_ = 2 * 3.14 * r_;
}

void Circle::calculate_area()
{
	area_ = 3.14 * r_ * r_;
}


std::shared_ptr<Figure> CreateFigure(std::istringstream& is)
{
	std::string figure;

	is >> figure;

    if (figure == "TRIANGLE")
    {
		int a, b, c;

		is >> a >> b >> c;

		return std::make_shared<Triangle>(a, b, c);
    }

	if (figure == "RECT")
	{
		int height, width;

		is >> width >> height;

		return std::make_shared<Rect>(width, height);
	}

	int r;

	is >> r;

	return std::make_shared<Circle>(r);
}


void RunFigures()
{
	std::vector<std::shared_ptr<Figure>> figures;
	for (std::string line; getline(std::cin, line); ) {
		std::istringstream is(line);

		std::string command;
		is >> command;
		if (command == "ADD") {
			figures.push_back(CreateFigure(is));
		}
		else if (command == "PRINT") {
			for (const auto& current_figure : figures) {
				std::cout << std::fixed << std::setprecision(3)
					<< current_figure->Name() << " "
					<< current_figure->Perimeter() << " "
					<< current_figure->Area() << std::endl;
			}
		}
	}
}
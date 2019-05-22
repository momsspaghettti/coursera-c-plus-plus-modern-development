#pragma once
#include <string>
#include <sstream>


class Figure
{
public:
	virtual std::string Name() = 0;

	virtual double Perimeter() = 0;

	virtual double Area() = 0;

private:
	virtual void calculate_perimeter() = 0;
	virtual void calculate_area() = 0;
};


class Triangle : public Figure
{
public:
	Triangle(const int& a, const int& b, const int& c);

    std::string Name() override;

	double Perimeter() override;

	double Area() override;

private:
	const std::string name_;
	const int a_, b_, c_;

	void calculate_perimeter() override;
	void calculate_area() override;

protected:
	double perimeter_;
	double area_;
};


class Rect : public Figure
{
public:
	Rect(const int& width, const int& height);

	std::string Name() override;

	double Perimeter() override;

	double Area() override;

private:
	const std::string name_;
	const int width_, height_;

	void calculate_perimeter() override;
	void calculate_area() override;

protected:
	double perimeter_;
	double area_;
};


class Circle : public Figure
{
public:
	Circle(const int& r);

	std::string Name() override;

	double Perimeter() override;

	double Area() override;

private:
	const std::string name_;
	const int r_;

	void calculate_perimeter() override;
	void calculate_area() override;

protected:
	double perimeter_;
	double area_;
};


std::shared_ptr<Figure> CreateFigure(std::istringstream& is);


void RunFigures();
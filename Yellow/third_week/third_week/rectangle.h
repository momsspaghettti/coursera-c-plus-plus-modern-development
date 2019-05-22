#pragma once
#include "pch.h"


class Rectangle {
public:
	Rectangle(int width, int height);

	int GetArea() const;

	int GetPerimeter() const;

	int GetWidth() const;
	int GetHeight() const;

private:
	int width_, height_;
};
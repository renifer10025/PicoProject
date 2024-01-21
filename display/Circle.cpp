#include "Circle.h"

Circle::Circle()
{
	Radius = 0;
}

Circle::~Circle()
{
	Radius = 0;
}

// Draw object
int Circle::Draw(int X, int Y)
{
	std::cout << "Class: Circle" << std::endl;
	return 0;
}

std::string Circle::GetType()
{
	return "Circle";
}

int Circle::GetTypeId()
{
	return 3;
}

int Circle::GetRadius()
{
	return Radius;
}

int Circle::SetRadius(int R)
{
	Radius = R;
	return 0;
}
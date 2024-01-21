#include "Rectangle.h"
Rectangle::Rectangle()
{
	EndPos.PosX = 0;
	EndPos.PosY = 0;
}

Rectangle::~Rectangle()
{
	EndPos.PosX = 0;
	EndPos.PosY = 0;
}

//Draw object
int Rectangle::Draw(int X, int Y)
{
	std::cout << "Class: Rectangle" << std::endl;
	return 0;
}

std::string Rectangle::GetType()
{
	return "Rectangle";
}

int Rectangle::GetTypeId()
{
	return 2;
}

Position Rectangle::GetEndPos()
{
	return EndPos;
}

int Rectangle::GetPosX()
{
	return EndPos.PosX;
}

int Rectangle::GetPosY()
{
	return EndPos.PosY;
}

int Rectangle::SetEndPos(int X, int Y)
{
	EndPos.PosX = X;
	EndPos.PosY = Y;
	return 0;
}
#include "Line.h"

Line::Line()
{
	EndPosXY.PosX = 0;
	EndPosXY.PosY = 0;

}

Line::~Line()
{
	EndPosXY.PosX = 0;
	EndPosXY.PosY = 0;

}

// Draw object
int Line::Draw(int X, int Y)
{
	std::cout << "Class: Line" << std::endl;
	return 0;
}

std::string Line::GetType()
{
	return "Line";
}

int Line::GetTypeId()
{
	return 1;
}

// Returns object's X and Y coordinates as Position
Position Line::GetPos()
{
	return EndPosXY;
}

// Sets object's X and Y position
int Line::SetEndPos(int X, int Y)
{
	EndPosXY.PosX = X;
	EndPosXY.PosY = Y;
	return 0;
}
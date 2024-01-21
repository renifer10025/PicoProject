#pragma once
#include "Object.h"

class Line : public Object
{
	Position EndPosXY;

public:
	Line();
	~Line();
	virtual int Draw(int X = 0, int Y = 0);
	virtual std::string GetType();
	virtual int GetTypeId(); // Returns 1 for Line Class

	Position GetPos();
	int SetEndPos(int X, int Y);
};
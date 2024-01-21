#pragma once
#include "Object.h"

class Rectangle : public Object
{
	Position EndPos;

public:
	Rectangle();
	~Rectangle();
	virtual int Draw(int X = 0, int Y = 0);
	virtual std::string GetType();
	virtual int GetTypeId(); // Returns 2 for Rectangle Class

	Position GetEndPos();
	int GetPosX();
	int GetPosY();
	int SetEndPos(int X, int Y);
};


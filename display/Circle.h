#pragma once
#include "Object.h"

class Circle : public Object
{
	int Radius;

public:

	Circle();
	~Circle();
	virtual int Draw(int X = 0, int Y = 0);
	virtual std::string GetType();
	virtual int GetTypeId(); // Returns 3 for Circle Class

	int GetRadius();
	int SetRadius(int R);
};


#include <iostream>
#include "Object.h"
#include "Line.h"
#include "Rectangle.h"
#include "Circle.h"

int Object::ObjectCount = 0;
std::vector<Object*> Object::ObjectRegister;

Object::Object()
{
	OriginPosXY.PosX = 0;
	OriginPosXY.PosY = 0;
	ObjectCount++;
	AddHistoryObject();
}

Object::~Object()
{
	OriginPosXY.PosX = 0;
	OriginPosXY.PosY = 0;
	ObjectCount--;
}

// Draw object
int Object::Draw(int X, int Y)
{
	std::cout << "Class: Object" << std::endl;
	return 0;
}

std::string Object::GetType()
{
	return "Object";
}

int Object::GetTypeId()
{
	return 0;
}

// Returns object's X and Y coordinates as Position
Position Object::GetPos()
{
	return OriginPosXY;
}

// Returns object's X coordinate
int Object::GetPosX()
{
	return OriginPosXY.PosX;
}

// Returns object's Y coordinate
int Object::GetPosY()
{
	return OriginPosXY.PosY;
}

// Sets object's X and Y position
int Object::SetPos(int X, int Y)
{
	OriginPosXY.PosX = X;
	OriginPosXY.PosY = Y;
	return 0;
}

int Object::AddHistoryObject()
{
	ObjectRegister.push_back(this);
	return 0;
}

int Object::PopLastObjectRegisterElement()
{
	ObjectRegister.pop_back();
	return 0;
}

int Object::RemoveAllObjectRegisterElements()
{
	ObjectCount = 0;
	while (!ObjectRegister.empty())
	{
		//ObjectRegister.back()->~Object();
		ObjectRegister.pop_back();
	}
	return 0;
}

int Object::AddObjectToObjectRegister(int ObjectClass, int X, int Y)
{
	ObjectCount++;

	switch (ObjectClass)
	{
	case 0:	// Object
	{
		Object* o = new Object;
		o->SetPos(X, Y);
				
		ObjectRegister.push_back(o);
		break;
	}
		

	case 1: // Line
	{
		Line* l = new Line;
				
		ObjectRegister.push_back(l);
		break;
	}
		

	case 2: // Rectangle
	{
		Rectangle* r = new Rectangle;
				
		ObjectRegister.push_back(r);
		break;
	}
		

	case 3: // Circle
	{
		Circle* c = new Circle;
		
		ObjectRegister.push_back(c);
		break;
	}
		
	
	default:
		std::cout << "Unidentified object identifier" << std::endl;
		break;
	}


	
	return 0;
}

int Object::GetObjectCount()
{
	return ObjectCount;
}
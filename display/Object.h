#pragma once
#include "Utility.h"

class Object
{
	friend class FileManager;

protected:
	Position OriginPosXY;

	static int ObjectCount;
	static std::vector<Object*> ObjectRegister;

public:
	Object();
	~Object();
	virtual int Draw(int X = 0, int Y = 0);
	virtual std::string GetType();
	virtual int GetTypeId(); // Returns 0 for Object Class
	
	Position GetPos();
	int GetPosX();
	int GetPosY();
	int SetPos(int X, int Y);
	int AddHistoryObject();	// Figure out history object
	int PopLastObjectRegisterElement();
	int RemoveAllObjectRegisterElements();
	int AddObjectToObjectRegister(int ObjectClass, int X = 0, int Y = 0);

	static int GetObjectCount();
};


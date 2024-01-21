#pragma once
#include <iostream>
#include <fstream>
#include <vector>

struct Position
{
	int PosX;
	int PosY;
};

struct DisplaySize
{
	int Width;
	int Height;
};

struct History
{
	//std::vector<Object> ObjectRegister;
};

class Object;

class FileManager
{
	friend class Object;
private:
	std::string FilePath;
	std::fstream File;
	bool IsValid;

public:
	FileManager();
	FileManager(std::string C_FilePath);
	~FileManager();

	std::string GetFilePath();
	int ResetFile();
	int SaveToFile(std::string Str);
	int SaveHistoryToFile();
	int SaveToNewFile(std::string Str);
	std::string ReadFromFile(std::string C_FilePath);
	
};

/* // Big endian
// MSB -> LSB
int ArrToBitNumber(int* Arr)
{
    int Number = 0;
    
    for (int i = 0; i < 8; i++)
    {
        Number += Arr[i];
        if (i < 8)
            Number = Number << 1;
       
        // std::cout << "Number: " << Number << " | i: " << i << std::endl;
    }
    return Number;
}


void NumberToBitArray(int Number, int* arr)
{
    for (int i = 7; i >= 0; i--)
    {
        arr[i] = Number % 2;
        Number /= 2;
    }
} */
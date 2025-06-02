//#pragma once
#include <iostream>
#include "Utility.h"
#include <string>
#include "Object.h"

FileManager::FileManager()
{
	IsValid = false;
	File.open("NewFile.txt");	//app or trunc
	if (!File.is_open())
	{
		/*try
		{
			File.open("NewFile.txt", std::fstream::out);
			if (!File.is_open())
			{
				std::runtime_error e("Nie można otworzyć pliku!");
				//throw e;
			}
			else
				std::cout << "File was opened!" << std::endl;
			
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}*/
		
		
		//File.setf(std::fstream::in);//	check wether or not this works!

	}
	else
	{
		std::cout << "NewFile.txt was opened!" << std::endl;
	}
	
	FilePath = "NewFile.txt";
}

FileManager::FileManager(std::string C_FilePath)
{
	FilePath = C_FilePath;
	File.open(FilePath);
	IsValid = true;
	std::string a = "Hello World!";
	File << a;

}

FileManager::~FileManager()
{
	FilePath = "";
	File.close();
	IsValid = false;
}

std::string FileManager::GetFilePath()
{
	return FilePath;
}

int FileManager::ResetFile()
{
	return 0;
}

int FileManager::SaveToFile(std::string Str)
{
	File << Str;
	return 0;
}

int FileManager::SaveHistoryToFile()
{
	Object o;
	int ObjCount = o.GetObjectCount() - 1;

	File << "Object History:\n";
	File << "Object Count" << ": " << ObjCount << " | Class(X:Y)\n" << "0 - Object | 1 - Line | 2 - Rectangle | 3 - Circle\n";

	for (int i = 0; i < ObjCount; i++)
	{
		File << o.ObjectRegister[i]->GetTypeId() << "(" << o.ObjectRegister[i]->GetPosX() << ":" << o.ObjectRegister[i]->GetPosY() << ")" << "\n";
		//std::cout << o.ObjectRegister[i]->GetType() << std::endl;
	}
	
	return 0;
}

int SaveToNewFile(std::string Str)
{
	return 0;
}

std::string FileManager::ReadFromFile(std::string C_FilePath)
{
	File.close();
	
	std::string OutputString = "This is a output string";

	std::cout << "Reading file" << std::endl;
	
	//File.open(File.GetFilePath(), std::fstream::in); // Get this working

	// Class(X:Y)
	
	if (File.is_open())
	{
		std::cout << "-" << std::endl;
		int a = 10;

		while (!File.eof() || a > 0)
		{
			File >> OutputString;
			std::getline(File, OutputString);
			std::cout << OutputString;
			a--;
			std::cout << a << std::endl;
		}
	}
	else
		std::cout << "Failed to open a file!" << std::endl;
	

	std::cout << "Ending file reading!" << std::endl;

	return OutputString;
}
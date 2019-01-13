#pragma once
#include "DataRoom.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
namespace fs = std::experimental::filesystem;


#pragma warning(disable:4996)

struct WorldFile {
	std::string world[200][200];
};


class WriteWorld
{
public:
	WriteWorld();
	~WriteWorld();
	void createWorldFile();
	void getFileNamesString(std::vector<std::string> &filesStringFormat);
	void readRoomFile(); // verification test


private:
	void writeRoomFile();

};


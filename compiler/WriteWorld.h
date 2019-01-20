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
	char world [25] = "";
};			

struct WorldFileMaster {
	WorldFile worldFile[10][10];  // vertical and then horizontals
};

class WriteWorld
{
public:
	WriteWorld();
	~WriteWorld();
	void createWorldFile();	
	void readRoomFile(); // verification test

private:
	void readRooms(std::string room, WorldFileMaster &worldFileMaster);
	void writeWorldFile(WorldFileMaster &world);
	void getFileNamesString(std::vector<std::string> &filesStringFormat);
};


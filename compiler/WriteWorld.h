#pragma once
#include "DataRoom.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

namespace fs = std::experimental::filesystem; 

#pragma warning(disable:4996)

// world structs
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
	virtual void createWorldFile();	
	virtual void readRoomFile(); // verification test

	const std::string JSW_FILE = ".jsb";

private:
	void readRooms(std::string room, WorldFileMaster &worldFileMaster);
	void writeWorldFile(WorldFileMaster &world);
	
protected:
	void getFileNamesString(std::vector<std::string> &filesStringFormat, std::string fileType);
};




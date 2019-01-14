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
	std::string world[200][200]; // vertical and then horizontal
};											  

class WriteWorld
{
public:
	WriteWorld();
	~WriteWorld();
	void createWorldFile();	
	void readRoomFile(); // verification test

private:
	void readRooms(std::string room, WorldFile &world);
	void writeWorldFile(WorldFile &world);
	void getFileNamesString(std::vector<std::string> &filesStringFormat);
};


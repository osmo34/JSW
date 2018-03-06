#pragma once
#include "DataRoom.h"
#include <fstream>
#include <String>
#include <iostream>


// this will at a later stage become a compiler
class WriteRoom
{
public:
	WriteRoom();
	~WriteRoom();
	void createRoom(const std::string fileName, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
};


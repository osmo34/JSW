#pragma once
#include "DataRoom.h"
#include <fstream>
#include <String>
#include <iostream>

class LoadRoom
{
public:
	LoadRoom();
	~LoadRoom();

	Room loadRoom(const std::string fileName);

private:
};


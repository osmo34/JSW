#pragma once
#include "DataRoom.h"
#include <fstream>
#include <String>
#include <iostream>
#include <cstdint>
#include <vector>
#include <memory>

#pragma warning(disable:4996)

struct types {
	std::string type[20] = { "num", "roomID", "type", "texture", "px", "py", "sx", "sy", "clampXLeft",
		"clampXRight", "clampYTop", "clampYBottom", "stbx", "stby", "sttx", "stty", "ropeX", "ropeY", "ropeTop", "travelator" };
};

class WriteRoom
{
public:
	WriteRoom();
	~WriteRoom();
	void createRoom(const std::string fileName, std::string inputTextFile);

private:
	Room createRoomData(std::string inputTextFile);
	void updateRoomData(RoomData *roomData, std::string textType, std::string value);

};


#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#pragma pack(push, 2)
struct RoomData {
	char objectType;
	//std::uint32_t textureId;
	char textureFileName[25]; // texture file name
	char displayRoomName[50]; // display name for our room							   
	float positionX;
	float positionY;
	float speedX;
	float speedY;
	std::uint16_t isTravelator;
	float clampXRight;
	float clampXLeft;
	float clampYTop;
	float clampYBottom;
	float stairBottomX;
	float stairBottomY;
	float stairTopX;
	float stairTopY;
	float ropeX;
	float ropeY;
	float ropeTopY;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Room {
	std::int32_t roomLevelVertical; // get room elevation
	std::int32_t roomId;	
	RoomData roomData[920];
	std::uint32_t numObjects;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct World {
	std::string fileNames[200][200];	
};
#pragma pack(pop)


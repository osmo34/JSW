#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#pragma pack(push, 2)
struct RoomData {
	char objectType;
	std::uint32_t textureId;
	float positionX;
	float positionY;
	float speedX;
	float speedY;
	float stairBottomX;
	float stairBottomY;
	float stairTopX;
	float stairTopY;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Room {
	std::int32_t roomId;
	RoomData roomData[256];
	std::uint8_t numObjects;
};
#pragma pack(pop)

// TODO: likely use this when we have more levels
#pragma pack(push, 2)
struct World {
	std::string fileNames[256];
};
#pragma pack(pop)


#pragma once
#include <cstdint>

#pragma pack(push, 2)
struct RoomData {
	char objectType;
	char textureId;
	float positionX;
	float positionY;
	float speedX;
	float speedY;	
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Room {
	std::uint32_t roomNumber;
	RoomData roomData[256];
	std::uint8_t numObjects;
};
#pragma pack(pop)

// TODO: likely use this when we have more levels
#pragma pack(push, 2)
struct World {
	Room rooms[256];
};
#pragma pack(pop)
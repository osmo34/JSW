#include "LoadRoom.h"

LoadRoom::LoadRoom(){}


LoadRoom::~LoadRoom(){}

Room LoadRoom::loadRoom(std::string fileName) {
	Room room{};
	std::ifstream inFile;
	inFile.open(fileName, std::ios::binary);

	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&room), sizeof(Room));
		inFile.close();
	}
	else {
		std::cout << "read failed";
	}

	return room;
}

#include "WriteRoom.h"

WriteRoom::WriteRoom() {}
WriteRoom::~WriteRoom() {}


void WriteRoom::updateRoomData(RoomData *roomData, std::string textType, std::string value)
{
	types type;
	std::string output;
	int id;	

	// floats that represent the roomData data structure, anything that isn't a float and is handled elsewhere is represented by a 0
	float roomTypes[20] = {0.0f, 0.0f, 0.0f, 0.0f, roomData->positionX, roomData->positionY, roomData->speedX, roomData->speedY, 0.0f,
								roomData->clampXLeft, roomData->clampXRight, roomData->clampYTop, roomData->clampYBottom, roomData->stairBottomX,
								roomData->stairBottomY, roomData->stairTopX, roomData->stairTopY, roomData->ropeX, roomData->ropeY, roomData->ropeTopY};

	// work out which bit of roomData we are working with
	for (int i = 0; i < sizeof(type.type) / sizeof(type.type[0]); i++) {
		if (textType == type.type[i]) {
			output = type.type[i];
			id = i;
			roomTypes[id] = std::stof(value);
			break;
		}
	}

	// re-copy all the data back to the pointer so it's not lost with scope
	roomData->positionX = roomTypes[4];
	roomData->positionY = roomTypes[5];
	roomData->speedX = roomTypes[6];
	roomData->speedY = roomTypes[7];
	roomData->clampXLeft = roomTypes[9];
	roomData->clampXRight = roomTypes[10];
	roomData->clampYTop = roomTypes[11];
	roomData->clampYBottom = roomTypes[12];
	roomData->stairBottomX = roomTypes[13];
	roomData->stairBottomY = roomTypes[14];
	roomData->stairTopX = roomTypes[15];
	roomData->stairTopY = roomTypes[16];
	roomData->ropeX = roomTypes[17];
	roomData->ropeY = roomTypes[18];
	roomData->ropeTopY = roomTypes[19];
	roomData->isTravelator = 0; // make sure we are not a travelator - this is handled elsewhere!
}


Room WriteRoom::createRoomData(std::string inputTextFile)
{
	Room room{};	

	std::string file;	

	if (inputTextFile != "") {
		file = inputTextFile;
	}
	else {
		std::cout << "Enter file name to compile including extension (*.txt): > ";		
		std::cin >> file;
		std::cout << "" << std::endl;
	}	

	std::ifstream inFile;
	inFile.open(file);
	if (!inFile.is_open()) {
		std::cout << "read failed";
	}
	RoomData *roomData = new RoomData;
	int i = 0;

	while (inFile) {
		std::string type;
		std::getline(inFile, type, ':');
		std::string value;
		inFile >> value;
		inFile >> std::ws;

		if (type == "num") {
			room.numObjects = std::stoi(value);
		}
		else if (type == "roomID") {
			room.roomId = std::stoi(value);
		}

		else if (type == "type") {
			char *c = new char[value.length() + 1];
			strcpy(c, value.c_str());
			roomData->objectType = *c;
			delete[] c;
		}
		else if (type == "texture") {
			roomData->textureId = std::stoi(value);
		}
		else if (type == "travelator") {
			if (value == "1") {
				roomData->isTravelator = 1;
			}
			else {
				roomData->isTravelator = 0;
			}
		}		 
		else if (type == "end") {
			room.roomData[i] = *roomData;
			i++;
		}
		else {			
			updateRoomData(roomData, type, value);
		}	
	}

	inFile.close();
	std::cout << "Compile complete" << std::endl;
	delete[] roomData;
	return room;
}

void WriteRoom::createRoom(std::string fileName, std::string inputTextFile)
{
	Room room = createRoomData(inputTextFile);

	std::ofstream outputFile;

	outputFile.open(fileName, std::ios::binary);

	if (outputFile.is_open()) {
		outputFile.write(reinterpret_cast<char*>(&room), sizeof(room));
		outputFile.close();
	}
	else {
		std::cout << "write failed";
	}

	std::cout << "verification" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << room.roomData[i].objectType << std::endl;
		std::cout << room.roomData[i].positionX << std::endl;
	}
}

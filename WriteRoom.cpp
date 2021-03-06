// this code is depreciated - please refer to seperate compiler.

#include "WriteRoom.h"

WriteRoom::WriteRoom(){}
WriteRoom::~WriteRoom(){}


Room WriteRoom::createRoomData()
{
	Room room{};

	std::string file = "testLevel.txt";
	std::ifstream inFile;
	inFile.open(file);
	if (!inFile.is_open()) {
		std::cout << "read failed";
	}
	RoomData roomData{};
	int i = 0;

	while (inFile) {
		std::string type;
		std::getline(inFile, type, ':');
		std::string value;
		inFile >> value;
		inFile >> std::ws;
		// TODO: Refactor this
		if (type == "num") {
			room.numObjects = std::stoi(value);
		}
		else if (type == "roomID") {
			room.roomId = std::stoi(value);
		}
		
		else if (type == "type") {		
			char *c = new char[value.length() + 1];
			strcpy(c, value.c_str());
			roomData.objectType = *c;
			delete c;
		}
		else if (type == "texture") {
			roomData.textureId = std::stoi(value);
		}
		else if (type == "px") {
			roomData.positionX = std::stof(value);
		}
		else if (type == "py") {
			roomData.positionY = std::stof(value);
		}
		else if (type == "sx") {
			roomData.speedX = std::stof(value);
		}
		else if (type == "sy") {
			roomData.speedY = std::stof(value);
		}
		else if (type == "stbx") {
			roomData.stairBottomX = std::stof(value);
		}
		else if (type == "stby") {
			roomData.stairBottomY = std::stof(value);
		}
		else if (type == "sttx") {
			roomData.stairTopX = std::stof(value);
		}
		else if (type == "stty") {
			roomData.stairTopY = std::stof(value);
		}										  
		else if (type == "end") {
			room.roomData[i] = roomData;
			i++;			
		}
	}

	inFile.close();		
	return room;
}

void WriteRoom::createRoom(std::string fileName, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	//To compile from txt file use the following:
	//std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);
	//writeRoom->createRoom(fileName, SCREEN_WIDTH, SCREEN_HEIGHT);

	Room room = createRoomData();

	std::ofstream outputFile;

	outputFile.open(fileName, std::ios::binary);

	if (outputFile.is_open()) {
		outputFile.write(reinterpret_cast<char*>(&room), sizeof(room));
		outputFile.close();
	}
	else {
		std::cout << "write failed";
	}
}

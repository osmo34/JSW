#include "WriteWorld.h"

WriteWorld::WriteWorld() {}	
WriteWorld::~WriteWorld() {}

void WriteWorld::createWorldFile() {																	
	WorldFileMaster world;
					  
	// create a vector of jsb files in current directory by name only
	std::vector <std::string> filesStringFormat;		
	getFileNamesString(filesStringFormat, JSW_FILE);

	// update world file using the room names
	for (auto &it : filesStringFormat) {
		readRooms(it, world);
	}	
	// write the world file
	writeWorldFile(world);	
}

void WriteWorld::writeWorldFile(WorldFileMaster &world) {
	std::ofstream outputFile;
	outputFile.open("world.jsw", std::ios::binary);

	if (outputFile.is_open()) {
		outputFile.write(reinterpret_cast<char*>(&world), sizeof(world));
		outputFile.close();
	}
	else {
		std::cout << "world write failed";
	}
}

void WriteWorld::getFileNamesString(std::vector<std::string> &filesStringFormat, std::string fileType) {
	for (const auto & entry : fs::directory_iterator(fs::current_path())) {
		std::string file = entry.path().string(); // get all files initially including paths
		std::string fileName; // full file name

		char seperator = '\\';

		// get rid of the path so we are working with the file name only
		size_t i = file.rfind(seperator, file.length());
		if (i != std::string::npos) {
			fileName = file.substr(i + 1, file.length() - i); 
			// check if jsb file format and if so add it to the vector
			if (fileName.find(fileType) != std::string::npos) {
				filesStringFormat.push_back(fileName);
			}
		}		
	}
}							

void WriteWorld::readRooms(std::string roomName, WorldFileMaster &worldFileMaster) {
	Room room{}; // create a temporary room
	std::ifstream inFile;
	inFile.open(roomName, std::ios::binary);
	
	// create the room from the file we are working with
	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&room), sizeof(Room));
		inFile.close();
	}
	else {
		std::cout << "Room read failed";
	}
	// get our vertical and horizontal positions
	int roomVert = room.roomLevelVertical;
	int roomID = room.roomId; 		
	strcpy(worldFileMaster.worldFile[roomVert][roomID].world, roomName.c_str()); // convert the string to char
}

// test method which will be used in game, copy in compiler for test purposes. Leaving in for verification
void WriteWorld::readRoomFile()	{
	//WorldFile worldFile;
	WorldFileMaster worldFile{};
	std::ifstream inFile;
	inFile.open("world.jsw", std::ios::binary);

	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&worldFile), sizeof(worldFile));
		inFile.close();
	}
	else {
		std::cout << " World Read failed";
	}
	std::cout << "Verification" << std::endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			std::string output = worldFile.worldFile[i][j].world;
			std::cout << output << " : " << i << " " << j << std::endl;
		}
	}
	std::cout << "World compile complete " << std::endl;
	system("pause");   	
}
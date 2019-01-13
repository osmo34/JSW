#include "WriteWorld.h"

WriteWorld::WriteWorld()
{
}


WriteWorld::~WriteWorld()
{
}

void WriteWorld::createWorldFile()
{																	
	WorldFile world;

	// TODO: test
	//std::string test1 = "test.jsb";
	//std::string test2 = "test2.jsb";
	//world.world[0][0] = test1;
	//world.world[0][1] = test2;

	// create a vector of jsb files in current directory by name only
	std::vector <std::string> filesStringFormat;		
	getFileNamesString(filesStringFormat);

	// TODO: test
	//for (auto &it : filesStringFormat) {
	//	std::cout << it << std::endl;
	//}
	
	// TODO: check each file and obtain the height followed by level id and then update world
		
	


	
	std::ofstream outputFile;
	outputFile.open("world.jsw", std::ios::binary);

	if (outputFile.is_open()) {
		outputFile.write(reinterpret_cast<char*>(&world), sizeof(world));
		outputFile.close();
	}
	else {
		std::cout << "write failed";
	}
	
}

void WriteWorld::getFileNamesString(std::vector<std::string> &filesStringFormat)
{
	for (const auto & entry : fs::directory_iterator(fs::current_path())) {
		std::string file = entry.path().string(); // get all files initially including paths
		std::string fileName; // full file name

		char seperator = '\\';
		std::string extension = "jsb";

		// get rid of the path so we are working with the file name only
		size_t i = file.rfind(seperator, file.length());
		if (i != std::string::npos) {
			fileName = file.substr(i + 1, file.length() - i); 		
		}

		// check if jsb file format and if so add it to the vector
		if (fileName.find(".jsb") != std::string::npos) {
			filesStringFormat.push_back(fileName);
		}
	}
}

// test method which will be used in game, copy in compiler for test purposes
void WriteWorld::readRoomFile()
{
	WorldFile worldFile;
	std::ifstream inFile;
	inFile.open("world.jsw", std::ios::binary);

	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&worldFile), sizeof(worldFile));
		inFile.close();
	}
	else {
		std::cout << "Read failed";
	}
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			std::cout << worldFile.world[i][j] << std::endl;
		}
	}			
}

void WriteWorld::writeRoomFile()
{
}

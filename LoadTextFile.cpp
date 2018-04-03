#include "LoadTextFile.h"

LoadTextFile::LoadTextFile(std::string fileName) : fileName(fileName) {}
LoadTextFile::~LoadTextFile(){}

void LoadTextFile::loadFile(std::vector<std::string> &v)
{
	std::ifstream inFile;
	inFile.open(fileName);
	if (!inFile.is_open()) { std::cout << "read textures failed "; }

	while (inFile) {
		std::string textName;
		std::getline(inFile, textName, ';');	
		inFile >> std::ws;
		if (textName == "") return; 
		v.push_back(textName);		
	}
}

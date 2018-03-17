#include "LoadTextures.h"

LoadTextures::LoadTextures(std::string fileName) : fileName(fileName) {}
LoadTextures::~LoadTextures(){}


void LoadTextures::loadTextures(std::vector<std::string> &textures)
{
	std::ifstream inFile;
	inFile.open(fileName);
	if (!inFile.is_open()) { std::cout << "read textures failed "; }

	while (inFile) {
		std::string textureName;
		std::getline(inFile, textureName, ';');	
		inFile >> std::ws;
		textures.push_back(textureName);		
	}
}

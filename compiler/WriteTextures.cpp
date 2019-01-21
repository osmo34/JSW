#include "WriteTextures.h"

WriteTextures::WriteTextures() {}
WriteTextures::~WriteTextures()	{}

void WriteTextures::createWorldFile()
{
	TextureFileMaster textureFileMaster;
	std::vector <std::string> texturesStringFormat;
	getFileNamesString(texturesStringFormat, TEXTURE_FILE);

	writeTextureFile(texturesStringFormat, textureFileMaster);
} 								 

void WriteTextures::writeTextureFile(std::vector<std::string> &texturesStringFormat, TextureFileMaster & textureFileMaster)
{ 	
	for (int i = 0; i < texturesStringFormat.size(); i++) {
		strcpy(textureFileMaster.textureFile[i].texture, texturesStringFormat[i].c_str());		 
	}
	
	std::ofstream outputFile;
	outputFile.open("textures.txx", std::ios::binary);

	if (outputFile.is_open()) {
		outputFile.write(reinterpret_cast<char*>(&textureFileMaster), sizeof(textureFileMaster));
		outputFile.close();
	}
	else {
		std::cout << "texture write failed";
	}
}

void WriteTextures::readRoomFile() {
	TextureFileMaster textureFile{};

	std::ifstream inFile;
	inFile.open("textures.txx", std::ios::binary);

	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&textureFile), sizeof(textureFile));
		inFile.close();
	}
	else {
		std::cout << "texture file read failed";
	}

	std::cout << "verification" << std::endl;

	for (int i = 0; i < 5; i++) {
		std::string output = textureFile.textureFile[i].texture;
		std::cout << output << std::endl;
	}
	std::cout << "Texture file compile complete " << std::endl;
	system("pause");
}

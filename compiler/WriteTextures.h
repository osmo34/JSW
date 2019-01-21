#pragma once
#include "WriteWorld.h"

// texture structs

#pragma warning(disable:4996)
struct Texture {
	char texture[25] = "";
};
struct TextureFileMaster {
	Texture textureFile[100]; // 100 textures for now
};

class WriteTextures :
	public WriteWorld
{
public:
	WriteTextures();
	~WriteTextures();
	void createWorldFile();
	void readRoomFile();
	const std::string TEXTURE_FILE = ".png";

private:	
	void writeTextureFile(std::vector<std::string> &texturesStringFormat, TextureFileMaster &textureFileMaster);
};


#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>

class LoadTextures
{
public:
	LoadTextures(std::string fileName);
	~LoadTextures();
	void loadTextures(std::vector<std::string> &textures);

private:
	std::string fileName;
};


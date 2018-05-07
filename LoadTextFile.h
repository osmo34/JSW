#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <unordered_map>

class LoadTextFile
{
public:
	LoadTextFile(std::string fileName);
	~LoadTextFile();
	void loadFile(std::vector<std::string> &v);

private:
	std::string fileName;
};


#include "WriteRoom.h"
#include <memory>
#include <string>
#include <iostream>

int main() {
	std::cout << "Enter compiled room file name including extension (*.jsb) >> ";
	std::string fileName;
	std::cin >> fileName;	
	std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);	
	writeRoom->createRoom(fileName);
	system("pause");
	return 0;
}
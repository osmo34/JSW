#include "WriteRoom.h"
#include <memory>
#include <string>
#include <iostream>

int main() {
	while (true) {
		std::cout << "Enter compiled room file name including extension (*.jsb) >> ";
		std::string fileName;
		std::cin >> fileName;
		std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);
		writeRoom->createRoom(fileName);
		std::cout << "" << std::endl;
	}
	system("pause");
	return 0;
}
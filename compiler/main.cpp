#include "WriteRoom.h"
#include <memory>
#include <string>
#include <iostream>
#include "main.h"

int main(int argc, char *argv[]) {
	while (true) {
		if (argc == 3) {
			std::string output = argv[2];
			std::string input = argv[1];
			CreateRoom(output, input);
			break;
		}
		else {
			CreateRoom();
		}					  
	}
	system("pause");
	return 0;
}
//
void CreateRoom() {
	std::cout << "Enter compiled room file name including extension (*.jsb) >> ";
	std::string fileName;
	std::cin >> fileName;
	std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);
	writeRoom->createRoom(fileName, "");
	std::cout << "" << std::endl;
}

void CreateRoom(std::string output, std::string input) {
	std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);
	writeRoom->createRoom(output, input);
}


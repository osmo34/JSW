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
			int input;
			do {
				system("cls");
				std::cout << "==========================" << std::endl;
				std::cout << "JSB Compiler: " << std::endl;
				std::cout << "==========================" << std::endl;
				std::cout << "(1) Compile room " << std::endl;
				std::cout << "(2) Compile world file " << std::endl;
				std::cout << "(0) Quit " << std::endl;

				std::cin >> input;

				if (input == 1) {
					CreateRoom();
				}
				else if (input == 2) {
					CreateWorld();
				}
				else if (input == 0) {
					return 0;
				}
			} while (input != 1 or input != 2);
		}					  
	}
	system("pause");
	return 0;
}
// if no program arguments
void CreateRoom() {
	std::cout << "Enter compiled room file name including extension (*.jsb) >> ";
	std::string fileName;
	std::cin >> fileName;
	std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);
	writeRoom->createRoom(fileName, "");
	std::cout << "" << std::endl;
}

// with arguments, likely use from level editor
void CreateRoom(std::string output, std::string input) {
	std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);
	writeRoom->createRoom(output, input);
}

void CreateWorld() {
	std::shared_ptr <WriteWorld> writeWorld(new WriteWorld);
	writeWorld->createWorldFile();
	writeWorld->readRoomFile();
}




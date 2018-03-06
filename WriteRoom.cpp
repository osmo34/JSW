#include "WriteRoom.h"

WriteRoom::WriteRoom(){}
WriteRoom::~WriteRoom(){}

void WriteRoom::createRoom(std::string fileName, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	// TODO: this is for testing only. The following data will eventually be loaded from an external file and then compiled
	// Test code for writing a level file as follows - 
	//std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);
	//writeRoom->createRoom(fileName, SCREEN_WIDTH, SCREEN_HEIGHT);

	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY = 'e', ENEMY_MOVING = 'm', ENEMY_STATIC = 'n', PICK_UP = 'u';
	const char TEST_TEXTURE = 't';

	Room room;
	room.roomNumber = 0;

	// create level objects
	RoomData roomData;
	roomData.objectType = STATIC_OBJECT;
	roomData.textureId = TEST_TEXTURE;
	roomData.positionX = 600.0f;
	roomData.positionY = SCREEN_HEIGHT - 50.0f;
	roomData.speedX = 0.0f;
	roomData.speedY = 0.0f;

	RoomData roomData1;
	roomData1.objectType = STATIC_OBJECT;
	roomData1.textureId = TEST_TEXTURE;
	roomData1.positionX = 800.0f;
	roomData1.positionY = SCREEN_HEIGHT - 50.0f;
	roomData1.speedX = 0.0f;
	roomData1.speedY = 0.0f;

	RoomData roomData2;
	roomData2.objectType = STATIC_OBJECT;
	roomData2.textureId = TEST_TEXTURE;
	roomData2.positionX = 900.0f;
	roomData2.positionY = SCREEN_HEIGHT - 110.0f;
	roomData2.speedX = 0.0f;
	roomData2.speedY = 0.0f;

	RoomData roomData3;
	roomData3.objectType = STATIC_OBJECT;
	roomData3.textureId = TEST_TEXTURE;
	roomData3.positionX = 1000.0f;
	roomData3.positionY = SCREEN_HEIGHT - 170.0f;
	roomData3.speedX = 0.0f;
	roomData3.speedY = 0.0f;

	RoomData enemy1;
	enemy1.objectType = ENEMY_MOVING;
	enemy1.textureId = TEST_TEXTURE;
	enemy1.positionX = 50.0f;
	enemy1.positionY = SCREEN_HEIGHT - 50.0f;
	enemy1.speedX = 0.0f;
	enemy1.speedY = 0.1f;

	RoomData enemy2;
	enemy2.objectType = ENEMY_MOVING;
	enemy2.textureId = TEST_TEXTURE;
	enemy2.positionX = 50.0f;
	enemy2.positionY = SCREEN_HEIGHT - 50.0f;
	enemy2.speedX = 0.1f;
	enemy2.speedY = 0.0f;

	RoomData enemy3;
	enemy3.objectType = ENEMY_STATIC;
	enemy3.textureId = TEST_TEXTURE;
	enemy3.positionX = 60.0f;
	enemy3.positionY = SCREEN_HEIGHT - 50.0f;
	enemy3.speedX = 0.0f;
	enemy3.speedY = 0.0f;

	RoomData pickup;
	pickup.objectType = PICK_UP;
	pickup.textureId = TEST_TEXTURE;
	pickup.positionX = 100;
	pickup.positionY = SCREEN_HEIGHT - 55.0f;
	pickup.speedX = 0.0f;
	pickup.speedY = 0.0f;

	room.numObjects = 8;

	// end create objects

	// TODO: will use loop long term
	room.roomData[0] = roomData;
	room.roomData[1] = roomData1;
	room.roomData[2] = roomData2;
	room.roomData[3] = roomData3;
	room.roomData[4] = enemy1;
	room.roomData[5] = enemy2;
	room.roomData[6] = enemy3;
	room.roomData[7] = pickup;

	std::ofstream outputFile;

	outputFile.open(fileName, std::ios::binary);

	if (outputFile.is_open()) {
		outputFile.write(reinterpret_cast<char*>(&room), sizeof(room));
		outputFile.close();
	}
	else {
		std::cout << "write failed";
	}


}

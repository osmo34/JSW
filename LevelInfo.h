#pragma once

static int currentRoom = 5;
static int nextRoomRight;
static int nextRoomLeft;
static int nextRoomUp;
static int nextRoomDown;
static int lives = 3;
static int itemsCollected = 0;

template <typename T>
void clearVector(std::vector<std::shared_ptr<T>> &t) {
	t.clear();
}

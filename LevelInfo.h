#pragma once

static int currentRoom = 0;
static int nextRoomRight;
static int nextRoomLeft;
static int lives = 3;
static int itemsCollected = 0;

template <typename T>
void clearVector(std::vector<std::shared_ptr<T>> &t) {
	t.clear();
}

#include "PlayerState.h"
char PlayerState::currentState = 'n';

PlayerState::PlayerState() {}						   

PlayerState::~PlayerState()	{}

void PlayerState::updateState(const char c) {
	currentState = c;
}

char PlayerState::getState() {	
	return currentState;
}

#include "EnemyStatic.h"

void EnemyStatic::update(const float dt){}

EnemyStatic::~EnemyStatic(){}

// overidden to ensure we can't move
void EnemyStatic::setUpEnemy() {
	movementSpeed.x = 0.0f;
	movementSpeed.y = 0.0f;
}

void EnemyStatic::reverseDirection(){
	movementSpeed.x = 0.0f;
	movementSpeed.y = 0.0f;
}
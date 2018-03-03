#pragma once
#include "Enemy.h"
// used for moving enemies, in either direction or diagonally if desired
class EnemyMoving :	public Enemy
{
public:
	using Enemy::Enemy;
	~EnemyMoving();
	void update(const float dt);
	void reverseDirection();

private:
	void setUpEnemy();
};


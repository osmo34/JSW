#pragma once
#include "Enemy.h"
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


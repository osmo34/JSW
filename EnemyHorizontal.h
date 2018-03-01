#pragma once
#include "Enemy.h"
class EnemyHorizontal :	public Enemy
{
public:
	using Enemy::Enemy;
	~EnemyHorizontal();
	void update(const float dt);
	void reverseDirection();

private:
	void setUpEnemy();
};


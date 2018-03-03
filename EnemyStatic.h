#pragma once
#include "Enemy.h"
//used for stationary enemies
class EnemyStatic : public Enemy
{
public:
	using Enemy::Enemy;
	void update(const float dt);
	~EnemyStatic();

private:
	void setUpEnemy();
	void reverseDirection();
};


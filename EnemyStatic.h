#pragma once
#include "Enemy.h"
//used for stationary enemies
class EnemyStatic : public Enemy
{
public:
	using Enemy::Enemy;	
	~EnemyStatic();

private:
	void update(const float dt);
	void setUpEnemy();
	void reverseDirection();
};


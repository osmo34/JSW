#pragma once
#include "Enemy.h"
//used for stationary enemies
class EnemyStatic : public Enemy
{
public:
	using Enemy::Enemy;	
	~EnemyStatic();
	void update(const float dt);

private: 	
	void setUpEnemy();
	void reverseDirection();
};


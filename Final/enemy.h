#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <vector>
#include "objects.h"

class Enemy
{
private:
	std::string Name;
	int MaxHealth;
	int CurrentHealth;
	int Armor;
	bool CanFly;
	bool IsGrounded;
	std::vector<StatusEffect> StatusEffects;
	std::vector<StatusEffect> StatusResistant;
	std::vector<StatusEffect> StatusWeakness;
	

public:
	Enemy();
	~Enemy();


};

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

#endif

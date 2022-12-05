#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <vector>
#include <tuple>
#include "Player.h"
#include "objects.h"
#include "inventory.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;


class Enemy
{
private:
	std::string Name;
	int MaxHealth;
	int CurrentHealth;
	int BaseDamage;
	int Armor;
	int XPGranted;
	bool IsGrounded;
	bool hasDied;
	InventorySlot* ItemDrop;
	std::vector<StatusEffectTracker> StatusEffects;
	std::vector<StatusEffect> StatusResistant;
	std::vector<StatusEffect> StatusWeakness;
	std::vector<AttackMove> AttackMoves;

public:
	bool CanFly;
	json Serialize();

	static Enemy* Deserialize(json data);
	static Enemy* Duplicate(Enemy* master);

	Enemy();
	/// @brief Includes options for flying enemies.
	Enemy(std::string name, int maxHealth, int baseDamage, std::vector<AttackMove> attacks, int xp,
		int armor, bool canFly, bool isGrounded, InventorySlot* itemDrop = nullptr,
		std::vector<StatusEffect> resistance = std::vector<StatusEffect>(),
		std::vector<StatusEffect> weakness = std::vector<StatusEffect>());
	Enemy(std::string name, int maxHealth, int baseDamage, std::vector<AttackMove> attacks, int xp,
		int armor, InventorySlot* itemDrop = nullptr,
		std::vector<StatusEffect> resistance = std::vector<StatusEffect>(),
		std::vector<StatusEffect> weakness = std::vector<StatusEffect>());


	void ChangeStance(bool forceChange=false);
	void Attack(Player& _player);

	/// <summary>Only returns data if the enemy has died.</summary>
	/// <returns>Name, XP reward, item drop</returns>
	tuple<std::string, int, InventorySlot*> EndTurn();

	/// <summary></summary>
	/// <returns>Name, XP reward, item drop</returns>
	tuple<std::string, int, InventorySlot*> Die();

	void AddStatusEffect(StatusEffect effect, int turnCount);


	std::string GetName();
	int GetHealth() { return CurrentHealth; }
	int GetArmor();
	bool IsGroundedCheck();
	bool DoesDropItem() { return (ItemDrop != nullptr); }
	InventorySlot* GetItemDrop() { return ItemDrop; }
	bool HasDied() { return hasDied; }

	std::vector<StatusEffect> GetStatusEffects();

	bool HasStatusEffect(StatusEffect effectSearch);

	/// <summary>
	/// Add or subtract health.
	/// </summary>
	/// <returns>The amount of health remaining after modification.</returns>
	int ModifyHealth(int healthMod);


	std::string PrintForCombat(int healthBarWidth=10);
};


#endif

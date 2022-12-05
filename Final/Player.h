#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <tuple>
#include "inventory.h"
#include "Rooms.h"
#include "objects.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Enemy;

class PlayerInventory
{
public:
	vector<InventorySlot> Slots;

	PlayerInventory();

	vector<json> Serialize();

	static PlayerInventory* Deserialize(vector<json> data);

	void PrintInventory();

	/// @brief returns nullptr if no healthpotions found
	InventorySlot* GetHealthPotions();


	/// <summary></summary>
	/// <returns>number of items left behind</returns>
	int AddItem(Item& item, int count = 1, bool supressOutput = false);
	int AddItem(MeleeWeapon& item, int count = 1, bool supressOutput = false);
	int AddItem(RangedWeapon& item, int count = 1, bool supressOutput = false);
	int AddItem(InventorySlot& item);

	void DropItem(string itemName, Room& currentRoom, int count = 1);

	/// <summary>
	/// Returns the matching item and the number of items removed.
	/// </summary>
	tuple<InventorySlot*, int> RemoveItem(string itemName, int count = 1);

	void EquipItem(string itemName, bool supressOutput = false);

	tuple<MeleeWeapon*, RangedWeapon*> GetEquipedWeapons();
};


class Player
{
private:
	Vector2 RoomLocation;
	Room* CurrentRoom;
	int PlayerLevel;
	int MaxHealth;
	int CurrentHealth;
	int CurrentXP;
	int NextLevelXP;
	int Stamina;
	vector<StatusEffectTracker> StatusEffects;
public:
	bool IsDead;
	PlayerInventory Inventory;

	json Serialize();

	static Player* Deserialize(json playerData);

	Player();

	void JumpToRoom(Room* room);
	void JumpToRoom(Vector2 location);

	string EnterRoom(string doorDirection);
	string EnterRoom(Doors doorDirection);

	string UnlockDoor(string doorDirection);

	Room* GetRoom();

	Room* AdjacentRoom(Vector2 direction);

	InventorySlot* GetKeys();

	/// @brief Not implemented. Always returns 0.
	int GetArmor() { return 0; }
	int GetStamina() { return Stamina; }

	// combat stuff

	std::string GetHealthBar(int barWidth);
	std::string GetStaminaBar(int barWidth, int usedStam);
	std::string GetXPBar(int barWidth);
	void AddXP(int xp);
	std::string UseHealthPotion();
	int GetHealAmount() { return round((float)MaxHealth * .5f); }

	void Attack(MeleeWeapon weapon, int attackIndex, Enemy& enemy);
	void Attack(RangedWeapon weapon, int attackIndex, Enemy& enemy);
	void Attack(AttackQueue attack);

	void EndTurn();
	void Die(Enemy& murderer);
	void LevelUp();


	void AddStatusEffect(StatusEffect effect, int turnCount);

	std::vector<StatusEffect> GetStatusEffects();

	bool HasStatusEffect(StatusEffect effectSearch);


	/// <summary>
	/// Add or subtract health.
	/// </summary>
	/// <returns>The amount of health remaining after modification.</returns>
	int ModifyHealth(int healthMod);
};


#endif // !PLAYER_H

#pragma once
#ifndef ROOMS_H
#define ROOMS_H

#include <vector>
#include "inventory.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class Enemy;

enum Doors {Door_None, North, East, South, West};

static unsigned int RoomIDIterator;
class Room
{
private:
	vector<InventorySlot> Items;
	vector<Doors> UnlockedDoors;
	vector<Doors> LockedDoors;
	unsigned int RoomID;
	bool playerVisited;
	bool isPlayerIn;

public:
	vector<Enemy*> Enemies;

	json Serialize();

	static Room* Deserialize(json data);

	Room(vector<Doors> unlockedDoors, vector<Doors> lockedDoors = vector<Doors>(), vector<Enemy*> enemies = vector<Enemy*>());

	Room(vector<InventorySlot> items = vector<InventorySlot>(),
		 vector<Doors> unlockedDoors = vector<Doors>(),
		 vector<Doors> lockedDoors = vector<Doors>(),
		 vector<Enemy*> enemies = vector<Enemy*>());

	bool operator==(const Room& rhs) const;

	void AddItem(Item& item, int count = 1);
	void AddItem(MeleeWeapon& item, int count = 1);
	void AddItem(RangedWeapon& item, int count = 1);
	void AddItem(InventorySlot item, int countOverride=-1);

	void EnterRoom();
	void ExitRoom();


	string DoorToString(Doors door, bool lowercase = false);


	vector<Doors>& GetUnlockedDoors();
	vector<Doors>& GetLockedDoors();
	vector<Doors> GetAllDoors();

	string UnlockDoor(Doors door);

	/// <summary></summary>
	/// <param name="foundItem">If an item was found it will be used here.</param>
	/// <returns>true if the item was successfully found.</returns>
	bool RemoveItem(InventorySlot& foundItem, string itemName, int itemCount = 100);

	/// @brief Clears all enemies and drops their items in the room.
	void ClearEnemies();

	string PrintItems();

	string PrintDoors();

	void GetItemCounts(int& items, int& weapons);

	/// @brief Returns a 5 long array of text to display the room on a map.
	string* PrintRoom(char _item, char _weapon, char _player);
};



#endif
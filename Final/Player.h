#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <tuple>
#include "inventory.h"
#include "Rooms.h"
#include "objects.h"
//#include "GameDatabase.h"
//#include "extensions.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class PlayerInventory
{
public:
	vector<InventorySlot> Slots;

	PlayerInventory();

	vector<json> Serialize();

	static PlayerInventory* Deserialize(vector<json> data);

	void PrintInventory();

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
	tuple<InventorySlot*, int> RemoveItem(string itemName, Room& currentRoom, int count = 1);

	void EquipItem(string itemName, bool supressOutput = false);
};


class Player
{
private:
	Vector2 RoomLocation;
	Room* CurrentRoom;
public:
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
};


#endif // !PLAYER_H

 #pragma once
#ifndef ROOMS_H
#define ROOMS_H

#include <vector>
//#include <regex>
#include "inventory.h"
//#include "Items.h"
//#include "Player.h"
//#include "commands.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;


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
	json Serialize();

	static Room* Deserialize(json data);

	Room(vector<Doors> unlockedDoors, vector<Doors> lockedDoors = vector<Doors>());

	Room(vector<InventorySlot> items = vector<InventorySlot>(),
		 vector<Doors> unlockedDoors = vector<Doors>(),
		 vector<Doors> lockedDoors = vector<Doors>());
	bool operator==(const Room& rhs) const;

	void AddItem(Item& item, int count = 1);
	void AddItem(MeleeWeapon& item, int count = 1);
	void AddItem(RangedWeapon& item, int count = 1);
	void AddItem(InventorySlot item, int count);

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

	string PrintItems();

	string PrintDoors();

	void GetItemCounts(int& items, int& weapons);

	/// @brief Returns a 5 long array of text to display the room on a map.
	string* PrintRoom(char _item, char _weapon, char _player);
};



#endif
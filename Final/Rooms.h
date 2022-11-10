 #pragma once
#ifndef ROOMS_H
#define ROOMS_H

#include <regex>
#include "inventory.h"
#include "Items.h"
#include "Player.h"
#include "commands.h"
using namespace std;

//extern Player player;
//extern bool exitProgram;

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
	Room(vector<Doors> unlockedDoors, vector<Doors> lockedDoors = vector<Doors>())
		:Room(vector<InventorySlot>(), unlockedDoors, lockedDoors) {}

	Room(vector<InventorySlot> items = vector<InventorySlot>(),
		 vector<Doors> unlockedDoors = vector<Doors>(),
		 vector<Doors> lockedDoors = vector<Doors>())
	{
		playerVisited = false;
		isPlayerIn = false;
		RoomID = RoomIDIterator;
		RoomIDIterator++;

		Items = items;
		UnlockedDoors = unlockedDoors;
		LockedDoors = lockedDoors;
		//for (int i = 0; i < items.size(); i++)
		//{
		//	Items.push_back(items[i]);
		//}
	}
	bool operator==(const Room& rhs) const
	{
		return RoomID == rhs.RoomID;
	}

	void AddItem(Item& item, int count = 1)
	{
		// check for existing items
		for (InventorySlot& _item : Items)
		{
			if (item.GetName() == _item.GetName())
			{
				_item.NumberInStack += count;
				return;
			}
		}

		// create inventory slot for new items
		Items.push_back(InventorySlot(item, count));
	}
	void AddItem(MeleeWeapon& item, int count = 1)
	{
		// check for existing items
		for (InventorySlot& _item : Items)
		{
			if (item.GetName() == _item.GetName())
			{
				_item.NumberInStack += count;
				return;
			}
		}

		// create inventory slot for new items
		Items.push_back(InventorySlot(item, count));
	}
	void AddItem(RangedWeapon& item, int count = 1)
	{
		// check for existing items
		for (InventorySlot& _item : Items)
		{
			if (item.GetName() == _item.GetName())
			{
				_item.NumberInStack += count;
				return;
			}
		}

		// create inventory slot for new items
		Items.push_back(InventorySlot(item, count));
	}
	void AddItem(InventorySlot item, int count)
	{
		switch (item.GetItemType())
		{
		case TItem:
			AddItem(item.GetItemAsItem(), count);
			break;
		case TMelee:
			AddItem(item.GetItemAsMelee(), count);
			break;
		case TRanged:
			AddItem(item.GetItemAsRanged(), count);
			break;
		}
	}

	void EnterRoom()
	{
		playerVisited = true;
		isPlayerIn = true;
	}
	void ExitRoom() { isPlayerIn = false; }


	string DoorToString(Doors door, bool lowercase = false)
	{
		switch (door)
		{
		case North:
			return (!lowercase) ? "North" : "north";
		case East:
			return (!lowercase) ? "East" : "east";
		case South:
			return (!lowercase) ? "South" : "south";
		case West:
			return (!lowercase) ? "West" : "west";
		default:
			return "";
		}
	}


	vector<Doors>& GetUnlockedDoors()
	{
		return UnlockedDoors;
	}
	vector<Doors>& GetLockedDoors()
	{
		return LockedDoors;
	}
	vector<Doors> GetAllDoors()
	{
		vector<Doors> allDoors = UnlockedDoors;
		for (Doors _door : LockedDoors)
		{
			if (!(count(UnlockedDoors.begin(), UnlockedDoors.end(), _door)))
				allDoors.push_back(_door);
		}
		return allDoors;
	}

	string UnlockDoor(Doors door)
	{
		auto match = find(LockedDoors.begin(), LockedDoors.end(), door);

		if (match != LockedDoors.end())
		{
			LockedDoors.erase(match);
			UnlockedDoors.push_back(door);
			return "Unlocked the " + DoorToString(door) + " door.";
		}

		return "That door is not locked.";
	}

	/// <summary></summary>
	/// <param name="foundItem">If an item was found it will be used here.</param>
	/// <returns>true if the item was successfully found.</returns>
	bool RemoveItem(InventorySlot& foundItem, string itemName, int itemCount=100)
	{
		bool itemFound = false;
		for (int i = 0; i < Items.size(); i++)
		{
			InventorySlot& _item = Items[i];

			if (itemName == ToLower(_item.GetName()) || 
				itemName == ToLower(_item.GetName()) + "s" || 
				itemName == ToLower(_item.GetName()) + "es")
			{
				
				if (itemCount > _item.NumberInStack)
					itemCount = _item.NumberInStack;

				_item.NumberInStack -= itemCount;

				switch (_item.GetItemType())
				{
				case TItem:
					foundItem = InventorySlot(_item.GetItemAsItem(), itemCount);
					itemFound = true;
					break;
				case TMelee:
					foundItem = InventorySlot(_item.GetItemAsMelee(), itemCount);
					itemFound = true;
					break;
				case TRanged:
					foundItem = InventorySlot(_item.GetItemAsRanged(), itemCount);
					itemFound = true;
					break;
				}

				// remove item if it was the last one
				if (_item.NumberInStack <= 0)
					Items.erase(Items.begin() + i);

				return itemFound;
			}
		}

		return false;
	}

	string PrintItems()
	{
		string description = "This room contains ";

		string name;
		int count;
		//for (InventorySlot _item : Items)

		if (Items.size() == 0) // no items
			description = "This room is empty.";
		else if (Items.size() == 1) // only one item
		{
			name = Items[0].GetName();
			if (Items[0].NumberInStack == 1)
			{
				description += WhenToUseAn(name) + ".";
			}
			else
				description += to_string(Items[0].NumberInStack) + " " + name + "s.";
		}
		else
		{
			for (int i = 0; i < Items.size(); i++)
			{
				InventorySlot _item = Items[i];
				name = _item.GetName();
				count = _item.NumberInStack;

				if (i != Items.size() - 1)
				{
					if (_item.NumberInStack == 1)
					{
						description += WhenToUseAn(name) + ", ";
					}
					else
						description += to_string(count) + " " + name + "s, ";
				}
				else // only for the last item in the list
				{
					description.pop_back();
					description.pop_back();

					description += " and " + WhenToUseAn(name) + ".";
				}
			}
		}

		return description;
		//Print(description);
	}

	string PrintDoors()
	{
		string message = "There are doors on the";
		vector<Doors> allDoors = GetAllDoors();
		vector<Doors> lockedDoors = GetLockedDoors();

		if (allDoors.size() == 0)
			return "There are no doors in this room.";

		for (int i = 0; i < allDoors.size(); i++)
		{
			Doors _door = allDoors[i];
			switch (_door)
			{
			case North:
				message += " north,";
				break;
			case East:
				message += " east,";
				break;
			case South:
				message += " south,";
				break;
			case West:
				message += " west,";
				break;
			}

			// on second to last door
			if (i == allDoors.size()-2)
			{
				message.pop_back();

				message += " and";
			}
			// only last door
			else if (i == allDoors.size() - 1)
			{
				message.pop_back();
				// if multiple doors
				if (allDoors.size() > 1)
					message += " walls.";
				else
				{
					message += " wall.";
					message = regex_replace(message, regex("are doors"), "is a door");
				}
			}
		}
		// get locked doors
		if (lockedDoors.size() > 0)
		{
			message.pop_back();
			message += ",\nbut there's a lock on the";

			for (int i = 0; i < lockedDoors.size(); i++)
			{
				Doors _door = lockedDoors[i];
				switch (_door)
				{
				case North:
					message += " north,";
					break;
				case East:
					message += " east,";
					break;
				case South:
					message += " south,";
					break;
				case West:
					message += " west,";
					break;
				}

				// on second to last door
				if (i == lockedDoors.size() - 2)
				{
					message.pop_back();
					message += " and";
				}
				// only last door
				else if (i == lockedDoors.size() - 1)
				{
					message.pop_back();
					// if multiple doors
					if (lockedDoors.size() > 1)
						message += " doors.";
					else
					{
						message += " door.";
					}
				}
			}
		}

		return message;
	}

	void GetItemCounts(int& items, int& weapons)
	{
		items = 0;
		weapons = 0;
		if (Items.size() == 0) // no items
			return;
		
		for (InventorySlot item : Items)
		{
			if (item.GetItemType() == TItem)
				items++;
			else if (item.GetItemType() == TMelee || item.GetItemType() == TRanged)
				weapons++;
		}
	}

	/// @brief Returns a 5 long array of text to display the room on a map.
	string* PrintRoom(char _item, char _weapon, char _player)
	{
		// detect if this is the dummy room
		if (/*!playerVisited ||*/ GetAllDoors().size() == 0)
		{
			string* temp = new string[5];
			temp[0] = "           "; // 11
			temp[1] = "           ";
			temp[2] = "           ";
			temp[3] = "           ";
			temp[4] = "           ";
			return temp;
		}


		string* lines = new string[5];
		// walls
		char w_hor = char(205); // ═
		char w_ver = char(186); // ║

		// corners
		char tl = char(201); // ╔
		char tr = char(187); // ╗
		char br = char(188); // ╝
		char bl = char(200); // ╚

		lines[0] = tl + string(9, w_hor) + tr;
		lines[1] = w_ver + string(9, ' ') + w_ver;
		lines[2] = w_ver + string(9, ' ') + w_ver;
		lines[3] = w_ver + string(9, ' ') + w_ver;
		lines[4] = bl + string(9, w_hor) + br;

		// doors
		char* d_top = &lines[0][5]; // ═
		char* d_rig = &lines[2][10]; // ║
		char* d_bot = &lines[4][5]; // ═
		char* d_lef = &lines[2][0]; // ║

		for (Doors unlocked : UnlockedDoors)
		{
			switch (unlocked)
			{
			case North:
				*d_top = ' ';
				break;
			case East:
				*d_rig = ' ';
				break;
			case South:
				*d_bot = ' ';
				break;
			case West:
				*d_lef = ' ';
				break;
			}
		}

		for (Doors locked : LockedDoors)
		{
			switch (locked)
			{
			case North:
				*d_top = 'x';
				break;
			case East:
				*d_rig = 'X';
				break;
			case South:
				*d_bot = 'x';
				break;
			case West:
				*d_lef = 'X';
				break;
			}
		}


		// add extras to room
		char* icon_player = &lines[2][5];
		char* icon_weapon = &lines[1][7];
		char* icon_item   = &lines[1][2];
		if (isPlayerIn)
			*icon_player = _player;

		int itemCount, weaponCount;
		GetItemCounts(itemCount, weaponCount);

		if (itemCount > 0)
			*icon_item = _item;
		if (weaponCount > 0)
			*icon_weapon = _weapon;

		return lines;

		/*
		☺☻
		±
		*/
	}
};



#endif
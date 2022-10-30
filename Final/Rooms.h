 #pragma once
#ifndef ROOMS_H
#define ROOMS_H

#include <regex>
#include "inventory.h"
#include "Items.h"
using namespace std;

enum Doors {North, East, South, West};

static unsigned int RoomIDIterator;
class Room
{
private:
	vector<InventorySlot> Items;
	vector<Doors> UnlockedDoors;
	vector<Doors> LockedDoors;
	unsigned int RoomID;

public:
	Room(vector<Doors> unlockedDoors, vector<Doors> lockedDoors = vector<Doors>())
		:Room(vector<InventorySlot>(), unlockedDoors, lockedDoors) {}

	Room(vector<InventorySlot> items = vector<InventorySlot>(),
		 vector<Doors> unlockedDoors = vector<Doors>(),
		 vector<Doors> lockedDoors = vector<Doors>())
	{
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

	vector<Doors> &GetUnlockedDoors()
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
};



#endif
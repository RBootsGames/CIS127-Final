 #pragma once
#ifndef ROOMS_H
#define ROOMS_H

#include "inventory.h"
#include "Items.h"
using namespace std;

class Room
{
private:
	vector<InventorySlot> Items;

public:
	Room() {}
	Room(InventorySlot items[], int size)
	{
		for (int i = 0; i < size; i++)
		{
			Items.push_back(items[i]);
		}
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


	/// <summary></summary>
	/// <param name="foundItem">If an item was found it will be used here.</param>
	/// <returns>true if the item was successfully found.</returns>
	bool RemoveItem(InventorySlot& foundItem, string itemName, int itemCount=1)
	{
		bool itemFound = false;
		for (int i = 0; i < Items.size(); i++)
		{
			InventorySlot& _item = Items[i];

			if (itemName == ToLower(_item.GetName()))
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

	void PrintItems()
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

		Print(description);
	}
};



#endif
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "inventory.h"
#include "GameDatabase.h"
#include "extensions.h"


class PlayerInventory
{
public:
	vector<InventorySlot> Slots;

	PlayerInventory() {}

	void PrintInventory()
	{
		/* Style
		┌──────────────────────────────┐
		│____________Items_____________│
		│	      Name       │  Count  │
		├────────────────────┼─────────┤
		│  Health Potion     │  10/10  │
		└────────────────────┴─────────┘
		*/

		// spacing
		int spaceName = 23;
		int spaceQuantity = 11;

		// top bar
		Print(char(201) + string(spaceName + spaceQuantity + 1, char(205)) + char(187));
		Print(char(186), false);
		Print(cjust("Items", spaceName + spaceQuantity + 1), false, true);
		Print(char(186), true);

		Print(char(186), false);
		Print(cjust("Name", spaceName) + char(179) + cjust("Count", spaceQuantity), false, true);
		Print(char(186));

		Print(char(195), false);
		Print(string(spaceName, char(196)) + char(197) + string(spaceQuantity, char(196)), false);
		Print(char(180));

		for (InventorySlot _slot : Slots)
		{
			if (_slot.GetItemType() == TItem)
				Print(_slot.PrintInfoItem(spaceName, spaceQuantity));
		}
		// bottom bar
		Print(char(192) + string(spaceName, char(196)) + char(193) + string(spaceQuantity, char(196)) + char(217));

		/*
		┌─────────────────────────────────────────────────────────┐
		│_______________________Weapons___________________________│
		│	 Type	   │		Name		  │ Damage │   Range  │
		├──────────────┼──────────────────────┼────────┼──────────┤
		│    Melee     │     Long Sword       │   15   │    3     │
		└──────────────┴──────────────────────┴────────┴──────────┘
		*/

		int spaceType = 12;
		int spaceDamage = 10;
		int spaceRange = 8;

		// top bar
		Print(char(201) + string(spaceType + spaceName + spaceDamage + spaceRange + 3, char(205)) + char(187));
		Print(char(186), false);
		Print(cjust("Weapons", spaceType + spaceName + spaceDamage + spaceRange + 3), false, true);
		Print(char(186), true);


		Print(char(186), false);
		Print(cjust("Type", spaceType), false, true);
		Print(char(179) + cjust("Name", spaceName), false, true);
		Print(char(179) + cjust("Damage", spaceDamage), false, true);
		Print(char(179) + cjust("Range", spaceRange), false, true);
		Print(char(186));


		Print(char(195), false);
		Print(string(spaceType, char(196)), false);
		Print(char(197) + string(spaceName, char(196)), false);
		Print(char(197) + string(spaceDamage, char(196)), false);
		Print(char(197) + string(spaceRange, char(196)), false);
		Print(char(180));


		for (InventorySlot _slot : Slots)
		{
			if (_slot.GetItemType() == TMelee || _slot.GetItemType() == TRanged)
				Print(_slot.PrintInfoWeapon(spaceType, spaceName, spaceDamage, spaceRange));
		}


		// bottom bar
		Print(char(192) + string(spaceType, char(196)) + char(193) + string(spaceName, char(196)) +
			char(193) + string(spaceDamage, char(196)) + char(193) + string(spaceRange, char(196)) + char(217));
		Print();
	}

	/// <summary></summary>
	/// <returns>number of items left behind</returns>
	int AddItem(Item& item, int count = 1, bool supressOutput = false)
	{
		// check for existing items
		bool isNewItem = true;
		for (InventorySlot& _slot : Slots)
		{
			if (item.GetName() == _slot.GetName())
			{
				if (_slot.NumberInStack == item.StackLimit)
				{
					if (!supressOutput)
						Print("You can't carry any more of these.");
					return count;
				}

				_slot.NumberInStack += count;
				if (_slot.NumberInStack > item.StackLimit) // more than you can carry
				{
					int tooMany = _slot.NumberInStack - item.StackLimit;
					_slot.NumberInStack = item.StackLimit;

					if (!supressOutput)
					{
						Print("You picked up " + to_string((count - tooMany)) + " ", false);
						if ((count - tooMany) == 1)
							Print(item.GetName(), false, true);
						else
							Print(item.GetName() + "s", false, true);

						Print(" and left " + to_string(tooMany) + " behind.");
					}
					return tooMany;
				}
				else // picked up all items
				{
					isNewItem = false;
					break;
				}
			}
		}

		if (!supressOutput)
		{
			Print("You picked up " + to_string(count) + " ", false);
			if (count == 1)
				Print(item.GetName(), false, true);
			else
				Print(item.GetName() + "s", false, true);
			Print(".");
		}

		// create inventory slot for new items
		if (isNewItem)
			Slots.push_back(InventorySlot(item, count));

		return 0;
	}
	int AddItem(MeleeWeapon& item, int count = 1, bool supressOutput = false)
	{
		// can't carry duplicate weapons
		for (InventorySlot& _slot : Slots)
		{
			if (item.GetName() == _slot.GetName())
			{
				if (!supressOutput)
					Print("You can't carry any more of these.");
				return count;
			}
		}

		Slots.push_back(InventorySlot(item, count));
		if (!supressOutput)
		{
			Print("You picked up ", false);
			Print(item.GetName(), false, true);
			Print(".");
		}
		return 0;
	}
	int AddItem(RangedWeapon& item, int count = 1, bool supressOutput = false)
	{
		// can't carry duplicate weapons
		for (InventorySlot& _slot : Slots)
		{
			if (item.GetName() == _slot.GetName())
			{
				if (!supressOutput)
					Print("You can't carry any more of these.");
				return count;
			}
		}

		Slots.push_back(InventorySlot(item, count));
		if (!supressOutput)
		{
			Print("You picked up ", false);
			Print(item.GetName(), true, true);
			Print(".");
		}
		return 0;
	}
	int AddItem(InventorySlot& item)
	{
		switch (item.GetItemType())
		{
		case TItem:
			return AddItem(item.GetItemAsItem(), item.NumberInStack);
		case TMelee:
			return AddItem(item.GetItemAsMelee(), item.NumberInStack);
		case TRanged:
			return AddItem(item.GetItemAsRanged(), item.NumberInStack);
		}
	}

	void EquipItem(string itemName, bool supressOutput = false)
	{
		// get item type first
		itemName = ToLower(itemName);
		TItemType itemType = None;

		for (InventorySlot& _slot : Slots)
		{
			if (itemName == ToLower(_slot.GetName()))
			{
				itemType = _slot.GetItemType();
				itemName = _slot.GetName();
				_slot.IsEquiped = true;
				if (!supressOutput)
				{
					Print("You equipped ", false);
					Print(itemName, false, true);
					Print(".");
				}
				break;
			}
		}

		// unequip all like-items
		for (InventorySlot& _slot : Slots)
		{
			if (itemName == _slot.GetName())
				continue;
			else if (_slot.GetItemType() == itemType)
				_slot.IsEquiped = false;
		}
	}
};


class Player
{
public:
	Room CurrentRoom;
	PlayerInventory Inventory;

	Player()
	{
		Inventory = PlayerInventory();
		Inventory.AddItem(melee_Unarmed, 1, true);
	}

	void EnterRoom(Room room)
	{
		CurrentRoom = room;
	}

	Room GetRoom() { return CurrentRoom; }
};


#endif // !PLAYER_H

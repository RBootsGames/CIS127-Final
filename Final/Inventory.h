#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>
#include "Items.h"
#include "extensions.h"
#include "objects.h"

enum TItemType
{
	None, TItem, TMelee, TRanged
};

class InventorySlot
{
private:
	//Item basicNullable;
	//MeleeWeapon meleeNullable;
	//RangedWeapon rangedNullable;

	Item* itemBasic;
	MeleeWeapon* itemMelee;
	RangedWeapon* itemRanged;
	TItemType ItemType;


public:
	int NumberInStack;
	bool IsEquiped;

	// Constructors
	InventorySlot() 
	{
		itemBasic = NULL;
		itemMelee = NULL;
		itemRanged = NULL;
		ItemType = None;
		IsEquiped = false;
	}
	InventorySlot(Item &item, int itemCount=1) : InventorySlot()
	{
		itemBasic = &item;
		NumberInStack = itemCount;
		ItemType = TItem;
	}
	InventorySlot(MeleeWeapon &item, int itemCount=1) : InventorySlot()
	{
		itemMelee = &item;
		NumberInStack = itemCount;
		ItemType = TMelee;
	}
	InventorySlot(RangedWeapon &item, int itemCount=1) : InventorySlot()
	{
		itemRanged = &item;
		NumberInStack = itemCount;
		ItemType = TRanged;
	}
	// End Constructors

	string GetName()
	{
		if (itemBasic != NULL)
			return itemBasic->GetName();
		else if (itemMelee != NULL)
			return itemMelee->GetName();
		else if (itemRanged != NULL)
			return itemRanged->GetName();
		else
			return "";
	}
	TItemType GetItemType() { return ItemType; }

	Item& GetItemAsItem() { return *itemBasic; }
	MeleeWeapon& GetItemAsMelee() { return *itemMelee; }
	RangedWeapon& GetItemAsRanged() { return *itemRanged; }

	int GetDamage()
	{
		if (ItemType == TMelee)
			return itemMelee->Damage;
		else if (ItemType == TRanged)
			return itemRanged->Damage;
		else
			return 0;
	}

	int GetRange()
	{
		if (ItemType == TMelee)
			return itemMelee->Range;
		else if (ItemType == TRanged)
			return itemRanged->Range;
		else
			return 0;
	}

	string PrintInfoItem(int spaceName, int spaceQuantity)
	{
		if (ItemType == TItem)
		{
			// Trying to figure out how to print item info differently from weapon info.
			string line = "";
			itemBasic->PrintInfo(line, TableRow(GetName(), spaceName),
									   TableRow(to_string(NumberInStack) + "/" + to_string(itemBasic->StackLimit), spaceQuantity));
			return line + char(179);
		}
		return "";
	}

	string PrintInfoWeapon(int spaceType, int spaceName, int spaceDamage, int spaceRange)
	{
		string line = "";
		string name = GetName();

		if (IsEquiped)
			name = "*" + name + "*";

		if (ItemType == TMelee)
		{
			itemMelee->PrintInfo(line,	TableRow("Melee", spaceType, IsEquiped),
										TableRow(name, spaceName, IsEquiped),
										TableRow(to_string(GetDamage()), spaceDamage, IsEquiped),
										TableRow(to_string(GetRange()), spaceRange, IsEquiped));

			line += char(179);
		}
		else if (ItemType == TRanged)
		{
			itemMelee->PrintInfo(line,  TableRow("Range", spaceType, IsEquiped),
										TableRow(name, spaceName, IsEquiped),
										TableRow(to_string(GetDamage()), spaceDamage, IsEquiped),
										TableRow(to_string(GetRange()), spaceRange, IsEquiped));

			line += char(179);
		}

		return line;
	}
};

#endif // !INVENTORY_H

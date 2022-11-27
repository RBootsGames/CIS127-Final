#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include "Items.h"

class InventorySlot
{
private:
	Item* itemBasic;
	MeleeWeapon* itemMelee;
	RangedWeapon* itemRanged;
	TItemType ItemType;


public:
	int NumberInStack;
	bool IsEquiped;

	json Serialize();
	/// <summary>
	/// This is incomplete.
	/// </summary>
	static InventorySlot* Deserialize(json data);

	// Constructors
	InventorySlot();
	InventorySlot(Item& item, int itemCount = 1);
	InventorySlot(MeleeWeapon& item, int itemCount = 1);
	InventorySlot(RangedWeapon& item, int itemCount = 1);
	// End Constructors

	std::string GetName();
	bool GetConsumable();
	TItemType GetItemType();

	Item& GetItemAsItem();
	MeleeWeapon& GetItemAsMelee();
	RangedWeapon& GetItemAsRanged();

	int GetDamage();

	int GetRange();

	std::string PrintInfoItem(int spaceName, int spaceQuantity);

	std::string PrintInfoWeapon(int spaceType, int spaceName, int spaceDamage, int spaceRange);
};

#endif // !INVENTORY_H

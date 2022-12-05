#include "inventory.h"

#include "nlohmann/json.hpp"
#include <vector>
#include <string>
#include "Items.h"
#include "extensions.h"
#include "objects.h"
#include "GameDatabase.h"

using json = nlohmann::json;

json InventorySlot::Serialize()
{
	json j;

	j["NumberInStack"] = NumberInStack;
	j["IsEquiped"] = IsEquiped;
	j["ItemType"] = GetItemType();


	if (itemBasic != nullptr)
	{
		json temp = itemBasic->Serialize();
		j.insert(temp.begin(), temp.end());
	}
	else if (itemMelee != nullptr)
	{
		json temp = itemMelee->Serialize();
		j.insert(temp.begin(), temp.end());
	}
	else if (itemRanged != nullptr)
	{
		json temp = itemRanged->Serialize();
		j.insert(temp.begin(), temp.end());
	}

	return j;
}
InventorySlot* InventorySlot::Deserialize(json data)
{
	InventorySlot* is = new InventorySlot();
	is->NumberInStack = data["NumberInStack"];
	is->IsEquiped = data["IsEquiped"];

	TItemType itemType = data["ItemType"];
	is->ItemType = itemType;

	switch (itemType)
	{
	case TItem:
		is->itemBasic = Item::GetItemByName(data["Name"]);
		break;
	case TMelee:
		is->itemMelee = MeleeWeapon::GetItemByName(data["Name"]);
		break;
	case TRanged:
		is->itemRanged = RangedWeapon::GetItemByName(data["Name"]);
		break;
	}


	return is;
}

// Constructors
InventorySlot::InventorySlot()
{
	itemBasic = nullptr;
	itemMelee = nullptr;
	itemRanged = nullptr;
	ItemType = None;
	IsEquiped = false;
	NumberInStack = 1;
}
InventorySlot::InventorySlot(Item& item, int itemCount) : InventorySlot()
{
	itemBasic = &item;
	NumberInStack = itemCount;
	ItemType = TItem;
}
InventorySlot::InventorySlot(MeleeWeapon& item, int itemCount) : InventorySlot()
{
	itemMelee = &item;
	NumberInStack = itemCount;
	ItemType = TMelee;
}
InventorySlot::InventorySlot(RangedWeapon& item, int itemCount) : InventorySlot()
{
	itemRanged = &item;
	NumberInStack = itemCount;
	ItemType = TRanged;
}
// End Constructors

std::string InventorySlot::GetName()
{
	if (itemBasic != nullptr)
		return itemBasic->GetName();
	else if (itemMelee != nullptr)
		return itemMelee->GetName();
	else if (itemRanged != nullptr)
		return itemRanged->GetName();
	else
		return "";
}
bool InventorySlot::GetConsumable()
{
	if (itemBasic != nullptr)
		return itemBasic->GetConsumable();
	else if (itemMelee != nullptr)
		return itemMelee->GetConsumable();
	else if (itemRanged != nullptr)
		return itemRanged->GetConsumable();
	else
		return false;
}
TItemType InventorySlot::GetItemType() { return ItemType; }


Item& InventorySlot::GetItemAsItem() { return *itemBasic; }
MeleeWeapon& InventorySlot::GetItemAsMelee() { return *itemMelee; }
RangedWeapon& InventorySlot::GetItemAsRanged() { return *itemRanged; }

int InventorySlot::GetDamage()
{
	if (ItemType == TMelee)
		return itemMelee->Damage;
	else if (ItemType == TRanged)
		return itemRanged->Damage;
	else
		return 0;
}

int InventorySlot::GetRange()
{
	if (ItemType == TMelee)
		return itemMelee->Range;
	else if (ItemType == TRanged)
		return itemRanged->Range;
	else
		return 0;
}

string InventorySlot::PrintInfoItem(int spaceName, int spaceQuantity)
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

string InventorySlot::PrintInfoWeapon(int spaceType, int spaceName, int spaceDamage, int spaceRange)
	{
		string line = "";
		string name = GetName();

		if (IsEquiped)
			name = "*" + name + "*";

		if (ItemType == TMelee)
		{
			itemMelee->PrintInfo(line, TableRow("Melee", spaceType, IsEquiped),
				TableRow(name, spaceName, IsEquiped),
				TableRow(to_string(GetDamage()), spaceDamage, IsEquiped),
				TableRow(to_string(GetRange()), spaceRange, IsEquiped));

			line += char(179);
		}
		else if (ItemType == TRanged)
		{
			itemMelee->PrintInfo(line, TableRow("Range", spaceType, IsEquiped),
				TableRow(name, spaceName, IsEquiped),
				TableRow(to_string(GetDamage()), spaceDamage, IsEquiped),
				TableRow(to_string(GetRange()), spaceRange, IsEquiped));

			line += char(179);
		}

		return line;
	}

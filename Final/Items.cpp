#include "Items.h"

#include "extensions.h"
#include "objects.h"
#include "nlohmann/json.hpp"
#include "GameDatabase.h"
//#include <string>

#include <iostream>

using json = nlohmann::json;

json Item::Serialize()
{
	json j;
	j["Name"] = Name;
	j["IsConsumable"] = IsConsumable;

	return j;
}

Item::Item()
{
	IsConsumable = false;
	StackLimit = 1;
}
Item::Item(std::string name, int stackLimit, bool isConsumable)
{
	Name = name;
	StackLimit = stackLimit;
	IsConsumable = isConsumable;
}
Item::Item(std::string name)
{
	Name = name;
}

std::string Item::GetName() { return Name; }
bool Item::GetConsumable() { return IsConsumable; }

//void Item::PrintInfo(std::string& output, TableRow row)
//{
//	output += PrintFormatting(row);
//}
//
//template <typename... extend>
//void Item::PrintInfo(std::string& output, TableRow row, extend... params)
//{
//	output += PrintFormatting(row);
//	PrintInfo(output, params...);
//}
std::string Item::PrintFormatting(TableRow tableRow)
{
	char filler = ' ';
	return char(179) + cjust(tableRow.Text, tableRow.Spacing, filler);
}


Item* Item::GetItemByName(std::string name)
{
	Item* match = nullptr;
	for (int i = 0; i < itemsArraySize; i++)
	{
		Item* _item = itemsArray[i];
		if (name == _item->GetName())
		{
			match = _item;
			break;
		}
	}
	return match;
}


json MeleeWeapon::Serialize()
{
	json j = Item::Serialize();
	j["Damage"] = Damage;
	j["Range"] = Range;

	return j;
}

MeleeWeapon::MeleeWeapon()
{
	Damage = 0;
	Range = 0;
}
MeleeWeapon::MeleeWeapon(std::string name, int damage, int range)
	:Item(name)
{
	StackLimit = 1;
	Damage = damage;
	Range = range;
}
MeleeWeapon* MeleeWeapon::GetItemByName(std::string name)
{
	MeleeWeapon* match = nullptr;
	for (int i = 0; i < meleeArraySize; i++)
	{
		MeleeWeapon* _item = meleeArray[i];
		if (name == _item->GetName())
		{
			match = _item;
			break;
		}
	}
	return match;
}

RangedWeapon::RangedWeapon(std::string name, int damage, int range)
	:MeleeWeapon(name, damage, range)
{

}
RangedWeapon* RangedWeapon::GetItemByName(string name)
{
	RangedWeapon* match = nullptr;
	for (int i = 0; i < rangedArraySize; i++)
	{
		RangedWeapon* _item = rangedArray[i];
		if (name == _item->GetName())
		{
			match = _item;
			break;
		}
	}
	return match;
}


//RangedWeapon* RangedWeapon::GetItemByName(string name)
//{
//	for (int i = 0; i < sizeof(rangedArray) / sizeof(RangedWeapon); i++)
//	{
//		RangedWeapon* _item = rangedArray[i];
//		if (name == _item->GetName())
//			return _item;
//	}
//	return nullptr;
//}

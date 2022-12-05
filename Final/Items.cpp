#include "Items.h"

#include <string>
#include "extensions.h"
#include "objects.h"
#include "nlohmann/json.hpp"
#include "GameDatabase.h"
#include <regex>

#include <iostream>

using json = nlohmann::json;

json Item::Serialize()
{
	json j;
	j["Name"] = Name;
	//j["IsConsumable"] = IsConsumable;

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



MeleeWeapon::MeleeWeapon()
{
	Damage = 0;
	Range = 0;
}
MeleeWeapon::MeleeWeapon(std::string name, int damage, std::vector<AttackMove> attacks, int range)
	:Item(name)
{
	Attacks = attacks;
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

std::vector<std::string> MeleeWeapon::PrintAttackInfo()
{
	std::vector<std::string> info;
	int widthName = 18;
	int widthDamage = 13;
	int widthStam = 17;
	for (auto attack : Attacks)
	{
		std::string line = ljust(attack.AttackName, widthName) + ' ';
		line += ljust("Damage: " + to_string(Damage + attack.DamageAddition), widthDamage);
		line += ljust("Stamina cost: " + to_string(attack.StaminaConsumption), widthStam);
		line += "(" + Name + ") ";
		if (attack.Description != "")
			line += "[" + attack.Description + "] ";
		info.push_back(line);
	}

	return info;
}

//RangedWeapon::RangedWeapon(std::string name, int damage, int range)
//	:MeleeWeapon(name, damage, range)
//{
//
//}
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

//std::vector<std::string> RangedWeapon::PrintAttackInfo()
//{
//	std::vector<std::string> info = MeleeWeapon::PrintAttackInfo();
//	for (auto line : info)
//	{
//		line = std::regex_replace(line, std::regex("(Melee)"), "(Ranged)");
//	}
//	return info;
//}
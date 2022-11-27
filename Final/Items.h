#pragma once
#ifndef ITEMS_H
#define ITEMS_H

//#include <string>
//#include "extensions.h"
#include "objects.h"
#include "nlohmann/json.hpp"

//#include "GameDatabase.h"

using json = nlohmann::json;
//using namespace std;

class Item
{
protected:
	std::string Name;
	bool IsConsumable;

public:
	int StackLimit;

	json Serialize();

	Item();
	Item(std::string name, int stackLimit, bool isConsumable);
	Item(std::string name);

	static Item* GetItemByName(std::string name);

	std::string GetName();
	bool GetConsumable();

	void PrintInfo(std::string& output, TableRow row)
	{
		output += PrintFormatting(row);
	}

	template <typename... extend>
	void PrintInfo(std::string& output, TableRow row, extend... params)
	{
		output += PrintFormatting(row);
		PrintInfo(output, params...);
	}
protected:
	std::string PrintFormatting(TableRow tableRow);
};


class MeleeWeapon : public Item
{
public:
	int Damage;
	int Range;

	json Serialize();

	MeleeWeapon();
	MeleeWeapon(std::string name, int damage, int range = 1);

	static MeleeWeapon* GetItemByName(std::string name);
};

class RangedWeapon : public MeleeWeapon
{
public:
	RangedWeapon(std::string name, int damage, int range = 1);

	static RangedWeapon* GetItemByName(std::string name);

};






#endif

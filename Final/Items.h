#pragma once
#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include "extensions.h"
#include "objects.h"
using namespace std;

class Item
{
protected:
	string Name;
	//bool IsStackable;
	//int CurrentStack;
	bool IsConsumable;
	//bool HasDurability;
	//int CurrentDurability;

public:
	int StackLimit;

	Item() {  }
	Item(string name, int stackLimit, bool isConsumable)
	{
		Name = name;
		//IsStackable = isStackable;
		StackLimit = stackLimit;
		IsConsumable = isConsumable;
	}
	Item(string name)
	{
		Name = name;
	}

	string GetName() { return Name; }

	void PrintInfo(string &output, TableRow row)
	{
		output += PrintFormatting(row);
	}

	template <typename... extend>
	void PrintInfo(string &output, TableRow row, extend... params)
	{
		output += PrintFormatting(row);
		PrintInfo(output, params...);
	}
protected:
	string PrintFormatting(TableRow tableRow)
	{
		char filler = ' ';
		return char(179) + cjust(tableRow.Text, tableRow.Spacing, filler);
	}
};


class MeleeWeapon : public Item
{
public:
	int Damage;
	int Range;

	MeleeWeapon() 
	{
		Damage = 0;
		Range = 0;
	}
	MeleeWeapon(string name, int damage, int range=1)
		:Item(name)
	{
		StackLimit = 1;
		Damage = damage;
		Range = range;
	}

//	string PrintInfo(int spaceType, int spaceName)
//	{
//		return "placeholder";
//		//return PrintFormatting("Melee", spaceType, spaceName);
//		//string temp = ljust("Melee", spaceType);
//		//temp += ljust(Name, spaceName);
//		//return temp;
//	}
};

class RangedWeapon : public MeleeWeapon
{
//public:
//	int Damage;
//	int Range;
//
//	RangedWeapon()
//	{
//		Damage = 0;
//		Range = 0;
//	}
//	RangedWeapon(string name, int damage, int range = 1)
//		:Item(name)
//	{
//		StackLimit = 1;
//		Damage = damage;
//		Range = range;
//	}
//	string PrintInfo(int spaceType, int spaceName)
//	{
//		return "placeholder";
//		//return PrintFormatting("Ranged", spaceType, spaceName);
//	}
};






#endif

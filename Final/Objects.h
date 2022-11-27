#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
//#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
//using namespace std;

enum TItemType
{
	None, TItem, TMelee, TRanged
};

enum StatusEffect
{
	SE_None, SE_WeakPoison, SE_StrongPoison, SE_WeakBleed, SE_StrongBleed,
	SE_WeakRegen, SE_StrongRegen, SE_Strength
};


struct Vector2
{
	int X, Y;

	json Serialize();

	static Vector2* Deserialize(json data);

	Vector2();
	Vector2(int x, int y);

	bool operator==(const Vector2& compare) const;

	Vector2 operator+(const Vector2& add) const;
};

struct TableRow
{
	std::string Text;
	int Spacing;
	bool IsEquiped;

	TableRow(std::string text, int spacing, bool isEquiped = false);
};

struct AttackMove
{
	std::string AttackName;
	std::string Description;
	int StaminaConsumption;
	int DamageAddition;
	int RangeAddition;
	StatusEffect StatusEffects;
	float StatusProbability;
};
#endif

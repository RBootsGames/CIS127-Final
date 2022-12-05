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
	SE_WeakRegen, SE_StrongRegen, SE_Strength, SE_Weakness, SE_StanceChanged
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

struct StatusEffectTracker
{
	StatusEffect Effect;
	int TurnsRemaining;

	json Serialize();

	static StatusEffectTracker* Deserialize(json data);

	StatusEffectTracker(StatusEffect effect, int turnCount);
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
	int StatusDuration;

	json Serialize();

	static AttackMove* Deserialize(json data);

	AttackMove();
	AttackMove(std::string name, int stamina, int extraDamage = 0,
		int extraRange = 0, StatusEffect effect = SE_None, float probability = 1, int turnDuration = 1);
	/// @brief Includes description.
	AttackMove(std::string name, std::string description, int stamina, int extraDamage = 0,
		int extraRange = 0, StatusEffect effect = SE_None, float probability = 1, int turnDuration = 1);
};


class MeleeWeapon;
class RangedWeapon;
class Enemy;

struct AttackQueue
{
	MeleeWeapon* melee;
	RangedWeapon* ranged;
	int attackIndex;
	Enemy& targetEnemy;

	AttackQueue(MeleeWeapon* weapon, int attackIndex, Enemy& enemy);
	AttackQueue(RangedWeapon* weapon, int attackIndex, Enemy& enemy);
};
#endif
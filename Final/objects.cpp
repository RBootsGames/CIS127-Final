#include "objects.h"

#include <string>
#include "Items.h"
#include "enemy.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
//using namespace std;

//enum TItemType
//{
//	None, TItem, TMelee, TRanged
//};



json Vector2::Serialize()
{
	json j;
	j["X"] = X;
	j["Y"] = Y;
	return j;
}

Vector2* Vector2::Deserialize(json data)
{
	return new Vector2(data["X"], data["Y"]);
}

Vector2::Vector2()
{
	X = 0;
	Y = 0;
}
Vector2::Vector2(int x, int y)
{
	X = x;
	Y = y;
}

bool Vector2::operator==(const Vector2& compare) const
{
	return (X == compare.X) && (Y == compare.Y);
}

Vector2 Vector2::operator+(const Vector2& add) const
{
	return Vector2(X + add.X, Y + add.Y);
}


TableRow::TableRow(std::string text, int spacing, bool isEquiped)
{
	Text = text;
	Spacing = spacing;
	IsEquiped = isEquiped;
}

StatusEffectTracker::StatusEffectTracker(StatusEffect effect, int turnCount)
{
	Effect = effect;
	TurnsRemaining = turnCount;
}
json StatusEffectTracker::Serialize()
{
	json j;
	j["Effect"] = Effect;
	j["TurnsRemaining"] = TurnsRemaining;
	return j;
}
StatusEffectTracker* StatusEffectTracker::Deserialize(json data)
{
	return new StatusEffectTracker(data["Effect"], data["TurnsRemaining"]);
}

json AttackMove::Serialize()
{
	json j;
	j["AttackName"] = AttackName;
	j["Description"] = Description;
	j["StaminaConsumption"] = StaminaConsumption;
	j["DamageAddition"] = DamageAddition;
	j["RangeAddition"] = RangeAddition;
	j["StatusEffects"] = StatusEffects;
	j["StatusProbability"] = StatusProbability;
	j["StatusDuration"] = StatusDuration;
	return j;
}

AttackMove* AttackMove::Deserialize(json data)
{
	AttackMove* am = new AttackMove();
	am->AttackName = data["AttackName"];
	am->Description = data["Description"];
	am->StaminaConsumption = data["StaminaConsumption"];
	am->DamageAddition = data["DamageAddition"];
	am->RangeAddition = data["RangeAddition"];
	am->StatusEffects = data["StatusEffects"];
	am->StatusProbability = data["StatusProbability"];
	am->StatusDuration = data["StatusDuration"];
	return am;
}

AttackMove::AttackMove(){}
AttackMove::AttackMove(std::string name, int stamina, int extraDamage,
	int extraRange, StatusEffect effect, float probability, int turnDuration):
	AttackMove(name, "", stamina, extraDamage, extraRange, effect, probability, turnDuration){}


AttackMove::AttackMove(std::string name, std::string description, int stamina, int extraDamage,
	int extraRange, StatusEffect effect, float probability, int turnDuration)
{
	AttackName = name;
	Description = description;
	StaminaConsumption = stamina;
	DamageAddition = extraDamage;
	RangeAddition = extraRange;
	StatusEffects = effect;
	StatusProbability = probability;
	StatusDuration = turnDuration;
}



AttackQueue::AttackQueue(MeleeWeapon* weapon, int attackIndex, Enemy& enemy)
	:targetEnemy(enemy)
{
	melee = weapon;
	ranged = nullptr;
	this->attackIndex= attackIndex;
}
AttackQueue::AttackQueue(RangedWeapon* weapon, int attackIndex, Enemy& enemy)
	:targetEnemy(enemy)
{
	melee = nullptr;
	ranged = weapon;
	this->attackIndex = attackIndex;
}
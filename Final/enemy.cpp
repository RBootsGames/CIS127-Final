#include "enemy.h"

#include <vector>
#include <string>
#include <tuple>
#include <random>
#include "objects.h"
#include "inventory.h"
#include "extensions.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;


json Enemy::Serialize()
{
	json j;
	j["Name"] = Name;
	j["MaxHealth"] = MaxHealth;
	j["CurrentHealth"] = CurrentHealth;
	j["BaseDamage"] = BaseDamage;
	j["Armor"] = Armor;
	j["XPGranted"] = XPGranted;
	j["CanFly"] = CanFly;
	j["IsGrounded"] = IsGrounded;

	if (ItemDrop == nullptr)
		j["ItemDrop"] = "";
	else
		j["ItemDrop"] = ItemDrop->Serialize();

	for (StatusEffectTracker effect : StatusEffects)
		j["StatusEffects"].push_back(effect.Serialize());

	j["StatusResistant"] = StatusResistant;
	j["StatusWeakness"] = StatusWeakness;

	for (AttackMove move : AttackMoves)
		j["AttackMoves"].push_back(move.Serialize());

	return j;
}

Enemy* Enemy::Deserialize(json data)
{
	Enemy* en = new Enemy();
	en->Name = data["Name"];
	en->MaxHealth = data["MaxHealth"];
	en->CurrentHealth = data["CurrentHealth"];
	en->BaseDamage = data["BaseDamage"];
	en->Armor = data["Armor"];
	en->XPGranted = data["XPGranted"];
	en->CanFly = data["CanFly"];
	en->IsGrounded = data["IsGrounded"];

	if (data["ItemDrop"] == "")
		en->ItemDrop = nullptr;
	else
		en->ItemDrop = InventorySlot::Deserialize(data["ItemDrop"]);

	for (json effect : data["StatusEffects"])
		en->StatusEffects.push_back(*StatusEffectTracker::Deserialize(effect));


	for (StatusEffect eff : data["StatusResistant"])
		en->StatusResistant.push_back(eff);

	for (StatusEffect eff : data["StatusWeakness"])
		en->StatusWeakness.push_back(eff);

	for (json move : data["AttackMoves"])
		en->AttackMoves.push_back(*AttackMove::Deserialize(move));

	return en;
}

Enemy* Enemy::Duplicate(Enemy* master)
{
	Enemy* dupe = new Enemy();
	dupe->Name = master->Name;
	dupe->MaxHealth = master->MaxHealth;
	dupe->CurrentHealth = master->CurrentHealth;
	dupe->BaseDamage = master->BaseDamage;
	dupe->Armor = master->Armor;
	dupe->XPGranted = master->XPGranted;
	dupe->CanFly = master->CanFly;
	dupe->IsGrounded = master->IsGrounded;
	dupe->ItemDrop = master->ItemDrop;
	dupe->StatusEffects = master->StatusEffects;
	dupe->StatusResistant = master->StatusResistant;
	dupe->StatusWeakness = master->StatusWeakness;
	dupe->AttackMoves = master->AttackMoves;
	return dupe;
	//	master->Name,
	//	master->MaxHealth,
	//	master->BaseDamage,
	//	master->AttackMoves,
	//	master->XPGranted,
	//	master->Armor,
	//	master->CanFly,
	//	master->IsGrounded,
	//	master->ItemDrop,
	//	master->StatusResistant,
	//	master->StatusWeakness,
	//	master->StatusEffects
	//);
}


Enemy::Enemy()
{
	Name = "NA";
	MaxHealth = 1;
	CurrentHealth = 1;
	BaseDamage = 0;
	Armor = 0;
	XPGranted = 0;
	CanFly = false;
	IsGrounded = true;
	hasDied = false;
	ItemDrop = nullptr;
}

Enemy::Enemy(std::string name, int maxHealth, int baseDamage, std::vector<AttackMove> attacks, int xp,
	int armor, bool canFly, bool isGrounded, InventorySlot* itemDrop,
	std::vector<StatusEffect> resistance,
	std::vector<StatusEffect> weakness)
{
	Name = name;
	MaxHealth = maxHealth;
	CurrentHealth = MaxHealth;
	Armor = armor;
	BaseDamage = baseDamage;
	AttackMoves = attacks;
	XPGranted = xp;
	CanFly = canFly;
	IsGrounded = isGrounded;
	hasDied = false;
	ItemDrop = itemDrop;
	StatusResistant = resistance;
	StatusWeakness = weakness;
}
Enemy::Enemy(std::string name, int maxHealth, int baseDamage, std::vector<AttackMove> attacks, int xp,
	int armor, InventorySlot* itemDrop,
	std::vector<StatusEffect> resistance,
	std::vector<StatusEffect> weakness):
	Enemy(name, maxHealth, baseDamage, attacks, xp, armor, false, true, itemDrop, resistance, weakness){}

void Enemy::ChangeStance(bool forceChange)
{
	if (!CanFly)
		return;

	if (!HasStatusEffect(SE_StanceChanged) || forceChange)
	{
		IsGrounded = !IsGrounded;
		AddStatusEffect(SE_StanceChanged, 4);
		if (IsGrounded)
			Print(Name + " is now grounded.");
		else
			Print(Name + " is now flying.");
	}
}

void Enemy::Attack(Player& _player)
{
	if (HasDied())
		return;
	// decide attack
	int attackIndex = rand() % AttackMoves.size();
	AttackMove attack = AttackMoves[attackIndex];

	// calculate status effect chance
	bool applyEffect = false;
	if (attack.StatusEffects != SE_None)
	{
		float chance = (float)rand() / RAND_MAX;
		if (chance <= attack.StatusProbability)
			applyEffect = true;
	}

	// modify based on current status effect
	int attackDamage = BaseDamage + attack.DamageAddition;
	if (StatusEffects.size() > 0)
	{
		if (HasStatusEffect(SE_Weakness))
			attackDamage = round(attackDamage * .85f);
		else if (HasStatusEffect(SE_Strength))
			attackDamage = round(attackDamage * 1.15f);
	}

	// modify based on player armor
	attackDamage -= _player.GetArmor();

	// do attack
	int playerNewHealth = _player.ModifyHealth(-attackDamage);
	Print(Name + " did " + to_string(attackDamage) + " damage using " + attack.AttackName + " attack.");
}


tuple<std::string, int, InventorySlot*> Enemy::EndTurn()
{
	//srand(time(NULL));
	if (CurrentHealth <= 0 && !hasDied)
	{
		hasDied = true;
		return Die();
	}

	// reduce and apply status effects
	vector<int> removeIndecies;
	for (int i = 0; i < StatusEffects.size(); i++)
	{
		StatusEffects[i].TurnsRemaining--;
		int damage = 0;
		switch (StatusEffects[i].Effect)
		{
		case SE_WeakPoison:
			damage = round(MaxHealth * .05f);
			damage = (damage > 0) ? damage : 1;
			CurrentHealth -= damage;
			Print(Name + " took " + to_string(damage) + " poison damage.");
			break;
		case SE_StrongPoison:
			damage = round(MaxHealth * .1f);
			damage = (damage > 1) ? damage : 2;
			CurrentHealth -= damage;
			Print(Name + " took " + to_string(damage) + " poison damage.");
			break;
		case SE_WeakBleed:
			damage = round(MaxHealth * .05f);
			damage = (damage > 0) ? damage : 1;
			CurrentHealth -= damage;
			Print(Name + " took " + to_string(damage) + " bleed damage.");
			break;
		case SE_StrongBleed:
			damage = round(MaxHealth * .1f);
			damage = (damage > 1) ? damage : 2;
			CurrentHealth -= damage;
			Print(Name + " took " + to_string(damage) + " bleed damage.");
			break;
		}

		if (StatusEffects[i].TurnsRemaining <= 0)
			removeIndecies.push_back(i);
	}

	for (int i = removeIndecies.size() - 1; i >= 0; i--)
	{
		StatusEffects.erase(StatusEffects.begin() + removeIndecies[i]);
	}

	if (CurrentHealth <= 0 && !hasDied)
	{
		hasDied = true;
		return Die();
	}

	if (CanFly)
	{
		float chance = (float)rand() / RAND_MAX;
		if (chance <= (1.0f / 4.0f))
			ChangeStance();
	}

	return make_tuple(Name, 0, nullptr);
}

tuple<std::string, int, InventorySlot*> Enemy::Die()
{
	tuple<std::string, int, InventorySlot*> response = make_tuple(Name, XPGranted, ItemDrop);
	return response;
}


void Enemy::AddStatusEffect(StatusEffect effect, int turnCount)
{
	// check if effect is already applied
	for (StatusEffectTracker currentEffect : StatusEffects)
	{
		if (currentEffect.Effect == effect)
		{
			// pick the largest number
			currentEffect.TurnsRemaining = (currentEffect.TurnsRemaining > turnCount) ? currentEffect.TurnsRemaining : turnCount;

			return;
		}
	}
	// effect doesn't exist
	StatusEffects.push_back(StatusEffectTracker(effect, turnCount));
}

std::string Enemy::GetName() { return Name; }
int Enemy::GetArmor() { return Armor; }
bool Enemy::IsGroundedCheck() { return IsGrounded; }

std::vector<StatusEffect> Enemy::GetStatusEffects()
{
	vector<StatusEffect> effs;
	for (StatusEffectTracker effect : StatusEffects)
		effs.push_back(effect.Effect);

	return effs;
}

bool Enemy::HasStatusEffect(StatusEffect effectSearch)
{
	for (auto eff : StatusEffects)
	{
		if (eff.Effect == effectSearch)
			return true;
	}

	return false;
}

int Enemy::ModifyHealth(int healthMod)
{
	CurrentHealth += healthMod;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
	else if (CurrentHealth < 0)
		CurrentHealth = 0;

	return CurrentHealth;
}


std::string Enemy::PrintForCombat(int healthBarWidth)
{
	std::string line = rjust(to_string(CurrentHealth), 3, '0') + '/' + rjust(to_string(MaxHealth), 3, '0');
	line += ProgressBar(healthBarWidth, MaxHealth, CurrentHealth);
	line += "- " + ljust(Name, 20);
	return line;
}
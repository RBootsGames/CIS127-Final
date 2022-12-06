#include "Player.h"

#include <string>
#include <tuple>
#include "inventory.h"
#include "Rooms.h"
#include "extensions.h"
#include "GameDatabase.h"
#include "enemy.h"
#include "objects.h"
#include "commands.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

PlayerInventory::PlayerInventory() {}

vector<json> PlayerInventory::Serialize()
{
	vector<json> s_items;

	for (InventorySlot _slot : Slots)
	{
		s_items.push_back(_slot.Serialize());
	}

	//j["Items"] = s_items;
	return s_items;
}

PlayerInventory* PlayerInventory::Deserialize(vector<json> data)
{
	//vector<InventorySlot>* slots;
	PlayerInventory* inv = new PlayerInventory();

	for (json serial_slot : data)
	{
		inv->Slots.push_back(*InventorySlot::Deserialize(serial_slot));
	}

	return inv;
}

void PlayerInventory::PrintInventory()
{
	/* Style
	┌──────────────────────────────┐ ┌─────────────────────────────────────────────────────────┐
	│____________Items_____________│ │_______________________Weapons___________________________│
	├────────────────────┬─────────┼─┼──────────────┬──────────────────────┬────────┬──────────┤
	│	      Name       │  Count  │ │	  Type	    │		Name		   │ Damage │   Range  │
	├────────────────────┼─────────┤ ├──────────────┼──────────────────────┼────────┼──────────┤
	│  Health Potion     │  10/10  │ │    Melee     │     Long Sword       │   15   │    3     │
	└────────────────────┴─────────┘ └──────────────┴──────────────────────┴────────┴──────────┘
	*/

	// spacing
	int spaceName = 23;
	int spaceQuantity = 11;

	int spaceType = 12;
	int spaceDamage = 10;
	int spaceRange = 8;

	// player stats
	Print(player.GetHealthBar(30) + ' ', false);
	Print(player.GetXPBar(30) + ' ');

	// top bar
	Print(char(201) + string(spaceName + spaceQuantity + 1, char(205)) + char(187), false);						// items
	Print(char(201) + string(spaceType + spaceName + spaceDamage + spaceRange + 3, char(205)) + char(187));		// weapons


	Print(char(186), false);																		// items
	Print(cjust("Items", spaceName + spaceQuantity + 1), false, true);								// items
	Print(char(186), false);																		// items

	Print(char(186), false);																		// weapons
	Print(cjust("Weapons", spaceType + spaceName + spaceDamage + spaceRange + 3), false, true);		// weapons
	Print(char(186), true);																			// weapons


	Print(char(186), false);																		// items
	Print(cjust("Name", spaceName) + char(179) + cjust("Count", spaceQuantity), false, true);		// items
	Print(char(186), false);																		// items

	Print(char(186), false);																		// weapons
	Print(cjust("Type", spaceType), false, true);													// weapons
	Print(char(179) + cjust("Name", spaceName), false, true);										// weapons
	Print(char(179) + cjust("Damage", spaceDamage), false, true);									// weapons
	Print(char(179) + cjust("Range", spaceRange), false, true);										// weapons
	Print(char(186));																				// weapons


	Print(char(195), false);																		// items
	Print(string(spaceName, char(196)) + char(197) + string(spaceQuantity, char(196)), false);		// items
	Print(char(180), false);																		// items

	Print(char(195), false);																		// weapons
	Print(string(spaceType, char(196)), false);														// weapons
	Print(char(197) + string(spaceName, char(196)), false);											// weapons
	Print(char(197) + string(spaceDamage, char(196)), false);										// weapons
	Print(char(197) + string(spaceRange, char(196)), false);										// weapons
	Print(char(180));																				// weapons


	vector<string> itemMenu;
	vector<string> weaponMenu;

	for (InventorySlot _slot : Slots)
	{
		if (_slot.GetItemType() == TItem)
			itemMenu.push_back(_slot.PrintInfoItem(spaceName, spaceQuantity));
		else if (_slot.GetItemType() == TMelee || _slot.GetItemType() == TRanged)
			weaponMenu.push_back(_slot.PrintInfoWeapon(spaceType, spaceName, spaceDamage, spaceRange));
	}


	string ibottom = char(192) + string(spaceName, char(196)) + char(193) + string(spaceQuantity, char(196)) + char(217);
	string wbottom = char(192) + string(spaceType, char(196)) + char(193) + string(spaceName, char(196)) +
		char(193) + string(spaceDamage, char(196)) + char(193) + string(spaceRange, char(196)) + char(217);

	int imenuSize = (int)itemMenu.size();
	int wmenuSize = (int)weaponMenu.size();
	for (int i = 0; i < imenuSize || i < wmenuSize; i++)
	{
		if (i < imenuSize && i < wmenuSize)
		{
			Print(itemMenu[i], false);
			Print(weaponMenu[i], true);

			if (i == imenuSize - 1 && i == wmenuSize - 1)
				Print(ibottom + wbottom);
		}
		else if (i < imenuSize)
		{
			Print(itemMenu[i], false);
			if (i == wmenuSize)
				Print(wbottom);
			if (i == imenuSize - 1)
				Print(ibottom, false);
			Print();
		}
		else
		{
			if (i == imenuSize)
				Print(ibottom + weaponMenu[i]);
			else
				Print(string(spaceName + spaceQuantity + 3, ' ') + weaponMenu[i]);

			if (i == wmenuSize - 1)
				Print(string(spaceName + spaceQuantity + 3, ' ') + wbottom, false);
		}
	}


	Print();
}

InventorySlot* PlayerInventory::GetHealthPotions()
{
	InventorySlot* potion = nullptr;

	for (int i = 0; i < Slots.size(); i++)
	{
		InventorySlot* item = &Slots[i];
		if (item->GetName() == item_HealthPotion.GetName())
		{
			potion = item;
			break;
		}
	}
	//for (InventorySlot item : Slots)
	//{
	//}

	return potion;
}


int PlayerInventory::AddItem(Item& item, int count, bool supressOutput)
{
	// check for existing items
	bool isNewItem = true;
	for (InventorySlot& _slot : Slots)
	{
		if (item.GetName() == _slot.GetName())
		{
			if (_slot.NumberInStack == item.StackLimit)
			{
				if (!supressOutput)
					Print("You can't carry any more of these.");
				return count;
			}

			isNewItem = false;
			_slot.NumberInStack += count;
			if (_slot.NumberInStack > item.StackLimit) // more than you can carry
			{
				int tooMany = _slot.NumberInStack - item.StackLimit;
				_slot.NumberInStack = item.StackLimit;

				if (!supressOutput)
				{
					Print("You picked up " + to_string((count - tooMany)) + " ", false);
					if ((count - tooMany) == 1)
						Print(item.GetName(), false, true);
					else
						Print(item.GetName() + "s", false, true);

					Print(" and left " + to_string(tooMany) + " behind.");
				}
				return tooMany;
			}
			else // picked up all items
			{
				break;
			}
		}
	}

	int tooMany = 0;
	// create inventory slot for new items
	if (isNewItem)
	{
		if (count > item.StackLimit)
		{
			tooMany = count - item.StackLimit;
			count = item.StackLimit;
		}
		Slots.push_back(InventorySlot(item, count));
	}

	if (!supressOutput)
	{
		Print("You picked up " + to_string(count) + " ", false);
		if (count == 1)
			Print(item.GetName(), false, true);
		else
			Print(item.GetName() + "s", false, true);

		if (tooMany > 0)
			Print(" and left " + to_string(tooMany) + " behind", false);

		Print(".");
		//Print("You picked up " + to_string(count) + " ", false);
		//if (count == 1)
		//	Print(item.GetName(), false, true);
		//else
		//	Print(item.GetName() + "s", false, true);
	}


	return tooMany;
}
int PlayerInventory::AddItem(MeleeWeapon& item, int count, bool supressOutput)
{
	// can't carry duplicate weapons
	for (InventorySlot& _slot : Slots)
	{
		if (item.GetName() == _slot.GetName())
		{
			if (!supressOutput)
				Print("You can't carry any more of these.");
			return count;
		}
	}

	Slots.push_back(InventorySlot(item, count));
	if (!supressOutput)
	{
		Print("You picked up ", false);
		Print(item.GetName(), false, true);
		Print(".");
	}
	return 0;
}
int PlayerInventory::AddItem(RangedWeapon& item, int count, bool supressOutput)
{
	// can't carry duplicate weapons
	for (InventorySlot& _slot : Slots)
	{
		if (item.GetName() == _slot.GetName())
		{
			if (!supressOutput)
				Print("You can't carry any more of these.");
			return count;
		}
	}

	Slots.push_back(InventorySlot(item, count));
	if (!supressOutput)
	{
		Print("You picked up ", false);
		Print(item.GetName(), true, true);
		Print(".");
	}
	return 0;
}
int PlayerInventory::AddItem(InventorySlot& item)
{
	switch (item.GetItemType())
	{
	case TItem:
		return AddItem(item.GetItemAsItem(), item.NumberInStack);
	case TMelee:
		return AddItem(item.GetItemAsMelee(), item.NumberInStack);
	case TRanged:
		return AddItem(item.GetItemAsRanged(), item.NumberInStack);
	}
	return 0;
}

void PlayerInventory::DropItem(string itemName, Room& currentRoom, int count)
{
	InventorySlot* matchedItem = nullptr;
	tie(matchedItem, count) = RemoveItem(itemName, count);

	if (matchedItem != nullptr)
	{
		TItemType itemType = matchedItem->GetItemType();
		itemName = matchedItem->GetName();

		// Add item back into current room
		switch (itemType)
		{
		case TItem:
			currentRoom.AddItem(matchedItem->GetItemAsItem(), count);
			break;
		case TMelee:
			currentRoom.AddItem(matchedItem->GetItemAsMelee(), count);
			break;
		case TRanged:
			currentRoom.AddItem(matchedItem->GetItemAsRanged(), count);
			break;
		default:
			Print("This is not a valid item type.");
			return;
		}

		Print("You dropped " + to_string(count) + " ", false);
		Print(itemName, false, true);
		if (count > 1)
			Print("(s).");
		else
			Print(".");

	}
}

/// <summary>
/// Returns the matching item and the number of items removed.
/// </summary>
tuple<InventorySlot*, int> PlayerInventory::RemoveItem(string itemName, int count)
{
	// get item type first
	itemName = ToLower(itemName);
	TItemType itemType = None;
	int index = 0;
	bool removeItem = false;

	InventorySlot* matchedItem = nullptr;

	for (InventorySlot& _slot : Slots)
	{
		if (itemName == ToLower(_slot.GetName()) || itemName == ToLower(_slot.GetName() + "s"))
		{
			matchedItem = &_slot;
			break;
		}
		index++;
	}

	if (matchedItem != nullptr)
	{
		itemType = matchedItem->GetItemType();
		itemName = matchedItem->GetName();

		if (count > matchedItem->NumberInStack)
			count = matchedItem->NumberInStack;

		matchedItem->NumberInStack -= count;
		if (matchedItem->NumberInStack <= 0)
			removeItem = true;

	}

	if (removeItem)
		Slots.erase(Slots.begin() + index);

	return make_tuple(matchedItem, count);
}

void PlayerInventory::EquipItem(string itemName, bool supressOutput)
	{
		// get item type first
		itemName = ToLower(itemName);
		TItemType itemType = None;

		for (InventorySlot& _slot : Slots)
		{
			if (itemName == ToLower(_slot.GetName()))
			{
				itemType = _slot.GetItemType();
				itemName = _slot.GetName();
				_slot.IsEquiped = true;
				if (!supressOutput)
				{
					Print("You equipped ", false);
					Print(itemName, false, true);
					Print(".");
				}
				break;
			}
		}

		if (itemType == None)
		{
			Print("I don't have that item.");
			return;
		}
		else if (itemType == TItem)
		{
			Print("I can only equip weapons.");
			return;
		}

		// unequip all like-items
		for (InventorySlot& _slot : Slots)
		{
			if (itemName == _slot.GetName())
				continue;
			else if (_slot.GetItemType() == itemType)
				_slot.IsEquiped = false;
		}
	}

tuple<MeleeWeapon*, RangedWeapon*> PlayerInventory::GetEquipedWeapons()
{
	MeleeWeapon* melee = nullptr;
	RangedWeapon* ranged = nullptr;

	for (InventorySlot _slot : Slots)
	{
		if (_slot.IsEquiped)
		{
			switch (_slot.GetItemType())
			{
			case TMelee:
				melee = &_slot.GetItemAsMelee();
				break;
			case TRanged:
				ranged = &_slot.GetItemAsRanged();
				break;
			}
		}
	}

	return make_tuple(melee, ranged);
}




json Player::Serialize()
{
	json j;
	j["RoomLocation"] = RoomLocation.Serialize();
	j["Inventory"] = Inventory.Serialize();
	j["PlayerLevel"] = PlayerLevel;
	j["MaxHealth"] = MaxHealth;
	j["CurrentHealth"] = CurrentHealth;
	j["CurrentXP"] = CurrentXP;
	j["NextLevelXP"] = NextLevelXP;
	j["Stamina"] = Stamina;
	for (StatusEffectTracker effect : StatusEffects)
		j["StatusEffect"].push_back(effect.Serialize());
	return j;
}

Player* Player::Deserialize(json playerData)
{
	Player* p = new Player();

	p->JumpToRoom(*Vector2::Deserialize(playerData["RoomLocation"]));
	p->Inventory = *PlayerInventory::Deserialize(playerData["Inventory"]);
	p->PlayerLevel = playerData["PlayerLevel"];
	p->MaxHealth = playerData["MaxHealth"];
	p->CurrentHealth = playerData["CurrentHealth"];
	p->CurrentXP = playerData["CurrentXP"];
	p->NextLevelXP = playerData["NextLevelXP"];
	p->Stamina = playerData["Stamina"];

	for (StatusEffect eff : playerData["StatusEffect"])
		p->StatusEffects.push_back(*StatusEffectTracker::Deserialize(eff));

	return p;
}

Player::Player()
{
	PlayerLevel = 1;
	MaxHealth = 20;
	CurrentHealth = MaxHealth;
	IsDead = false;
	CurrentXP = 0;
	NextLevelXP = 25;
	Stamina = 8;

	RoomLocation = Vector2();
	CurrentRoom = nullptr;
	Inventory = PlayerInventory();
	Inventory.AddItem(melee_Unarmed, 1, true);
	Inventory.EquipItem(melee_Unarmed.GetName(), true);
	Inventory.AddItem(ranged_BasicBow, 1, true);
	Inventory.EquipItem(ranged_BasicBow.GetName(), true);
}

void Player::JumpToRoom(Room* room)
{
	for (int y = 0; y < Level1.FullLevel.size(); y++) {
		for (int x = 0; x < Level1.FullLevel[y].size(); x++)
		{					   //       X and Y are inverted here to make it easier to read when laying out
			if (*room == Level1.FullLevel[y][x])//    the level in the GameDatabase.
			{
				//RoomLocation = Vector2(x, y);
				JumpToRoom(Vector2(x, y));
				//if (CurrentRoom != nullptr)
				//	this->CurrentRoom->ExitRoom();
				//CurrentRoom = room;
				//CurrentRoom->EnterRoom();
				return;
			}
		}
	}
}
void Player::JumpToRoom(Vector2 location)
{
	if (CurrentRoom != nullptr)
		this->CurrentRoom->ExitRoom();

	RoomLocation = location; //			X and Y are inverted here to make it easier to read when laying out
	CurrentRoom = &Level1.FullLevel[location.Y][location.X]; //    the level in the GameDatabase.
	CurrentRoom->EnterRoom();
}

string Player::EnterRoom(string doorDirection)
{
	Doors dir;
	if (doorDirection == "north")
		dir = North;
	else if (doorDirection == "east")
		dir = East;
	else if (doorDirection == "south")
		dir = South;
	else if (doorDirection == "west")
		dir = West;
	else
		return "I can't move that direction.";

	return EnterRoom(dir);
}
string Player::EnterRoom(Doors doorDirection)
{
	// check if door is locked
	if (count(CurrentRoom->GetLockedDoors().begin(), CurrentRoom->GetLockedDoors().end(), doorDirection))
	{
		return "That door is locked.";
	}
	else if (count(CurrentRoom->GetUnlockedDoors().begin(), CurrentRoom->GetUnlockedDoors().end(), doorDirection))
	{
		Vector2 nextLocation = RoomLocation;
		string message = "You moved one room to the ";

		switch (doorDirection)
		{
		case North:
			nextLocation.Y--;
			message += "north.";
			break;
		case East:
			nextLocation.X++;
			message += "east.";
			break;
		case South:
			nextLocation.Y++;
			message += "south.";
			break;
		case West:
			nextLocation.X--;
			message += "west.";
			break;
		}
		JumpToRoom(nextLocation);

		return message;
	}
	else
		return "There is no door in that direction.";
}

string Player::UnlockDoor(string doorDirection)
{
	Doors dir = Door_None;

	if (doorDirection == "north")
		dir = North;
	else if (doorDirection == "east")
		dir = East;
	else if (doorDirection == "south")
		dir = South;
	else if (doorDirection == "west")
		dir = West;

	if (dir == Door_None)
		return "That is not a valid door.";

	InventorySlot* keys = GetKeys();

	if (keys != nullptr)
	{
		// unlock door on the other side
		Inventory.RemoveItem(keys->GetName());
		switch (dir)
		{
		case North:
			AdjacentRoom(Vector2(0, -1))->UnlockDoor(South);
			break;
		case East:
			AdjacentRoom(Vector2(1, 0))->UnlockDoor(West);
			break;
		case South:
			AdjacentRoom(Vector2(0, 1))->UnlockDoor(North);
			break;
		case West:
			AdjacentRoom(Vector2(-1, 0))->UnlockDoor(East);
			break;
		}
		return GetRoom()->UnlockDoor(dir);
	}
	else
		return "You don't have any keys.";

}

Room* Player::GetRoom() { return CurrentRoom; }

Room* Player::AdjacentRoom(Vector2 direction)
{
	Vector2 adjacent = RoomLocation + direction;
	return &Level1.FullLevel[adjacent.Y][adjacent.X];
}

InventorySlot* Player::GetKeys()
{
	InventorySlot* keys = nullptr;

	for (InventorySlot _slot : Inventory.Slots)
	{
		if (_slot.GetName() == item_GoldKey.GetName())
		{
			keys = &_slot;
			break;
		}
	}

	return keys;
}


// combat stuff

std::string Player::GetHealthBar(int barWidth)
{
	std::string line = "Health: ";
	line += rjust(to_string(CurrentHealth), 3, '0') + '/' + rjust(to_string(MaxHealth), 3, '0');
	line += ProgressBar(barWidth, MaxHealth, CurrentHealth);
	return line;
}
std::string Player::GetStaminaBar(int barWidth, int usedStam)
{
	std::string line = "Stamina: ";
	line += rjust(to_string(Stamina - usedStam), 2, '0') + '/' + rjust(to_string(Stamina), 2, '0');
	line += ProgressBar(barWidth, Stamina, Stamina-usedStam, char(205));
	return line;
}
std::string Player::GetXPBar(int barWidth)
{
	std::string line = "Level: " + to_string(PlayerLevel) + ' ';
	line += "XP: ";
	line += rjust(to_string(CurrentXP), 3, '0') + '/' + rjust(to_string(NextLevelXP), 3, '0');
	line += ProgressBar(barWidth, NextLevelXP, CurrentXP);
	return line;
}
void Player::AddXP(int xp)
{
	if (xp == 0)
		return;

	Print("You gained " + to_string(xp) + " XP.");
	bool stillLeveling = true;
	while (stillLeveling)
	{
		CurrentXP += xp;
		if (CurrentXP > NextLevelXP)
		{
			xp = CurrentXP - NextLevelXP;
			CurrentXP = 0;
			LevelUp();
		}
		else if (CurrentXP == NextLevelXP)
		{
			LevelUp();
			stillLeveling = false;
		}
		else
			stillLeveling = false;
	}
}
std::string Player::UseHealthPotion()
{
	InventorySlot* potion = Inventory.GetHealthPotions();
	if (potion == nullptr)
		return "You don't have any health potions.";

	Inventory.RemoveItem(potion->GetName());
	int before = CurrentHealth;
	ModifyHealth(GetHealAmount());

	return "You healed " + to_string(CurrentHealth - before) + " health.";
}


void Player::Attack(MeleeWeapon weapon, int attackIndex, Enemy& enemy)
{
	// check if enemy is flying
	if (!enemy.IsGroundedCheck())
	{
		Print("I can't hit " + enemy.GetName() + " from here. It needs to be on the ground.");
		return;
	}

	// get damaged based on weapon and attack
	AttackMove attack = weapon.Attacks[attackIndex];
	int damage = weapon.Damage + attack.DamageAddition;

	// adjust based on status effect
	bool applyEffect = false;
	if (attack.StatusEffects != SE_None)
	{
		float chance = (float)rand() / RAND_MAX;
		if (chance <= attack.StatusProbability)
			applyEffect = true;
	}

	// adjust based on armor
	damage -= enemy.GetArmor();

	enemy.ModifyHealth(-damage);
	Print("You did " + to_string(damage) + " damage to " + enemy.GetName() + ".");
}
void Player::Attack(RangedWeapon weapon, int attackIndex, Enemy& enemy)
{
	//srand(time(NULL));
	// get damaged based on weapon and attack
	AttackMove attack = weapon.Attacks[attackIndex];
	int damage = weapon.Damage + attack.DamageAddition;

	// adjust based on status effect
	bool applyEffect = false;
	if (attack.StatusEffects != SE_None)
	{
		float chance = (float)rand() / RAND_MAX;
		if (chance <= attack.StatusProbability)
			applyEffect = true;
	}

	// adjust based on armor
	damage -= enemy.GetArmor();

	enemy.ModifyHealth(-damage);
	Print("You did " + to_string(damage) + " damage to " + enemy.GetName() + ".");

	// check if enemy is flying
	if (!enemy.IsGroundedCheck())
	{
		float chance = (float)rand() / RAND_MAX;
		if (chance <= .75f)
			enemy.ChangeStance(true);
	}
}
void Player::Attack(AttackQueue attack)
{
	if (attack.melee != nullptr)
		Attack(*attack.melee, attack.attackIndex, attack.targetEnemy);
	else if (attack.ranged != nullptr)
		Attack(*attack.ranged, attack.attackIndex, attack.targetEnemy);
}

void Player::EndTurn()
{
	if (CurrentHealth <= 0 && !IsDead)
	{
		IsDead = true;
		DisableAllCommands("You died. You need to load your last save or start over.");
		Print("You were killed. You need to load your last save or restart the game.");
		return;
	}
	else if (IsDead)
		return;

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
			Print("You took " + to_string(damage) + " poison damage.");
			break;
		case SE_StrongPoison:
			damage = round(MaxHealth * .1f);
			damage = (damage > 1) ? damage : 2;
			CurrentHealth -= damage;
			Print("You took " + to_string(damage) + " poison damage.");
			break;
		case SE_WeakBleed:
			damage = round(MaxHealth * .05f);
			damage = (damage > 0) ? damage : 1;
			CurrentHealth -= damage;
			Print("You took " + to_string(damage) + " bleed damage.");
			break;
		case SE_StrongBleed:
			damage = round(MaxHealth * .1f);
			damage = (damage > 1) ? damage : 2;
			CurrentHealth -= damage;
			Print("You took " + to_string(damage) + " bleed damage.");
			break;
		}

		if (StatusEffects[i].TurnsRemaining <= 0)
			removeIndecies.push_back(i);
	}

	for (int i = removeIndecies.size() - 1; i >= 0; i--)
	{
		StatusEffects.erase(StatusEffects.begin() + removeIndecies[i]);
	}

	if (CurrentHealth <= 0)
	{
		DisableAllCommands("You died. You need to load your last save or start over.");
		Print("You were killed. You need to load your last save or restart the game.");
	}
}

void Player::Die(Enemy& murderer)
{
	DisableAllCommands("You died. You need to load your last save or restart the game.");

	Print("You were killed by " + murderer.GetName() + ". You need to load your last save or restart the game.");
}

void Player::LevelUp()
{
	PlayerLevel++;
	MaxHealth = round(MaxHealth * 1.125f);
	CurrentHealth = MaxHealth;
	NextLevelXP = round(NextLevelXP * 1.1f);
	Stamina = round(Stamina * 1.1f);

	Print("You are now level " + to_string(PlayerLevel) + ".");
}

void Player::AddStatusEffect(StatusEffect effect, int turnCount)
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

std::vector<StatusEffect> Player::GetStatusEffects()
{
	vector<StatusEffect> effs;
	for (StatusEffectTracker effect : StatusEffects)
		effs.push_back(effect.Effect);

	return effs;
}

bool Player::HasStatusEffect(StatusEffect effectSearch)
{
	for (auto eff : StatusEffects)
	{
		if (eff.Effect == effectSearch)
			return true;
	}

	return false;
}



int Player::ModifyHealth(int healthMod)
{
	CurrentHealth += healthMod;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
	else if (CurrentHealth < 0)
		CurrentHealth = 0;

	return CurrentHealth;
}
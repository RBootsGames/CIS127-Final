#pragma once
#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include "Items.h"
#include "Rooms.h"

using namespace std;

// Items
static MeleeWeapon melee_Unarmed = MeleeWeapon("Unarmed", 5);
static MeleeWeapon melee_ImportantSword = MeleeWeapon("Important Sword", 10);
static MeleeWeapon melee_CurvedSword = MeleeWeapon("Curved Sword", 12);

static Item item_HealthPotion = Item("Health Potion", 8, true);
static Item item_GoldKey = Item("Golden Key", 5, true);


// Rooms
static InventorySlot _items[] = {
	InventorySlot(item_HealthPotion, 3),
	InventorySlot(melee_CurvedSword),
	InventorySlot(melee_ImportantSword)
};

static Room room_Start(_items, 3);
static Room room_Empty;


#endif // !GAMEDATABASE_H

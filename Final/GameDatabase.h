#pragma once
#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include "Items.h"
#include "Rooms.h"
#include "Level.h"

using namespace std;

// Items
static MeleeWeapon melee_Unarmed = MeleeWeapon("Unarmed", 5);
static MeleeWeapon melee_ImportantSword = MeleeWeapon("Important Sword", 10);
static MeleeWeapon melee_CurvedSword = MeleeWeapon("Curved Sword", 12);

static Item item_HealthPotion = Item("Health Potion", 8, true);
static Item item_GoldKey = Item("Golden Key", 5, true);


// Rooms

static Room room_DUMMY;

static Room room_Start({
	InventorySlot(item_HealthPotion, 3),
	InventorySlot(item_GoldKey, 10),
	InventorySlot(melee_CurvedSword),
	InventorySlot(melee_ImportantSword)},
	{ West },
	{ North, East, South });

static Room room_00_00({
	InventorySlot(item_GoldKey, 20)},
	{East},
	{North, South, West});

static Room room_02_00(vector<InventorySlot>(),
	{East},
	{West, South });

static Room room_03_00(
	{ East, West },
	{ South, North });

static Room room_01_01(
	{ North, South },
	{ East });

static Room room_01_02(
	{ North, East },
	{ West });

static Room room_02_02(vector<InventorySlot>(),
	{ West },
	{ North });

// Level

static Level Level1(
	{
		{ room_00_00, room_Start, room_02_00, room_03_00},
		{ room_DUMMY, room_01_01},
		{ room_DUMMY, room_01_02, room_02_02 }
	}
);

#endif // !GAMEDATABASE_H

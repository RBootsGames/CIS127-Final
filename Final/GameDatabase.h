#pragma once
#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H


#include "Items.h"
#include "Rooms.h"
#include "Level.h"
#include "Player.h"
//#include "objects.h"

//using namespace std;

extern Player player;
extern bool exitProgram;

// Items
extern Item item_HealthPotion;
extern Item item_GoldKey;
extern int itemsArraySize;
extern Item* itemsArray[2];

extern MeleeWeapon melee_Unarmed;
extern MeleeWeapon melee_ImportantSword;
extern MeleeWeapon melee_CurvedSword;
extern int meleeArraySize;
extern MeleeWeapon* meleeArray[3];

extern RangedWeapon ranged_Placeholder;
extern int rangedArraySize;
extern RangedWeapon* rangedArray[1];

// Rooms

extern Room room_DUMMY;

extern Room room_Start;

extern Room room_00_00;

extern Room room_02_00;

extern Room room_03_00;

extern Room room_01_01;

extern Room room_01_02;

extern Room room_02_02;

// Level

extern Level Level1;

#endif // !GAMEDATABASE_H

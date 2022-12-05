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
extern bool inCombat;
extern Room* combatRoom;

// Items
extern Item item_HealthPotion;
extern Item item_GoldKey;
extern int itemsArraySize;
extern Item* itemsArray[2];

// Melee weapons

extern MeleeWeapon melee_Unarmed;
extern MeleeWeapon melee_ShortSword;
extern MeleeWeapon melee_CurvedSword;
extern MeleeWeapon melee_Spear;
extern MeleeWeapon melee_Mace;
extern MeleeWeapon melee_MorningStar;
extern int meleeArraySize;
extern MeleeWeapon* meleeArray[];

// Ranged weapons

extern RangedWeapon ranged_BasicBow;
extern RangedWeapon ranged_RecurveBow;
extern RangedWeapon ranged_Crossbow;
extern int rangedArraySize;
extern RangedWeapon* rangedArray[];

// Enemies

extern Enemy* nme_Slime;
extern Enemy* nme_Skeleton;
extern Enemy* nme_CursedArmor;
extern Enemy* nme_BasicMage;
extern Enemy* nme_Mimic;
extern Enemy* nme_Bat;
extern Enemy* nme_Wisp;
extern Enemy* nme_Mosquito;

// Rooms

extern Room room_DUMMY;

extern Room room_Start;

extern Room room_00_00;

extern Room room_02_00;

extern Room room_03_00;

extern Room room_01_01;

extern Room room_01_02;

extern Room room_01_03;

extern Room room_02_02;

extern Room room_03_02;

extern Room room_03_03;

extern Room room_03_04;

extern Room room_04_04;

extern Room room_01_04;

extern Room room_00_04;

extern Room room_03_05;

extern Room room_02_05;

extern Room room_02_06;

extern Room room_02_07;

// Level

extern Level Level1;

#endif // !GAMEDATABASE_H

#include "GameDatabase.h"


#include "Items.h"
#include "Rooms.h"
#include "Level.h"
//#include "objects.h"

//using namespace std;

Player player;
bool exitProgram = false;

// Items
Item item_HealthPotion = Item("Health Potion", 8, true);
Item item_GoldKey = Item("Golden Key", 5, true);
int itemsArraySize = 2;
Item* itemsArray[2] = { &item_HealthPotion, &item_GoldKey };

MeleeWeapon melee_Unarmed = MeleeWeapon("Unarmed", 5);
MeleeWeapon melee_ImportantSword = MeleeWeapon("Important Sword", 10);
MeleeWeapon melee_CurvedSword = MeleeWeapon("Curved Sword", 12);
int meleeArraySize = 3;
MeleeWeapon* meleeArray[3] = { &melee_Unarmed, &melee_ImportantSword, &melee_CurvedSword };

RangedWeapon ranged_Placeholder = RangedWeapon("Placeholder", 1, 2);
int rangedArraySize = 1;
RangedWeapon* rangedArray[1] = { &ranged_Placeholder };

// Rooms

Room room_DUMMY;

Room room_Start({
	InventorySlot(item_HealthPotion, 30),
	InventorySlot(item_GoldKey, 15),
	InventorySlot(melee_CurvedSword),
	InventorySlot(melee_ImportantSword) },
	{ West },
	{ North, East, South });

Room room_00_00({
	InventorySlot(item_GoldKey, 20) },
	{ East },
	{ North, South, West });

Room room_02_00(vector<InventorySlot>(),
	{ East },
	{ West, South });

Room room_03_00(
	{ East, West },
	{ South, North });

Room room_01_01(
	{ North, South },
	{ East });

Room room_01_02(
	{ North, East },
	{ West });

Room room_02_02(vector<InventorySlot>(),
	{ West },
	{ North });

// Level

Level Level1(
	{
		{ room_00_00, room_Start, room_02_00, room_03_00},
		{ room_DUMMY, room_01_01},
		{ room_DUMMY, room_01_02, room_02_02 }
	}
);

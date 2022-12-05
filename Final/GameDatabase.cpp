#include "GameDatabase.h"


#include "Items.h"
#include "Rooms.h"
#include "Level.h"
#include "enemy.h"
#include "objects.h"


bool exitProgram = false;
bool inCombat = false;
Room* combatRoom = nullptr;

// Items
Item item_HealthPotion = Item("Health Potion", 8, true);
Item item_GoldKey = Item("Golden Key", 5, true);
int itemsArraySize = 2;
Item* itemsArray[2] = { &item_HealthPotion, &item_GoldKey };

// Melee weapons

AttackMove atkM_Unarmed_Punch("Punch", "Basic punch attack", 2);
AttackMove atkM_Unarmed_UpperPunch("Uppercut", "A stronger punch attack", 4, 3);
MeleeWeapon melee_Unarmed = MeleeWeapon("Unarmed", 4, {atkM_Unarmed_Punch, atkM_Unarmed_UpperPunch});

AttackMove atkM_ShortSword_Stab("Stab", 3);
AttackMove atkM_ShortSword_Slash("Slash", 5, 5);
MeleeWeapon melee_ShortSword = MeleeWeapon("Short Sword", 10, {atkM_ShortSword_Stab, atkM_ShortSword_Slash});

AttackMove atkM_CurvedSword_Stab("Stab", 5);
AttackMove atkM_CurvedSword_Slash("Slash", 6, 7);
MeleeWeapon melee_CurvedSword = MeleeWeapon("Curved Sword", 12, {atkM_CurvedSword_Stab, atkM_CurvedSword_Slash});

AttackMove atkM_Spear_Stab("Stab", 7, 9);
AttackMove atkM_Spear_Slash("Slash", 4, 4);
MeleeWeapon melee_Spear = MeleeWeapon("Spear", 12, { atkM_Spear_Stab,atkM_Spear_Slash });

AttackMove atkM_Mace_Bash("Bash", 10);
AttackMove atkM_Mace_Crush("Crush", 13, 5);
MeleeWeapon melee_Mace = MeleeWeapon("Mace", 14, { atkM_Mace_Bash,atkM_Mace_Crush });

AttackMove atkM_MorningStar_Bash("Bash", 12);
AttackMove atkM_MorningStar_Crush("Crush", 15, 7);
MeleeWeapon melee_MorningStar = MeleeWeapon("Morning Star", 16, { atkM_MorningStar_Bash,atkM_MorningStar_Crush });

int meleeArraySize = 6;
MeleeWeapon* meleeArray[] = { &melee_Unarmed, &melee_ShortSword, &melee_CurvedSword, &melee_Spear, &melee_Mace, &melee_MorningStar };

// Ranged weapons

AttackMove atkR_BasicBow_Shoot("Shoot", 3);
RangedWeapon ranged_BasicBow("Basic Bow", 5, { atkR_BasicBow_Shoot });

AttackMove atkR_RecurveBow_Shoot("Shoot", 5);
RangedWeapon ranged_RecurveBow("Recurve Bow", 7, { atkR_RecurveBow_Shoot });

AttackMove atkR_Crossbow_Shoot("Shoot", 9);
RangedWeapon ranged_Crossbow("Crossbow", 9, { atkR_Crossbow_Shoot });

int rangedArraySize = 3;
RangedWeapon* rangedArray[] = { &ranged_BasicBow, &ranged_RecurveBow, &ranged_Crossbow };

// Enemies

AttackMove atkE_Slime_Jump("Jump", 0);
AttackMove atkE_Slime_Crush("Crush", 0, 2);
Enemy* nme_Slime = new Enemy("Slime", 8, 2, { atkE_Slime_Jump, atkE_Slime_Crush }, 6, 0);

AttackMove atkE_Skeleton_Stab("Stab", 0);
AttackMove atkE_Skeleton_Slash("Slash", 0, 2);
Enemy* nme_Skeleton = new Enemy("Skeleton", 11, 4, { atkE_Skeleton_Stab,atkE_Skeleton_Slash }, 9, 0);

AttackMove atkE_CursedArmor_Bash("Shield Bash", 0, 2);
AttackMove atkE_CursedArmor_Curse("Curse", 0, -1);
AttackMove atkE_CursedArmor_Shoulder("Shoulder Bash", 0, 3);
Enemy* nme_CursedArmor = new Enemy("Cursed Armor", 15, 5, { atkE_CursedArmor_Bash, atkE_CursedArmor_Curse, atkE_CursedArmor_Shoulder }, 13, 2);

AttackMove atkE_BasicMage_Missile("Magic Missile", 0);
AttackMove atkE_BasicMage_Fireball("Fireball", 0, 3);
Enemy* nme_BasicMage = new Enemy("Green Mage", 13, 4, { atkE_BasicMage_Missile, atkE_BasicMage_Fireball }, 11, 0);

AttackMove atkE_Mimic_Chomp("Chomp", 0, 4);
AttackMove atkE_Mimic_Lick("Lick", 0);
AttackMove atkE_Mimic_Roll("Roll", 0, 2);
Enemy* nme_Mimic = new Enemy("Mimic", 14, 5, { atkE_Mimic_Chomp,atkE_Mimic_Lick,atkE_Mimic_Roll }, 16, 3);

AttackMove atkE_Bat_Scratch("Scratch", 0, 0);
AttackMove atkE_Bat_Swoop("Swoop", 0, 1);
Enemy* nme_Bat = new Enemy("Bat", 5, 2, { atkE_Bat_Scratch, atkE_Bat_Swoop }, 4, 0, true, false);

AttackMove atkE_Wisp_Flutter("Flutter", 0);
AttackMove atkE_Wisp_Zap("Zap", 0, 2);
Enemy* nme_Wisp = new Enemy("Wisp", 7, 3, { atkE_Wisp_Flutter,atkE_Wisp_Zap }, 6, 0);

AttackMove atkE_Mosquito_Flurry("Flurry", 0);
AttackMove atkE_Mosquito_BloodDrain("Blood Drain", 0, 3);
Enemy* nme_Mosquito = new Enemy("Giant Mosquito", 11, 3, { atkE_Mosquito_Flurry,atkE_Mosquito_BloodDrain }, 9, 0);


// Rooms
Room room_DUMMY;

Room room_Start({ InventorySlot(item_HealthPotion) },
	{ East, South, West },
	{});


Room room_00_00({
	InventorySlot(item_GoldKey, 2) },
	{ East },
	{ },
	{ nme_Bat });

Room room_02_00({ InventorySlot(item_HealthPotion) },
	{ West },
	{ East },
	{ nme_Slime, nme_Bat });

Room room_03_00({ InventorySlot(melee_ShortSword) },
	{  },
	{ West },
	{ nme_Skeleton });

Room room_01_01(
	{ North, South },
	{ },
	{nme_Bat, nme_Slime});

Room room_01_02(
	{ North, East },
	{ South });

Room room_01_03({InventorySlot(melee_CurvedSword)},
	{South},
	{North});

Room room_02_02(vector<InventorySlot>(),
	{ East, West },
	{  },
	{nme_Skeleton, nme_BasicMage});

Room room_03_02({ InventorySlot(item_HealthPotion, 2) },
	{ South, West },
	{},
	{ nme_Wisp, nme_Wisp });

Room room_03_03(
	{ InventorySlot(item_GoldKey) },
	{ North, South },
	{},
	{ nme_Mimic });

Room room_03_04({ InventorySlot(item_HealthPotion, 1) },
	{ North, South },
	{ East },
	{ nme_Skeleton, nme_Skeleton, nme_Slime });

Room room_04_04({ InventorySlot(melee_Spear) },
	{},
	{ West },
	{ nme_CursedArmor, nme_Slime });

Room room_01_04(
	{ North },
	{ West },
	{ nme_BasicMage });

Room room_00_04({ InventorySlot(ranged_RecurveBow) },
	{},
	{ East },
	{ nme_CursedArmor });

Room room_03_05({ InventorySlot(item_GoldKey, 2) },
	{ North, West },
	{});

Room room_02_05(
	{ East, South },
	{},
	{ nme_BasicMage, nme_Skeleton });

Room room_02_06({InventorySlot(item_HealthPotion, 3)},
	{ North },
	{ South },
	{ nme_CursedArmor });

Room room_02_07(
	{},
	{ North },
	{ nme_Mosquito, nme_Mimic, nme_CursedArmor });

// Level

Level Level1(
	{
		{ room_00_00, room_Start, room_02_00, room_03_00},
		{ room_DUMMY, room_01_01},
		{ room_DUMMY, room_01_02, room_02_02, room_03_02 },
		{ room_DUMMY, room_01_03, room_DUMMY, room_03_03},
		{ room_00_04, room_01_04, room_DUMMY, room_03_04, room_04_04},
		{ room_DUMMY, room_DUMMY, room_02_05, room_03_05},
		{ room_DUMMY, room_DUMMY, room_02_06},
		{ room_DUMMY, room_DUMMY, room_02_07}
	}
);



Player player;

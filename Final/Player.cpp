#include "Player.h"

#include <string>
#include <tuple>
#include "inventory.h"
#include "Rooms.h"
#include "extensions.h"
#include "GameDatabase.h"
//#include "objects.h"
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
	tie(matchedItem, count) = RemoveItem(itemName, currentRoom, count);

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
tuple<InventorySlot*, int> PlayerInventory::RemoveItem(string itemName, Room& currentRoom, int count)
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

		// unequip all like-items
		for (InventorySlot& _slot : Slots)
		{
			if (itemName == _slot.GetName())
				continue;
			else if (_slot.GetItemType() == itemType)
				_slot.IsEquiped = false;
		}
	}




json Player::Serialize()
{
	json j;
	j["RoomLocation"] = RoomLocation.Serialize();
	j["Inventory"] = Inventory.Serialize();
	return j;
}

Player* Player::Deserialize(json playerData)
{
	Player* p = new Player();

	p->JumpToRoom(*Vector2::Deserialize(playerData["RoomLocation"]));
	p->Inventory = *PlayerInventory::Deserialize(playerData["Inventory"]);

	return p;
}

Player::Player()
{
	RoomLocation = Vector2();
	CurrentRoom = nullptr;
	Inventory = PlayerInventory();
	Inventory.AddItem(melee_Unarmed, 1, true);
	Inventory.EquipItem(melee_Unarmed.GetName(), true);
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
		Inventory.RemoveItem(keys->GetName(), *GetRoom());
		switch (dir)
		{
		case North:
			AdjacentRoom(Vector2(0, 1))->UnlockDoor(South);
			break;
		case East:
			AdjacentRoom(Vector2(1, 0))->UnlockDoor(West);
			break;
		case South:
			AdjacentRoom(Vector2(0, -1))->UnlockDoor(North);
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

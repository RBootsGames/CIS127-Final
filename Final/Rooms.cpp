#include "Rooms.h"

#include <vector>
#include <regex>
#include "extensions.h"
#include "inventory.h"
#include "Items.h"
#include "Player.h"
#include "commands.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;


//static unsigned int RoomIDIterator;
json Room::Serialize()
{
	json j;
	// Items
	vector<json> s_items;

	for (InventorySlot _slot : Items)
		s_items.push_back(_slot.Serialize());

	j["Items"] = s_items;

	// Doors
	j["UnlockedDoors"] = UnlockedDoors;
	j["LockedDoors"] = LockedDoors;

	// Extra info
	j["RoomID"] = RoomID;
	j["PlayerVisited"] = playerVisited;
	j["IsPlayerIn"] = isPlayerIn;

	return j;
}

Room* Room::Deserialize(json data)
{
	Room* room = new Room();

	// Items
	for (json item : data["Items"])
		room->Items.push_back(*InventorySlot::Deserialize(item));

	// Doors
	//room->UnlockedDoors = (vector<Doors>)data["UnlockedDoors"];
	//room->LockedDoors = (vector<Doors>)data["LockedDoors"];
	for (Doors door : data["UnlockedDoors"])
		room->UnlockedDoors.push_back((Doors)door);
	for (Doors door : data["LockedDoors"])
		room->LockedDoors.push_back((Doors)door);
	
	// Extra info
	room->RoomID = (int)data["RoomID"];
	room->playerVisited = (bool)data["PlayerVisited"];
	room->isPlayerIn = (bool)data["IsPlayerIn"];

	return room;
}


Room::Room(vector<Doors> unlockedDoors, vector<Doors> lockedDoors)
	 :Room(vector<InventorySlot>(), unlockedDoors, lockedDoors) {}

Room::Room(vector<InventorySlot> items,
		   vector<Doors> unlockedDoors,
		   vector<Doors> lockedDoors)
{
	playerVisited = false;
	isPlayerIn = false;
	RoomID = RoomIDIterator;
	RoomIDIterator++;

	this->Items = items;
	UnlockedDoors = unlockedDoors;
	LockedDoors = lockedDoors;
}
bool Room::operator==(const Room& rhs) const
{
	return RoomID == rhs.RoomID;
}

void Room::AddItem(Item& item, int count)
{
	// check for existing items
	for (InventorySlot& _item : Items)
	{
		if (item.GetName() == _item.GetName())
		{
			_item.NumberInStack += count;
			return;
		}
	}

	// create inventory slot for new items
	Items.push_back(InventorySlot(item, count));
}
void Room::AddItem(MeleeWeapon& item, int count)
{
	// check for existing items
	for (InventorySlot& _item : Items)
	{
		if (item.GetName() == _item.GetName())
		{
			_item.NumberInStack += count;
			return;
		}
	}

	// create inventory slot for new items
	Items.push_back(InventorySlot(item, count));
}
void Room::AddItem(RangedWeapon& item, int count)
{
	// check for existing items
	for (InventorySlot& _item : Items)
	{
		if (item.GetName() == _item.GetName())
		{
			_item.NumberInStack += count;
			return;
		}
	}

	// create inventory slot for new items
	Items.push_back(InventorySlot(item, count));
}
void Room::AddItem(InventorySlot item, int count)
{
	switch (item.GetItemType())
	{
	case TItem:
		AddItem(item.GetItemAsItem(), count);
		break;
	case TMelee:
		AddItem(item.GetItemAsMelee(), count);
		break;
	case TRanged:
		AddItem(item.GetItemAsRanged(), count);
		break;
	}
}

void Room::EnterRoom()
{
	playerVisited = true;
	isPlayerIn = true;
}
void Room::ExitRoom() { isPlayerIn = false; }


string Room::DoorToString(Doors door, bool lowercase)
{
	switch (door)
	{
	case North:
		return (!lowercase) ? "North" : "north";
	case East:
		return (!lowercase) ? "East" : "east";
	case South:
		return (!lowercase) ? "South" : "south";
	case West:
		return (!lowercase) ? "West" : "west";
	default:
		return "";
	}
}


vector<Doors>& Room::GetUnlockedDoors()
{
	return UnlockedDoors;
}
vector<Doors>& Room::GetLockedDoors()
{
	return LockedDoors;
}
vector<Doors> Room::GetAllDoors()
{
	vector<Doors> allDoors = UnlockedDoors;
	for (Doors _door : LockedDoors)
	{
		if (!(count(UnlockedDoors.begin(), UnlockedDoors.end(), _door)))
			allDoors.push_back(_door);
	}
	return allDoors;
}

string Room::UnlockDoor(Doors door)
{
	auto match = find(LockedDoors.begin(), LockedDoors.end(), door);

	if (match != LockedDoors.end())
	{
		LockedDoors.erase(match);
		UnlockedDoors.push_back(door);
		return "Unlocked the " + DoorToString(door) + " door.";
	}

	return "That door is not locked.";
}

bool Room::RemoveItem(InventorySlot& foundItem, string itemName, int itemCount)
{
	bool itemFound = false;
	for (int i = 0; i < Items.size(); i++)
	{
		InventorySlot& _item = Items[i];

		if (itemName == ToLower(_item.GetName()) ||
			itemName == ToLower(_item.GetName()) + "s" ||
			itemName == ToLower(_item.GetName()) + "es")
		{

			if (itemCount > _item.NumberInStack)
				itemCount = _item.NumberInStack;

			_item.NumberInStack -= itemCount;

			switch (_item.GetItemType())
			{
			case TItem:
				foundItem = InventorySlot(_item.GetItemAsItem(), itemCount);
				itemFound = true;
				break;
			case TMelee:
				foundItem = InventorySlot(_item.GetItemAsMelee(), itemCount);
				itemFound = true;
				break;
			case TRanged:
				foundItem = InventorySlot(_item.GetItemAsRanged(), itemCount);
				itemFound = true;
				break;
			}

			// remove item if it was the last one
			if (_item.NumberInStack <= 0)
				Items.erase(Items.begin() + i);

			return itemFound;
		}
	}

	return false;
}

string Room::PrintItems()
{
	string description = "This room contains ";

	string name;
	int count;

	if (Items.size() == 0) // no items
		description = "This room is empty.";
	else if (Items.size() == 1) // only one item
	{
		name = Items[0].GetName();
		if (Items[0].NumberInStack == 1)
		{
			description += WhenToUseAn(name) + ".";
		}
		else
			description += to_string(Items[0].NumberInStack) + " " + name + "s.";
	}
	else
	{
		for (int i = 0; i < Items.size(); i++)
		{
			InventorySlot _item = Items[i];
			name = _item.GetName();
			count = _item.NumberInStack;

			if (i != Items.size() - 1)
			{
				if (_item.NumberInStack == 1)
				{
					description += WhenToUseAn(name) + ", ";
				}
				else
					description += to_string(count) + " " + name + "s, ";
			}
			else // only for the last item in the list
			{
				description.pop_back();
				description.pop_back();

				if (_item.NumberInStack == 1)
					description += " and " + WhenToUseAn(name) + ".";
				else
					description += " and " + to_string(count) + " " + name + "s.";
			}
		}
	}

	return description;
	//Print(description);
}

string Room::PrintDoors()
{
	string message = "There are doors on the";
	vector<Doors> allDoors = GetAllDoors();
	vector<Doors> lockedDoors = GetLockedDoors();

	if (allDoors.size() == 0)
		return "There are no doors in this room.";

	for (int i = 0; i < allDoors.size(); i++)
	{
		Doors _door = allDoors[i];
		switch (_door)
		{
		case North:
			message += " north,";
			break;
		case East:
			message += " east,";
			break;
		case South:
			message += " south,";
			break;
		case West:
			message += " west,";
			break;
		}

		// on second to last door
		if (i == allDoors.size() - 2)
		{
			message.pop_back();

			message += " and";
		}
		// only last door
		else if (i == allDoors.size() - 1)
		{
			message.pop_back();
			// if multiple doors
			if (allDoors.size() > 1)
				message += " walls.";
			else
			{
				message += " wall.";
				message = regex_replace(message, regex("are doors"), "is a door");
			}
		}
	}
	// get locked doors
	if (lockedDoors.size() > 0)
	{
		message.pop_back();
		message += ",\nbut there's a lock on the";

		for (int i = 0; i < lockedDoors.size(); i++)
		{
			Doors _door = lockedDoors[i];
			switch (_door)
			{
			case North:
				message += " north,";
				break;
			case East:
				message += " east,";
				break;
			case South:
				message += " south,";
				break;
			case West:
				message += " west,";
				break;
			}

			// on second to last door
			if (i == lockedDoors.size() - 2)
			{
				message.pop_back();
				message += " and";
			}
			// only last door
			else if (i == lockedDoors.size() - 1)
			{
				message.pop_back();
				// if multiple doors
				if (lockedDoors.size() > 1)
					message += " doors.";
				else
				{
					message += " door.";
				}
			}
		}
	}

	return message;
}

void Room::GetItemCounts(int& items, int& weapons)
{
	items = 0;
	weapons = 0;
	if (Items.size() == 0) // no items
		return;

	for (InventorySlot item : Items)
	{
		if (item.GetItemType() == TItem)
			items++;
		else if (item.GetItemType() == TMelee || item.GetItemType() == TRanged)
			weapons++;
	}
}

/// @brief Returns a 5 long array of text to display the room on a map.
string* Room::PrintRoom(char _item, char _weapon, char _player)
{
	// detect if this is the dummy room
	if (/*!playerVisited ||*/ GetAllDoors().size() == 0)
	{
		string* temp = new string[5];
		temp[0] = "           "; // 11
		temp[1] = "           ";
		temp[2] = "           ";
		temp[3] = "           ";
		temp[4] = "           ";
		return temp;
	}


	string* lines = new string[5];
	// walls
	char w_hor = char(205); // ═
	char w_ver = char(186); // ║

	// corners
	char tl = char(201); // ╔
	char tr = char(187); // ╗
	char br = char(188); // ╝
	char bl = char(200); // ╚

	lines[0] = tl + string(9, w_hor) + tr;
	lines[1] = w_ver + string(9, ' ') + w_ver;
	lines[2] = w_ver + string(9, ' ') + w_ver;
	lines[3] = w_ver + string(9, ' ') + w_ver;
	lines[4] = bl + string(9, w_hor) + br;

	// doors
	char* d_top = &lines[0][5]; // ═
	char* d_rig = &lines[2][10]; // ║
	char* d_bot = &lines[4][5]; // ═
	char* d_lef = &lines[2][0]; // ║

	for (Doors unlocked : UnlockedDoors)
	{
		switch (unlocked)
		{
		case North:
			*d_top = ' ';
			break;
		case East:
			*d_rig = ' ';
			break;
		case South:
			*d_bot = ' ';
			break;
		case West:
			*d_lef = ' ';
			break;
		}
	}

	for (Doors locked : LockedDoors)
	{
		switch (locked)
		{
		case North:
			*d_top = 'x';
			break;
		case East:
			*d_rig = 'X';
			break;
		case South:
			*d_bot = 'x';
			break;
		case West:
			*d_lef = 'X';
			break;
		}
	}


	// add extras to room
	char* icon_player = &lines[2][5];
	char* icon_weapon = &lines[1][7];
	char* icon_item = &lines[1][2];
	if (playerVisited)
	{
		if (isPlayerIn)
			*icon_player = _player;

		int itemCount, weaponCount;
		GetItemCounts(itemCount, weaponCount);

		if (itemCount > 0)
			*icon_item = _item;
		if (weaponCount > 0)
			*icon_weapon = _weapon;
	}
	else
		*icon_player = '?';

	return lines;

}

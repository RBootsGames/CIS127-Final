#include "Level.h"

#include "Rooms.h"
#include "extensions.h"
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;


json Level::Serialize()
{
	vector<vector<json>> s_Level;

	for (int x = 0; x < FullLevel.size(); x++)
	{
		vector<json> temp;
		for (int y = 0; y < FullLevel[x].size(); y++)
		{
			temp.push_back(FullLevel[x][y].Serialize());
		}
		s_Level.push_back(temp);
	}

	return s_Level;
}

Level* Level::Deserialize(json data)
{
	vector<vector<Room>> fullLevel;
	
	for (int x = 0; x <  data.size(); x++)
	{
		vector<Room> temp;
		for (int y = 0; y < data[x].size(); y++)
		{

			temp.push_back(*Room::Deserialize(data[x][y]));
		}
		fullLevel.push_back(temp);
	}

	return new Level(fullLevel);
}


Level::Level(vector<vector<Room>> fullLevel)
{
	FullLevel = fullLevel;
}

void Level::PrintMap()
{
	const int roomHeight = 5;
	int mapYValue = 0;
	vector<string> lines;
	//string singleLine = "";


	// icon key
	char _item = char(15);
	char _weapon = char(19);
	char _player = char(12);

	Print(cjust("Icon Key", 25), true, true);
	Print(ljust("  Player: ", 18), false);
	Print(_player);
	Print(ljust("  Items: ", 18), false);
	Print(_item);
	Print(ljust("  Weapons: ", 18), false);
	Print(_weapon);
	Print(ljust("  Locked Doors: ", 18), false);
	Print('x');

	for (int y = 0; y < FullLevel.size(); y++) {
		// add enough lines to accommodate for each room height
		lines.insert(lines.end(), { "", "", "", "", "" });
		//singleLine = "";
		for (int x = 0; x < FullLevel[y].size(); x++)
		{
			string* room = FullLevel[y][x].PrintRoom(_item, _weapon, _player);
			for (int i = 0; i < 5; i++)
			{
				lines[(mapYValue * roomHeight) + i] += room[i];
				//Print(room[i]);
			}
		}

		mapYValue++;
	}

	for (string line : lines)
		Print(line);
}

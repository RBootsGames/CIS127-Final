#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include "Rooms.h"
#include <vector>

class Level
{
private:

public:
	vector<vector<Room>> FullLevel;
	Level(vector<vector<Room>> fullLevel)
	{
		FullLevel = fullLevel;
	}

	void PrintMap()
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
};


#endif // !LEVEL_H

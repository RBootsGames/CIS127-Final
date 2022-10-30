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
};


#endif // !LEVEL_H

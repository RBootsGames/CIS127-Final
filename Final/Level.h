#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include "Rooms.h"
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Level
{
public:
	vector<vector<Room>> FullLevel;

	json Serialize();
	static Level* Deserialize(json data);


	Level(vector<vector<Room>> fullLevel);

	void PrintMap();
};


#endif // !LEVEL_H

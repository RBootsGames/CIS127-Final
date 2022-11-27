#include "objects.h"

#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
//using namespace std;

//enum TItemType
//{
//	None, TItem, TMelee, TRanged
//};



json Vector2::Serialize()
{
	json j;
	j["X"] = X;
	j["Y"] = Y;
	return j;
}

Vector2* Vector2::Deserialize(json data)
{
	return new Vector2(data["X"], data["Y"]);
}

Vector2::Vector2()
{
	X = 0;
	Y = 0;
}
Vector2::Vector2(int x, int y)
{
	X = x;
	Y = y;
}

bool Vector2::operator==(const Vector2& compare) const
{
	return (X == compare.X) && (Y == compare.Y);
}

Vector2 Vector2::operator+(const Vector2& add) const
{
	return Vector2(X + add.X, Y + add.Y);
}


TableRow::TableRow(std::string text, int spacing, bool isEquiped)
{
	Text = text;
	Spacing = spacing;
	IsEquiped = isEquiped;
}

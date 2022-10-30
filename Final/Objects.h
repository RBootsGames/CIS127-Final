#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
using namespace std;

struct Vector2
{
	int X, Y;
	Vector2()
	{
		X = 0;
		Y = 0;
	}
	Vector2(int x, int y)
	{
		X = x;
		Y = y;
	}

	bool operator==(const Vector2& compare) const
	{
		return (X == compare.X) && (Y == compare.Y);
	}
};

struct TableRow
{
	string Text;
	int Spacing;
	bool IsEquiped;

	TableRow(string text, int spacing, bool isEquiped=false)
	{
		Text = text;
		Spacing = spacing;
		IsEquiped = isEquiped;
	}
};
#endif

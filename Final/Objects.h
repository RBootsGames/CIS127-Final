#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
using namespace std;

struct Size
{
	int X, Y;
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

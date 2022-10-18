#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
using namespace std;

struct Size
{
	int X, Y;
};

using ComFunc = string(*)(string);
class Command
{

	public:
		string Key;
		string HelpMessage;
		ComFunc Execute;

		Command(){}
		Command(string key, string helpMessage, ComFunc executionCommand)
		{
			Key = key;
			HelpMessage = helpMessage;
			Execute = executionCommand;
		}
};
#endif

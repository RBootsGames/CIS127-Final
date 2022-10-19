#ifndef COMMANDOBJECT_H
#define COMMANDOBJECT_H

#include <string>
#include <iostream>
#include <vector>
using namespace std;

using ComFunc = string(*)(vector<string>);
class Command
{
private:
	bool Enabled;
	string DisabledReason;
	string Key;
	string HelpMessage;
	ComFunc TheCommand;


public:
	Command() { }
	Command(string key, string helpMessage, ComFunc executionCommand, bool enabled = true, string disabledReason = "")
	{
		Key = key;
		HelpMessage = helpMessage;
		TheCommand = executionCommand;
		Enabled = enabled;
		DisabledReason = disabledReason;
	}

	string Execute(vector<string> args)
	{
		if (Enabled)
			return TheCommand(args);
		else
			return DisabledReason;
	}

	string GetHelp()
	{
		return HelpMessage;
	}

	string GetKey()
	{
		return Key;
	}

	bool IsEnabled() { return Enabled; }

	void SetEnabled(bool value, string reason)
	{
		Enabled = value;
		if (value)
			DisabledReason = "";
		else
			DisabledReason = reason;
	}
};


#endif
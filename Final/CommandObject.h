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
	bool SkipHelpPrint;

	Command();
	Command(string key, string helpMessage, ComFunc executionCommand, bool skipHelpPrint = false, bool enabled = true, string disabledReason = "");

	string Execute(vector<string> args);

	string GetHelp();

	string GetKey();

	bool IsEnabled();

	void SetEnabled(bool value, string reason="");
};


#endif
#include "CommandObject.h"

#include <string>
#include <iostream>
#include <vector>
using namespace std;

using ComFunc = string(*)(vector<string>);
Command::Command() {}
Command::Command(string key, string helpMessage, ComFunc executionCommand, bool skipHelpPrint, bool enabled, string disabledReason)
{
	Key = key;
	SkipHelpPrint = skipHelpPrint;
	HelpMessage = helpMessage;
	TheCommand = executionCommand;
	Enabled = enabled;
	DisabledReason = disabledReason;
}

string Command::Execute(vector<string> args)
{
	if (Enabled)
		return TheCommand(args);
	else
		return DisabledReason;
}

string Command::GetHelp()
{
	return HelpMessage;
}

string Command::GetKey()
{
	return Key;
}

bool Command::IsEnabled() { return Enabled; }

void Command::SetEnabled(bool value, string reason)
{
	Enabled = value;
	if (value)
		DisabledReason = "";
	else
		DisabledReason = reason;
}

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include "extensions.h"
#include "Objects.h"

using namespace std;
using ComFunc = string(*)(string);

// ########## VARIABLES ##########

extern bool exitProgram;

// ########## COMMANDS ##########

string ClearConsole(string args)
{
    Point p = GetConsoleSize();
    cout << string(p.Y, '\n');
    return "";
}

string Test1(string args)
{
    return "This is a test command...";
}
string GetSize(string args)
{
    GetConsoleSize();
    return "";
}
string ExitGame(string args)
{
    exitProgram = true;
    return "";
}
string Loop(string args)
{
    try
    {
        int count = stoi(args);
        string temp = "";

        for (int i = 0; i < count; i++)
            temp += to_string(i) + "\n";

        return temp;
    }
    catch (const std::exception&)
    {
        return args + " needs to be an integer.";
    }
}

// ########## COMMAND DICTIONARY ##########
map<string, ComFunc> ComLibrary =
{
    {"test", Test1},
    {"clear", ClearConsole},
    {"loop", Loop},
    {"exit", ExitGame},
};

void SeparateCommandArgs(string text, string& command, string& args)
{
    bool findingCommand = true;
    command = "";

    for (char& c : text)
    {
        if (findingCommand)
        {
            if (c == ' ')
            {
                findingCommand = false;
                continue;
            }

            command += c;
        }
        else
            args += c;
    }
}

void RunCommand(string command)
{
    string args = "";
    SeparateCommandArgs(command, command, args);

    if (ComLibrary.find(command) == ComLibrary.end())
    {
        Print(command + " does not exist");
    }
    else
    {
        ComFunc execution = ComLibrary[command];
        Print(execution(args));
    }
}

#endif
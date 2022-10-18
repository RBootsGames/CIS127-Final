#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include "extensions.h"
#include "Objects.h"

using namespace std;
//using ComFunc = string(*)(string);

// ########## VARIABLES ##########

extern bool exitProgram;
map<string, Command> ComLibrary;

// ########## COMMANDS ##########

string ClearConsole(string args)
{
    Size p = GetConsoleSize();
    cout << string(p.Y, '\n');
    return "";
}


string Test1(string args)
{
    if (args =="/?")
        return "This just prints a test command.";

    return "This is a test command...";
}
string GetSize(string args)
{
    GetConsoleSize();
    return "";
}
string ExitGame(string args)
{
    if (args == "/?")
        return "Closes out of the game.";

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
        return "Loop needs to be run with an integer.";
    }
}

// ########## COMMAND DICTIONARY ##########
//map<string, Command> ComLibrary =
//{
//    {"test", Command("test", "a test", Test1)}
//};
//map<string, ComFunc> ComLibrary =
//{
//    //{"help", Help},
//    {"test", Test1},
//    {"clear", ClearConsole},
//    {"loop", Loop},
//    {"exit", ExitGame},
//};

string Help(string args)
{
    cout << endl;
    for (auto const& key : ComLibrary)
    {
        cout << right << setw(12) << key.first << " | " << key.second.HelpMessage << endl;
    }
    return "";
}


void SeparateCommandArgs(string text, string& command, vector<string>& args)
{
    bool findingCommand = true;
    string tempArg = "";
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
        {
            if (c == ' ')
            {
                args.push_back(tempArg);
                tempArg = "";
                continue;
            }
            tempArg += c;
        }
    }

    //if (tempArg != "")
    args.push_back(tempArg);
}

void RunCommand(string command)
{
    vector<string> args;
    SeparateCommandArgs(command, command, args);

    if (ComLibrary.find(command) == ComLibrary.end())
    {
        Print(command + " does not exist");
    }
    else
    {
        ComFunc execution = ComLibrary[command].Execute;
        Print(execution(args[0]));
    }
}


void AddCommand(Command comm)
{
    ComLibrary.insert({ comm.Key, comm });
}

void InitializeCommands()
{
    AddCommand(Command(
        "test",
        "Displays a test message.",
        Test1));

    AddCommand(Command(
        "exit",
        "Exits the game.",
        ExitGame));

    AddCommand(Command(
        "help",
        "Lists all possible commands.",
        Help));

    AddCommand(Command(
        "loop",
        "Counts up from 0 to the number specified.",
        Loop));
}

#endif
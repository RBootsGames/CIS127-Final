#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>

#include "extensions.h"
#include "objects.h"
#include "CommandObject.h"
#include "Inventory.h"

using namespace std;
// console colors: https://stackoverflow.com/questions/24281603/c-underline-output

// ########## VARIABLES ##########

extern bool exitProgram;
map<string, Command> ComLibrary;

// ########## COMMANDS ##########

string ClearConsole()
{
    Size p = GetConsoleSize();
    cout << string(p.Y, '\n');
    return "";
}
string ClearConsole(vector<string> args)
{
    Size p = GetConsoleSize();
    cout << string(p.Y, '\n');
    return "";
}


string Test1(vector<string> args)
{
    return "This is a test command...";
}
string Test2(vector<string> args)
{
    return "Another test with a space.";
}
string GetSize(vector<string> args)
{
    GetConsoleSize();
    return "";
}
string ExitGame(vector<string> args)
{
    exitProgram = true;
    return "";
}
string Loop(vector<string> args)
{
    if (args.size() > 0)
    {
        try
        {
            int count = stoi(args[0]);
            string temp = "";

            for (int i = 0; i < count; i++)
                temp += to_string(i) + "\n";

            return temp;
        }
        catch (const std::exception&) { }
    }

    return "Loop needs to be run with an integer.";
}
string Say(vector<string> args)
{
    string temp = "    ";
    for (string word : args)
    {
        temp += word + " ";
    }
    return rtrim(temp);
}
string Toggle(vector<string> args)
{
    for (string word : args)
    {
        if (word == "toggle")
            continue;

        if (!(ComLibrary.find(word) == ComLibrary.end()))
        {
            ComLibrary[word].SetEnabled(!ComLibrary[word].IsEnabled(), "This was disabled by a command.");

            if (ComLibrary[word].IsEnabled())
                Print("Enabled " + word + " command.");
            else
                Print("Disabled " + word + " command.");
        }
    }

    return "";
}
string Inventory(vector<string> args)
{
    string message = "";
    if (Sword)
        message += " Sword";
    if (Treasure)
        message += " Treasure";

    if (message == "")
        message = "You have nothing in your inventory.";

    return ltrim(message);
}
string PickUp(vector<string> args)
{
    if (args.size() == 0)
        return "What do you want to pick up?";

    else if (args[0] == "sword")
    {
        if (Sword == false)
        {
            Sword = true;
            return "You picked up the sword.";
        }
        else
            return "You already have the sword.";
    }
    else if (args[0] == "treasure")
    {
        if (Treasure == false)
        {
            Treasure = true;
            return "You picked up the treasure.";
        }
        else
            return "You already have the treasure.";
    }
    return "I can't pick that up";
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

string Help(vector<string> args)
{
    cout << endl;
    for (auto const& key : ComLibrary)
    {
        Command comm = key.second;
        
        if (comm.IsEnabled())
            cout << right << setw(15) << key.first << " | " << comm.GetHelp() << endl;
    }
    return "";
}


vector<string> SeparateCommandArgs(string text)
{
    bool findingCommand = true;
    string tempArg = "";
    vector<string> args;
    //command = "";

    for (char& c : text)
    {
        //if (findingCommand)
        //{
        //    if (c == ' ')
        //    {
        //        findingCommand = false;
        //        continue;
        //    }

        //    command += c;
        //}
        //else
        //{
        if (c == ' ')
        {
            if (tempArg != "")
                args.push_back(tempArg);
            tempArg = "";
            continue;
        }
        tempArg += c;
        //}
    }

    //if (tempArg != "")
    args.push_back(tempArg);

    return args;
}

void RunCommand(string command)
{
    vector<string> args = SeparateCommandArgs(command);
    bool commandFound = false;

    // add words together until a match is found
    string justCommand = "";
    //string tempCommand = "";
    for (int i = 0; i < args.size(); i++)
    {
        justCommand += " " + args[i];
        justCommand = trim(justCommand);

        // check for a command match
        if (!(ComLibrary.find(justCommand) == ComLibrary.end()))
        {
            commandFound = true;

            for (int del = 0; del <= i; del++)
            {
                args.erase(args.begin());
            }
            break;
        }
    }


    if (commandFound)
    {
        Command comm = ComLibrary[justCommand];
        Print(comm.Execute(args));
    }
    else
        Print(command + " does not exist");
}


void AddCommand(Command comm)
{
    ComLibrary.insert({ comm.GetKey(), comm});
}

void InitializeCommands()
{
    AddCommand(Command(
        "clear",
        "Clears the window.",
        ClearConsole));

    AddCommand(Command(
        "test",
        "Displays a test message.",
        Test1));

    AddCommand(Command(
        "another test",
        "Accept a command with a space in it.",
        Test2));

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

    AddCommand(Command(
        "say",
        "Useful for talking to people.",
        Say));

    AddCommand(Command(
        "toggle",
        "This will enable or disable other commands.",
        Toggle));

    AddCommand(Command(
        "inventory",
        "Check your inventory.",
        Inventory));

    AddCommand(Command(
        "pick up",
        "Used to pick up items.",
        PickUp));
}

#endif
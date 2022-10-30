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
#include "inventory.h"
#include "Player.h"

using namespace std;
// console colors: https://stackoverflow.com/questions/24281603/c-underline-output

// ########## VARIABLES ##########

extern Player player;
extern bool exitProgram;
map<string, Command> ComLibrary;

// ########## COMMANDS ##########

string ClearConsole()
{
    Vector2 p = GetConsoleSize();
    cout << string(p.Y, '\n');
    return "";
}
string ClearConsole(vector<string> args)
{
    Vector2 p = GetConsoleSize();
    cout << string(p.Y, '\n');
    return "";
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
    player.Inventory.PrintInventory();
    return "";
}
string PickUp(vector<string> args)
{
    if (args.size() == 0)
        return "What do you want to pick up?";
    else
    {
        string itemName = ArgsToString(args);
        InventorySlot pickedUpItem;
        bool success = player.CurrentRoom.RemoveItem(pickedUpItem, itemName);

        if (!success)
            return "I can't pick up " + itemName + ".";

        player.Inventory.AddItem(pickedUpItem);
        return "";
    }
}
string DropItem(vector<string> args)
{
    return "";
}
string SearchRoom(vector<string> args)
{
    string message = "\n";
    message += player.CurrentRoom.PrintItems() + '\n';
    message += player.CurrentRoom.PrintDoors();
    return message;
}
string EquipItem(vector<string> args)
{
    player.Inventory.EquipItem(ArgsToString(args));
    return "";
}
string Move(vector<string> args)
{
    if (args.size() == 0)
        return "You need to specify which direction you want to go? (north, east, south, west)";

    return player.EnterRoom(args[0]);
}

string Help(vector<string> args)
{
    cout << endl;
    for (auto const& key : ComLibrary)
    {
        Command comm = key.second;
        
        if (comm.IsEnabled() && !comm.SkipHelpPrint)
            cout << right << setw(15) << key.first << " | " << comm.GetHelp() << endl;
    }
    return "";
}


vector<string> SeparateCommandArgs(string text)
{
    bool findingCommand = true;
    string tempArg = "";
    vector<string> args;

    for (char& c : text)
    {
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
    if (command == "")
        return;

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

            args.erase(args.begin(), args.begin() + i+1);
            // old version was working
            //for (int del = 0; del <= i; del++)
            //{
            //    args.erase(args.begin());
            //}
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
        "exit",
        "Exits the game.",
        ExitGame));

    AddCommand(Command(
        "help",
        "Lists all possible commands.",
        Help));

    AddCommand(Command(
        "say",
        "Useful for talking to people.",
        Say));

    AddCommand(Command(
        "inventory",
        "Check your inventory.",
        Inventory));

    AddCommand(Command(
        "pick up",
        "Used to pick up items.",
        PickUp));

    AddCommand(Command(
        "pickup",
        "Used to pick up items.",
        PickUp, true));

    AddCommand(Command(
        "search",
        "Searches the current room you are in.",
        SearchRoom));

    AddCommand(Command(
        "equip",
        "Used to equip a weapon in your inventory.",
        EquipItem));

    AddCommand(Command(
        "move",
        "Moves through a door in a specific direction. (north, east, south, west)",
        Move));
}

#endif
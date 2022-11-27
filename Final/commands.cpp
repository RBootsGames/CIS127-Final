#include "commands.h"

#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>

#include "extensions.h"
#include "objects.h"
#include "CommandObject.h"
#include "inventory.h"
#include "Player.h"
#include "objects.h"
#include "GameDatabase.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;
// console colors: https://stackoverflow.com/questions/24281603/c-underline-output

// ########## VARIABLES ##########

string saveFilePath = "save.json";
//Player player;
//bool exitProgram = false;
map<string, Command> ComLibrary;

/*Doors StringToDirection(string text)
{
    Doors dir = Door_None;

    if (text == "north")
        dir = North;
    else if (text == "east")
        dir = East;
    else if (text == "south")
        dir = South;
    else if (text == "west")
        dir = West;

    return dir;
}
*/

// ########## COMMANDS ##########

//void SetExitProgram(bool value) { exitProgram = value; }
//bool GetExitProgram() { return exitProgram; }

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
        bool success = player.GetRoom()->RemoveItem(pickedUpItem, itemName);

        if (!success)
            return "I can't pick up " + itemName + ".";

        int returned = player.Inventory.AddItem(pickedUpItem);

        if (returned > 0)
            player.GetRoom()->AddItem(pickedUpItem, returned);
        return "";
    }
}
string DropItem(vector<string> args)
{
    if (args.size() == 0)
        return "What do you want to drop?";
    else
    {
        // get number
        int count = 100;

        if (StringToInt(args.front(), count))
            args.erase(args.begin());
        else if (StringToInt(args.back(), count))
            args.pop_back();

        string itemName = ArgsToString(args);


        player.Inventory.DropItem(itemName, *player.GetRoom(), count);
        return "";
    }
}
string SearchRoom(vector<string> args)
{
    string message = "\n";
    message += player.GetRoom()->PrintItems() + '\n';
    message += player.GetRoom()->PrintDoors();
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
string MoveNorth(vector<string> args)
{
    return player.EnterRoom("north");
}
string MoveEast(vector<string> args)
{
    return player.EnterRoom("east");
}
string MoveSouth(vector<string> args)
{
    return player.EnterRoom("south");
}
string MoveWest(vector<string> args)
{
    return player.EnterRoom("west");
}
string PrintMap(vector<string> args)
{
    Level1.PrintMap();
    return "";
}
string UnlockDoor(vector<string> args)
{
    if (args.size() < 1)
        return "Specify the direction of the door you want to unlock.";

    string message = player.UnlockDoor(args.front());
    return message;
}
string Save(vector<string> args)
{
    json data;

    data["Level"] = Level1.Serialize();
    data["Player"] = player.Serialize();

    ofstream save(saveFilePath);
    save << setw(4) << data << endl;
    save.close();

    return "Saved";
}
string Load(vector<string> args)
{
    ifstream saveFile(saveFilePath);
    if (saveFile.fail())
        return "No save file found.";
    json data = json::parse(saveFile);

    player = *Player::Deserialize(data["Player"]);
    Level1 = *Level::Deserialize(data["Level"]);

    return "data loaded";
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

            args.erase(args.begin(), args.begin() + i + 1);
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


void AddCommands(vector<Command> comms)
{
    for (Command comm : comms)
        ComLibrary.insert({ comm.GetKey(), comm });
}

void InitializeCommands()
{
    AddCommands({
        Command(
            "clear",
            "Clears the window.",
            ClearConsole),

        Command(
            "exit",
            "Exits the game.",
            ExitGame),

        Command(
            "help",
            "Lists all possible commands.",
            Help),

        Command(
            "say",
            "Useful for talking to people.",
            Say, true),

        Command(
            "inventory",
            "Check your inventory.",
            Inventory),

        Command(
            "pick up",
            "Used to pick up items.",
            PickUp),
        Command(
            "pickup",
            "Used to pick up items.",
            PickUp, true),

        Command(
            "drop",
            "Used to drop items.",
            DropItem),

        Command(
            "search",
            "Searches the current room you are in.",
            SearchRoom),

        Command(
            "unlock",
            "Consumes a key and unlocks a door based on the direction given.",
            UnlockDoor),

        Command(
            "equip",
            "Used to equip a weapon in your inventory.",
            EquipItem),

        Command(
            "move",
            "Moves through a door in a direction. You can also just use directions. (north, east, south, west)",
            Move),
        Command(
            "north",
            "Moves through a door in a specific direction. (north, east, south, west)",
            MoveNorth, true),
        Command(
            "east",
            "Moves through a door in a specific direction. (north, east, south, west)",
            MoveEast, true),
        Command(
            "south",
            "Moves through a door in a specific direction. (north, east, south, west)",
            MoveSouth, true),
        Command(
            "west",
            "Moves through a door in a specific direction. (north, east, south, west)",
            MoveWest, true),

        Command(
            "map",
            "Shows a map of the level.",
            PrintMap),

        Command(
            "save",
            "Saves the game.",
            Save),

        Command(
            "load",
            "Loads the game.",
            Load)
        });
}

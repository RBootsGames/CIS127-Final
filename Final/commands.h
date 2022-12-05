#ifndef COMMANDS_H
#define COMMANDS_H

#include <map>
#include <vector>

#include "CommandObject.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;
// console colors: https://stackoverflow.com/questions/24281603/c-underline-output

// ########## VARIABLES ##########

extern string saveFilePath;
//extern Player player;
//extern bool exitProgram;
extern map<string, Command> ComLibrary;


// ########## COMMANDS ##########

//void SetExitProgram(bool value);
//bool GetExitProgram();

string ClearConsole();
string ClearConsole(vector<string> args);


string GetSize(vector<string> args);
string ExitGame(vector<string> args);
string Say(vector<string> args);
string Toggle(vector<string> args);
string Inventory(vector<string> args);
string PickUp(vector<string> args);
string DropItem(vector<string> args);
string SearchRoom(vector<string> args);
string EquipItem(vector<string> args);
string Move(vector<string> args);
string MoveNorth(vector<string> args);
string MoveEast(vector<string> args);
string MoveSouth(vector<string> args);
string MoveWest(vector<string> args);
string PrintMap(vector<string> args);
string UnlockDoor(vector<string> args);
string Save(vector<string> args);
string Load(vector<string> args);

string Help(vector<string> args);


vector<string> SeparateCommandArgs(string text);

void RunCommand(string command);


void AddCommands(vector<Command> comms);

void InitializeCommands();

/// @brief Disables all commands except Load, Exit and Help.
void DisableAllCommands(string reason="");
void EnableAllCommands();
#endif
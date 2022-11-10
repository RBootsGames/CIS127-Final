/* Ross Boetsma
*  CIS 127 - 2102
*  Description:
*/

// https://askcodes.net/coding/what-is-a-c++-delegate-
// option 3

#include <iostream>
//#include <iomanip>
#include <string>
//#include <map>
#include <conio.h>
#include "extensions.h"
#include "commands.h"
#include "inventory.h"
#include "GameDatabase.h"
#include "Player.h"
//#include "Items.h"
//#include "Objects.h"

#include <vector>
using namespace std;

Player player;
bool exitProgram = false;




int main()
{
    /*PlayerInventory inventory;
    inventory.AddItem(Unarmed, 1, true);
    inventory.EquipItem("unarmed", true);
    

    inventory.AddItem(HealthPotion, 1);
    inventory.AddItem(HealthPotion, 5);
    inventory.AddItem(ImportantSword);
    inventory.PrintInventory();*/

    //player.Inventory.AddItem(item_HealthPotion,3);
    //player.Inventory.AddItem(item_GoldKey,2);
    //player.Inventory.EquipItem("Unarmed");
    //player.Inventory.AddItem(melee_ImportantSword);
    //player.Inventory.AddItem(melee_CurvedSword);
    //player.Inventory.PrintInventory();
    
    player.JumpToRoom(&room_Start);
    player.Inventory.AddItem(item_GoldKey, 8);
    //player.JumpToRoom(Vector2(0,0));
    //player.CurrentRoom.AddItem(HealthPotion, 2);
    //player.CurrentRoom.PrintItems();
    //player.CurrentRoom.RemoveItem("health potion", 2);
    //player.CurrentRoom.RemoveItem("health potion", 2);
    //player.CurrentRoom.PrintItems();


    //_getch();
    //return 0;

    InitializeCommands();
    Print("Started");
    ClearConsole();


    while (exitProgram == false)
    {
        string text;

        getline(cin, text);

        // Convert to lowercase text
        text = ToLower(text);

        RunCommand(text);
    }

    Print("Press any key to exit...");
    _getch();

    return 0;
}
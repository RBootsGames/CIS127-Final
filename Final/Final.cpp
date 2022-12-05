/* Ross Boetsma
*  CIS 127 - 2102
*  Description:
*/
// https://askcodes.net/coding/what-is-a-c++-delegate-
// option 3

#ifdef _WIN32

#include <Windows.h>

/// @brief adds support for underlining
void enableColors()
{
    DWORD consoleMode;
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleMode(outputHandle, &consoleMode))
    {
        SetConsoleMode(outputHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

#endif


#include <iostream>
#include <string>
#include <conio.h>
#include <tuple>
#include <random>
#include "extensions.h"
#include "commands.h"
#include "inventory.h"
#include "GameDatabase.h"
#include "Player.h"
#include "enemy.h"
#include "objects.h"

#include <vector>
using namespace std;


//#include <fstream>
//#include <random>
//#include "nlohmann/json.hpp"
//
//using json = nlohmann::json;


void CombatLoop()
{
    int menuHeight = 6;
    int answer = 0;
    int answerMax = 0;
    MeleeWeapon* equipedMelee = nullptr;
    RangedWeapon* equipedRanged = nullptr;
    Enemy* selectedEnemy = nullptr;

    vector<AttackQueue> attackQueue;

    while (inCombat)
    {
        bool chooseAttacks = true;
        bool selectingEnemy = true;
        int usedStamina = 0;
        InventorySlot* hPotion = nullptr;
        while (chooseAttacks)
        {
            std::vector<std::string> allLines;


            // print enemies
            for (Enemy* en : combatRoom->Enemies)
            {
                //enemyHealth.push_back(rjust(en->PrintForCombat(20), GetConsoleSize().X));
                allLines.push_back(rjust(en->PrintForCombat(20), GetConsoleSize().X));
            }
            allLines.push_back("");
            allLines.push_back("");


            // print options
            std::string playerInfo = player.GetHealthBar(20) + "  ";
            playerInfo += player.GetStaminaBar(16, usedStamina) + "  ";
            playerInfo += player.GetXPBar(20);
            allLines.push_back(ljust(playerInfo, GetConsoleSize().X)); // Top bar for the menu #########################
            allLines.push_back(cjust("Press 'Enter' or 'Space Bar' to end your turn.", GetConsoleSize().X));

            int fillerLines = 0;
            if (selectingEnemy)
            {
                allLines.push_back(cjust("Choose enemy to attack.", GetConsoleSize().X));

                int enemyCount = 1;
                for (Enemy* en : combatRoom->Enemies)
                {
                    std::string line = to_string(enemyCount) + char(179) + " " + en->GetName();
                    if (en->CanFly && en->IsGroundedCheck())
                        line += " (Grounded) ";
                    if (en->GetHealth() <= 0)
                        line += " (Dead)";
                    allLines.push_back(line);
                    enemyCount++;
                    fillerLines++;
                }
                answerMax = enemyCount - 1;

                hPotion = player.Inventory.GetHealthPotions();
                if (hPotion != nullptr)
                {
                    answerMax++;
                    std::string line = to_string(enemyCount) + char(179) + " Use health potion. ";
                    line += "(Heals " + to_string(player.GetHealAmount()) + " HP. " + to_string(hPotion->NumberInStack) + " remaining.)";
                    allLines.push_back(line);
                }
            }
            // Attack selection
            else 
            {
                allLines.push_back(cjust("Choose attack move.", GetConsoleSize().X));
                allLines.push_back(to_string(0) + char(179) + " or 'Esc' Back to enemy selector.");
                fillerLines++;

                tie(equipedMelee, equipedRanged) = player.Inventory.GetEquipedWeapons();

                int attackNumber = 1;
                // melee attacks
                if (equipedMelee != nullptr)
                {
                    for (auto attack : equipedMelee->PrintAttackInfo())
                    {
                        allLines.push_back(to_string(attackNumber) + char(179) + " " + attack);
                        attackNumber++;
                        fillerLines++;
                    }
                }
                if (equipedRanged != nullptr)
                {
                    for (auto attack : equipedRanged->PrintAttackInfo())
                    {
                        allLines.push_back(to_string(attackNumber) + char(179) + " " + attack);
                        attackNumber++;
                        fillerLines++;
                    }
                }
                answerMax = attackNumber - 1;
            }

            // do actual printing
            std::string fullScreen = string(GetConsoleSize().Y - allLines.size(), '\n');
            
            for (auto line : allLines)
            {
                fullScreen += line + '\n';
                //Print(line);
            }
            if (fillerLines < menuHeight)
                fullScreen += string(menuHeight - fillerLines, '\n');
            Print(fullScreen, false);
            //Print("--", false);
            


            // get number input
            answer = _getch();
            answer = answer - '0';
            // -35 = enter
            // -16 = space
            // -21 = esc
            
            if (answer >= 0 && answer <= answerMax || answer == -21)
            {
                if (selectingEnemy && hPotion != nullptr && answer == answerMax)
                {
                    player.UseHealthPotion();
                }
                else if (selectingEnemy == true && answer != 0)
                {
                    selectingEnemy = false;
                    selectedEnemy = combatRoom->Enemies[answer - 1];
                }
                else if (!selectingEnemy && answer == 0 || !selectingEnemy && answer == -21)
                {
                    selectingEnemy = true;
                    selectedEnemy = nullptr;
                }
                else if (!selectingEnemy && answer <= answerMax)
                {
                    // selection is a melee attack
                    if (answer <= equipedMelee->Attacks.size())
                    {
                        if (player.GetStamina() - usedStamina >= equipedMelee->Attacks[answer - 1].StaminaConsumption)
                        {
                            attackQueue.push_back(AttackQueue(equipedMelee, answer - 1, *selectedEnemy));
                            usedStamina += equipedMelee->Attacks[answer - 1].StaminaConsumption;

                            selectingEnemy = true;
                        }
                    }
                    else // selection is a ranged attack
                    {
                        int index = answer - equipedMelee->Attacks.size() - 1;
                        if (player.GetStamina() - usedStamina >= equipedRanged->Attacks[index].StaminaConsumption)
                        {
                            attackQueue.push_back(AttackQueue(equipedRanged, index, *selectedEnemy));
                            usedStamina += equipedRanged->Attacks[index].StaminaConsumption;

                            selectingEnemy = true;
                        }
                    }

                    equipedMelee = nullptr;
                    equipedRanged= nullptr;
                    selectedEnemy = nullptr;
                }
            }
            // attack
            else if (answer == -35 || answer == -16) // -35 = enter  -16 = space
            {
                for (auto aq : attackQueue)
                {
                    player.Attack(aq);
                }

                player.EndTurn();
                int xpReward = 0;
                bool roomCleared = true;

                for (auto en : combatRoom->Enemies)
                {
                    int xp;
                    std::string enName;
                    InventorySlot* droppedItem;
                    tie(enName, xp, droppedItem) = en->EndTurn();
                    xpReward += xp;

                    // only when enemy dies
                    if (xp > 0)
                    {
                        std::string deathMessage = "You killed " + enName + ".";
                        if (droppedItem != nullptr)
                            deathMessage += " It dropped " + to_string(droppedItem->NumberInStack) + " " + droppedItem->GetName() + "(s).";

                        Print(deathMessage);
                    }
                    if (en->GetHealth() > 0)
                        roomCleared = false;
                }

                player.AddXP(xpReward);
                Print("Press any key to advance...\n");
                _getch();

                usedStamina = 0;
                selectingEnemy = true;
                attackQueue.clear();

                if (roomCleared)
                {
                    combatRoom->ClearEnemies();
                    inCombat = false;
                    Print("You killed all the enemies in this room.\n");
                    return;
                }

                // end players turn
                chooseAttacks = false;
            }
        }

        
        // enemies attack

        for (auto enemy : combatRoom->Enemies)
        {
            enemy->Attack(player);
            enemy->EndTurn();
        }
        player.EndTurn();

        Print("Press any key to advance...\n");
        _getch();

        if (player.IsDead)
        {
            inCombat = false;
            Print();
            return;
        }

        // show results of turn/end turns

        // press any key to continue

        // repeat
    }

}


void MainMenu()
{
    Print("Welcome to The Text Dungeon.");
    Print("How to play:");
    Print("Move around to other rooms using the move command.");
    Print("You can also just use direction names (north, east, south, west), or just the first letter of each direction.");
    Print("");
    Print("Any room with enemies will put you into a combat loop.");
    Print("You can't leave once you start a fight, so you need to be prepared before the fight.");
    Print("You will use numbers to select the enemy and select the attack move.");
    Print("Each attack will require stamina, and when you're out you will have to end your turn.");
    Print("Flying enemies need to be shot with a ranged weapon, or hit while on the ground.");
    Print("End your turn using 'Enter' or 'Space bar'.");
    Print("");
    Print("There is no final boss or end-game, so just go around and explore.");
    Print("");
    Print("Important commands:");
    Print("   help : Lists possible commands.");
    Print("   save : Saves the game.");
    Print("   load : Loads the game. (save file is placed next to the game program 'save.json')");
    Print("   exit : Exits the game.");
    Print("   inventory : Prints your inventory and shows some stats.");
    Print("   pick up : Picks up items in the current room.");
    Print("   search : Gives a description of the current room.");
    Print("   unlock : Uses a key to open a locked door.");
    Print("   equip : Equips weapons in your inventory.");
    Print("   heal : Heals while outside of combat.");
    Print("   move : Moves to different rooms.");
    Print("   map : Shows a map of the dungeon");
    Print("");
    Print("Press any key to play...");

    _getch();
}


int main()
{
    enableColors();
    srand(time(NULL));
    MainMenu();

    player.JumpToRoom(&room_Start);
    //player.Inventory.AddItem(item_HealthPotion, 5);

    //player.Inventory.AddItem(item_GoldKey, 8);
    //player.JumpToRoom(Vector2(0,0));
    //player.CurrentRoom.PrintItems();
    //player.CurrentRoom.RemoveItem("health potion", 2);
    //player.CurrentRoom.RemoveItem("health potion", 2);
    //player.CurrentRoom.PrintItems();


    InitializeCommands();
    ClearConsole();

    while (exitProgram == false)
    {
        if (inCombat)
            CombatLoop();

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
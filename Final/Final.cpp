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
#include <algorithm>
#include <conio.h>
#include "extensions.h"
#include "commands.h"
//#include "Objects.h"

using namespace std;

bool exitProgram = false;


int main()
{

    InitializeCommands();
    Print("Started");
    ClearConsole();
    while (exitProgram == false)
    {
        string text;

        getline(cin, text);

        // Convert to lowercase text
        for_each(text.begin(), text.end(), [](char& c) {
            c = tolower(c);
        });

        RunCommand(text);
    }

    Print("Press any key to exit...");
    _getch();

    return 0;
}
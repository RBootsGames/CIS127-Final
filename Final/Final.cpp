/* Ross Boetsma
*  CIS 127 - 2102
*  Description:
*/
// https://askcodes.net/coding/what-is-a-c++-delegate-
// option 3
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <algorithm>
#include "extensions.h"
#include "commands.h"
#include "Objects.h"

using namespace std;

bool exitProgram = false;


int main()
{
    // using pfunc = void (*)(string);
    // map<int,pfunc> mappy =
    // {
    //     {0, Test},
    //     {1, Test2}
    // };
    // // typedef void (*asdf)();

    // pfunc f = mappy[1];
    // f("asdf");

    // return 0;
    Print("Started");
    ClearConsole("");
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

    return 0;
}
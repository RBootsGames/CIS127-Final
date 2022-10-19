#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>

#include "Objects.h"
//#include "Effects.h"
using namespace std;


// Print statements

/// @brief Simple print statement.
void Print(string text, bool newLine = true)
{
    cout << text;
    if (newLine) cout << endl;
}
/// @brief Simple print statement.
void Print(int num, bool newLine = true)
{
    Print(to_string(num), newLine);
}
/// @brief Simple print statement.
void Print(double num, bool newLine = true)
{
    Print(to_string(num), newLine);
}
/// @brief Simple print statement.
void Print(char c, bool newLine = true)
{
    Print(string(1, c), newLine);
}

// Trim statements

const string WHITESPACE = " \n\r\t\f\v";

/// <summary>
/// Removes spaces from the front of a string.
/// </summary>
string ltrim(const string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}
/// <summary>
/// Removes spaces from the end of a string.
/// </summary>
string rtrim(const string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}
/// <summary>
/// Removes spaces from both ends of a string.
/// </summary>
string trim(const string& s) {
    return rtrim(ltrim(s));
}

/// <summary>
/// Gets the width and height of the console window.
/// </summary>
/// <returns>X = width, Y = height</returns>
Size GetConsoleSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    Size point;
    point.X = columns;
    point.Y = rows;
    //printf("columns: %d\n", columns);
    //printf("rows: %d\n", rows);
    return point;
}

/* Linux (not being used)
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);
    return 0;  // make sure your main returns int
}
*/

#endif
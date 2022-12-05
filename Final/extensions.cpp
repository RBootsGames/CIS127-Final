#include "extensions.h"

#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include <algorithm>
#include <vector>

#include "Objects.h"
using namespace std;

//char UNDERLINED[] = { 0x1b,'[','4',';','3','9','m',0 };
//char NORMAL[] = { 0x1b,'[','0',';','3','9','m',0 };

//#define UNDERLINED "\033[4m";
//#define NORMAL "\033[0m";


// Print statements

/// @brief Simple print statement.
void Print(string text, bool newLine, bool underline)
{
    if (underline) cout << UNDERLINED;
    //if (underline) cout << "\e[4m";
    cout << text;
    if (underline) cout << NORMAL;
    //if (underline) cout << "\e[0m";
    if (newLine) cout << endl;
}
/// @brief Simple print statement.
void Print()
{
    cout << endl;
}
/// @brief Simple print statement.
void Print(int num, bool newLine, bool underline)
{
    Print(to_string(num), newLine, underline);
}
/// @brief Simple print statement.
void Print(double num, bool newLine, bool underline)
{
    Print(to_string(num), newLine, underline);
}
/// @brief Simple print statement.
void Print(char c, bool newLine, bool underline)
{
    Print(string(1, c), newLine, underline);
}

/// @brief [■■■■■■     ]
std::string ProgressBar(int width, int maxValue, int currentValue, char fillChar)
{
    std::string bar = "[";
    int fullCharCount = round(((float)width / maxValue) * maxValue);
    int valueCharCount = round(((float)width / maxValue) * currentValue);
    bar += ljust(string(valueCharCount, char(254)), fullCharCount, fillChar) + ']';

    return bar;
}



/// <summary>
/// Tries to convert a string to an int.
/// </summary>
/// <returns>True if successful.</returns>
bool StringToInt(string input, int& num)
{
    try
    {
        num = stoi(input);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

string ToLower(string text)
{
    string newString = "";
    for (char c : text)
    {
        newString += tolower(c);
    }
    //for_each(text.begin(), text.end(), [](char& c) {
    //    c = tolower(c);
    //});
    return newString;
}

const char VOWELS[] = "aeiou";
/// <summary>
/// Some words use 'a' in front of them, some use 'an' when they are singular.
/// </summary>
/// <returns>word with 'a' or 'an' in front of it.</returns>
string WhenToUseAn(string itemName)
{
    //string singleWord;
    //bool startsWithVowel = false;

    //for (char c : itemName)
    //{
    //    if (c != ' ')
    //        singleWord += c;
    //    else
    //        break;
    //}

    // exceptions
    // nothing so far

    for (char vowel : VOWELS)
    {
        if (tolower(itemName[0]) == vowel)
        {
            return "an " + itemName;
            //startsWithVowel = true;
        }
    }

    return "a " + itemName;
}

/// <summary>
/// Converts a vector of arguments to a single string with spaces between the words.
/// </summary>
string ArgsToString(vector<string> args)
{
    string text = "";
    for (string word : args)
    {
        text += word + " ";
    }
    text.pop_back();
    return text;
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

string ljust(string text, int count, char fillChar)
{
    string temp = text;
    int fillCount = count - (int)text.size();
    if (fillCount > 0)
        temp += string(fillCount, fillChar);
    return temp;
}
string rjust(string text, int count, char fillChar)
{
    int fillCount = count - (int)text.size();
    string temp = "";
    if (fillCount > 0)
        temp += string(fillCount, fillChar);
    temp += text;
    return temp;
}
string cjust(string text, int count, char fillChar)
{
    int fillCount = count - (int)text.size();

    if (fillCount > 0)
    {
        int left = fillCount / 2;
        int right = fillCount - left;
        return string(left, fillChar) + text + string(right, fillChar);
    }
    else
        return text;
}

/// <summary>
/// Gets the width and height of the console window.
/// </summary>
/// <returns>X = width, Y = height</returns>
Vector2 GetConsoleSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    Vector2 point;
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

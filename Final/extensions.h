#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <vector>
#include <string>
#include "objects.h"

using namespace std;
// Print statements

/// @brief Simple print statement.
void Print(std::string text, bool newLine = true, bool underline = false);
/// @brief Simple print statement.
void Print();
/// @brief Simple print statement.
void Print(int num, bool newLine = true, bool underline = false);
/// @brief Simple print statement.
void Print(double num, bool newLine = true, bool underline = false);
/// @brief Simple print statement.
void Print(char c, bool newLine = true, bool underline = false);

/// <summary>
/// Tries to convert a string to an int.
/// </summary>
/// <returns>True if successful.</returns>
bool StringToInt(std::string input, int& num);

std::string ToLower(std::string text);

/// <summary>
/// Some words use 'a' in front of them, some use 'an' when they are singular.
/// </summary>
/// <returns>word with 'a' or 'an' in front of it.</returns>
std::string WhenToUseAn(std::string itemName);

/// <summary>
/// Converts a vector of arguments to a single string with spaces between the words.
/// </summary>
std::string ArgsToString(vector<std::string> args);

// Trim statements

/// <summary>
/// Removes spaces from the front of a string.
/// </summary>
std::string ltrim(const std::string& s);
/// <summary>
/// Removes spaces from the end of a string.
/// </summary>
std::string rtrim(const std::string& s);
/// <summary>
/// Removes spaces from both ends of a string.
/// </summary>
std::string trim(const std::string& s);

std::string ljust(std::string text, int count, char fillChar = ' ');
std::string rjust(std::string text, int count, char fillChar = ' ');
std::string cjust(std::string text, int count, char fillChar = ' ');

/// <summary>
/// Gets the width and height of the console window.
/// </summary>
/// <returns>X = width, Y = height</returns>
Vector2 GetConsoleSize();


#endif
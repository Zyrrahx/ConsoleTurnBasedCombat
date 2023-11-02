// ConsoleTurnBasedCombat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Battle.h"

int main()
{
	do
	{
		std::cout << '\n' << "Press a key to continue.";
	} while (std::cin.get() != '\n');
	//test UI functionality and drawing commands
	UI currentUI;
	Battle currentBattle;
	currentUI.UICreateWindow();
	do
	{
		std::cout << '\n' << "Press a key to continue.";
	} while (std::cin.get() != '\n');
    std::cout << "Hello World!\n";
	currentUI.SetCursorPosition(1, 1);
	do
	{
		for (int i = 0; i < 15; i++)
		{
			std::cout << '\n';
		}
	} while (std::cin.get() != '\n');
	currentUI.SetCursorPosition(0, 0);
	do
	{
	} while (std::cin.get() != '\n');
	std::cout << "Testing new cursor pos";
	std::cout << '\n';
	do
	{
	} while (std::cin.get() != '\n');
	currentUI.Cls();
	do
	{
	} while (std::cin.get() != '\n');
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

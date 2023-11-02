#ifndef BATTLE_H
#define BATTLE_H
#include "Player.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#define MAX_X 100					//Max characters on a line of the console
#define MAX_Y 50					//Max number of lines in the console
using namespace std::literals::chrono_literals;

static bool ComparePlayable(Player* a, Player* b)			//used in sort algorithm to compare isPlayable variable with next one in sort order(pulls playable actors to the front of the vector)
{
	return (a->isPlayable > b->isPlayable);
}

class Chronomancy			//:D				<- For test usage of delta time concepts
{
public:
	static std::chrono::time_point<std::chrono::high_resolution_clock> last;
	static std::chrono::steady_clock::duration epoch;
	static std::chrono::nanoseconds delta;
	static std::chrono::time_point<std::chrono::high_resolution_clock> now;
	static std::chrono::steady_clock::time_point now_ms;
	static long delta_ms;
};

class UI
{
public:
	void Cls()																	//Completely clears the console window
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO bi;
		COORD coord = { 0, 0 };
		std::cout.flush();
		// Figure out the current width and height of the console window
		if (!GetConsoleScreenBufferInfo(hOut, &bi)) {
			abort();
		}
		DWORD length = bi.dwSize.X * bi.dwSize.Y;
		DWORD written;
		// Flood-fill the console with spaces to clear it
		FillConsoleOutputCharacter(hOut, TEXT(' '), length, coord, &written);
		// Reset the attributes of every character to the default.
		// This clears all background colour formatting, if any.
		FillConsoleOutputAttribute(hOut, bi.wAttributes, length, coord, &written);
		// Move the cursor back to the top left for the next sequence of writes
		SetConsoleCursorPosition(hOut, coord);
	}
	void SetCursorPosition(int x, int y)							//Used to move cursor through window and redraw
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		std::cout.flush();
		COORD coord = { (SHORT)x, (SHORT)y };
		SetConsoleCursorPosition(hOut, coord);
	}
	bool GetConsoleSize(int* cols, int* rows)
	{
		HWND hWnd;
		HANDLE hOut;
		CONSOLE_FONT_INFO fi;
		int w, h;
		RECT rect = { 0,0,0,0 };
		hWnd = GetConsoleWindow();
		if(hWnd)
		{
			hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (hOut && hOut != (HANDLE)-1)
			{
				if (GetCurrentConsoleFont(hWnd, FALSE, &fi))
				{
					if (GetClientRect(hWnd, &rect))
					{
						w = rect.right - rect.left;
						h = rect.bottom - rect.top;
						*cols = w / fi.dwFontSize.X;
						*rows = h / fi.dwFontSize.Y;
						return TRUE;
					}
				}
			}
		}
		return FALSE;
	}
	bool SetConsoleSize(int cols, int rows)
	{
		HWND hWnd;
		HANDLE hOut;
		CONSOLE_FONT_INFO fi;
		CONSOLE_SCREEN_BUFFER_INFO bi;
		int w, h, bw, bh;
		RECT rect = { 0,0,0,0 };
		COORD coord = { 0,0 };
		hWnd = GetConsoleWindow();
		//Disable resizing and maximizing of console window to ensure consistancy of UI
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
		if (hWnd) {
			hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (hOut && hOut != (HANDLE)-1) {
				if (GetCurrentConsoleFont(hOut, FALSE, &fi)) {
					if (GetClientRect(hWnd, &rect)) {
						w = rect.right - rect.left;
						h = rect.bottom - rect.top;
						if (GetWindowRect(hWnd, &rect)) {
							bw = rect.right - rect.left - w;
							bh = rect.bottom - rect.top - h;
							if (GetConsoleScreenBufferInfo(hOut, &bi)) {
								coord.X = bi.dwSize.X;
								coord.Y = bi.dwSize.Y;
								if (coord.X < cols || coord.Y < rows) {
									if (coord.X < cols) {
										coord.X = cols;
									}
									if (coord.Y < rows) {
										coord.Y = rows;
									}
									if (!SetConsoleScreenBufferSize(hOut, coord)) {
										return FALSE;
									}
								}
								return SetWindowPos(hWnd, NULL, rect.left, rect.top, cols * fi.dwFontSize.X + bw, rows * fi.dwFontSize.Y + bh, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
							}
						}
					}
				}
			}
		}
		return FALSE;
	}
	bool GetConsoleBufferSize(int* cols, int* rows)
	{
		HANDLE hOut;
		CONSOLE_SCREEN_BUFFER_INFO bi;
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut && hOut != (HANDLE)-1)
		{
			if (GetConsoleScreenBufferInfo(hOut, &bi))
			{
				*cols = bi.dwSize.X;
				*rows = bi.dwSize.Y;
				return TRUE;
			}
		}
		return FALSE;
	}
	bool SetConsoleBufferSize(int cols, int rows)
	{
		HANDLE hOut;
		CONSOLE_SCREEN_BUFFER_INFO bi;
		COORD coord { 0,0 };
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if(hOut && hOut != (HANDLE)-1) 
		{
			if (GetConsoleScreenBufferInfo(hOut, &bi))
			{
				coord.X = bi.dwSize.X;
				coord.Y = MAX_Y;
				return SetConsoleScreenBufferSize(hOut, coord);
			}
		}
		return FALSE;
	}
	bool ConstrainConsoleWindow(int xMin, int yMin)
	{
		return FALSE;
	}
	void UICreateWindow()
	{
		SetConsoleSize(MAX_X+3, MAX_Y);				//Add 3 as calculations due to font reduce buffer size by 3
		//SetConsoleBufferSize(MAX_X, MAX_Y);	
		UIAltCreateWindow();
	}
	void UIAltCreateWindow()
	{
		// get handle to the console window
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		// retrieve screen buffer info
		CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
		GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);

		// current window size
		short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
		short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;

		// current screen buffer size
		short scrBufferWidth = scrBufferInfo.dwSize.X;
		short scrBufferHeight = scrBufferInfo.dwSize.Y;

		// to remove the scrollbar, make sure the window height matches the screen buffer height
		COORD newSize;
		newSize.X = scrBufferWidth;
		newSize.Y = winHeight;

		// set the new screen buffer dimensions
		int Status = SetConsoleScreenBufferSize(hOut, newSize);
		if (Status == 0)
		{
			std::cout << "SetConsoleScreenBufferSize() failed! Reason : " << GetLastError() << std::endl;
			exit(Status);
		}

		// print the current screen buffer dimensions
		GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);
		std::cout << "Screen Buffer Size : " << scrBufferInfo.dwSize.X << " x " << scrBufferInfo.dwSize.Y << std::endl;

	}

	void UIDraw()
	{
		std::memset((char*)prevScreen, 0, MAX_X * MAX_Y);

		for (int y = 0; y != MAX_Y; y++)
		{
			for (int x = 0; x != MAX_X; x++)
			{
				if (currentScreen[x][y] == prevScreen[x][y])
				{
					continue;
				}
				SetCursorPosition(x, y);
				std::cout << currentScreen[x][y];
			}
		}
		std::cout.flush();
		std::memcpy((char*)prevScreen, (char const*)currentScreen, MAX_X * MAX_Y);
	}
	void UIUpdate(char* updateScreen)
	{
		std::memcpy((char*)currentScreen, (char const*)updateScreen, MAX_X * MAX_Y);
	}
	
	void UIClear()
	{

	}
private:
	int cols, rows;
	char prevScreen[MAX_X][MAX_Y];
	char currentScreen[MAX_X][MAX_Y];
};

class Party
{
public:
	Player* FetchActor(int x)
	{
		Player* y = new Player;
		y = &playerActors[x];
		return y;
	}
	void addActor(Player x)
	{
		playerActors.push_back(x);
	}
	void removeActor(int x)
	{
		//Erase actor from the party and resize the vector
		playerActors.erase(playerActors.begin() + 1, playerActors.begin() + x);
	}
private:
	std::vector<Player> playerActors;
};

class Battle
{
public:
	void AssignUIWindow(UI* x)
	{
		battleWindow = x;
	}
	void BattleStart()		//battle loop goes here
	{
		battleState = true;
		//Battle starts, battle steps through until an actor reaches the speed threshold then they act
		//After acting that actors turn ends and their speed is set back to 0, if another creature is
		//acting on the same turn they then act, otherwise it proceeds to battle step again. Once all
		//player controlled actors or enemy controlled actors are defeated the battle ends and breaks the loop.
		while(battleState == true) 
		{
			Chronomancy::last = Chronomancy::now;
			Chronomancy::now = std::chrono::high_resolution_clock::now();
			Chronomancy::now_ms = std::chrono::time_point_cast<std::chrono::nanoseconds>(Chronomancy::now);
			Chronomancy::epoch = Chronomancy::now_ms.time_since_epoch();
			Chronomancy::delta = std::chrono::duration_cast<std::chrono::nanoseconds>(Chronomancy::epoch);
			Chronomancy::delta_ms = Chronomancy::delta.count();
			BattleStep();
			if (actorsTurn.size() > 0)
			{
				for (int i = 0; i < actorsTurn.size(); i++) 
				{
					TurnStart(actorsTurn[i]);
					TurnStep(actorsTurn[i]);
					TurnEnd(actorsTurn[i]);
				}
				actorsTurn.clear();				//After all actors act for the turn, clear their turns
			}
		std::this_thread::sleep_for(1000ms);			//Delay next round by delta time
		std::this_thread::sleep_until(std::chrono::high_resolution_clock::now() + 1000ms);
		}
	}
	void BattleEnd()		//activates when battle ends, used to break loop
	{
		battleState = false;
	}
	void BattleStep()		//steps through battle after all current turns have ended
	{
		for (int i = 0; i < battleActors.size(); i++)
		{
			battleActors[i]->currentSpeed += battleActors[i]->FetchStat(Stats::SPD);	//Increase actors rate to act by their speed
			if (battleActors[i]->health > 0)									//Check that actor is alive
			{
				if (battleActors[i]->currentSpeed >= speedThreshold)			//Check if actor has passed the speed threshold to act
				{
					actorsTurn.push_back(i);
				}
			}
			else										//if the actor is dead, set their speed to 0
			{
				battleActors[i]->currentSpeed = 0;
			}
		}						
	}
	void TurnStart(int x)		//starts a battle actors turn if their speed value reached threshold
	{
		if (battleActors[x]->isPlayable == true)
		{
			//user controlled UI functions here
		} 
		else 
		{
			//Ai controlled actor functions here
		}
	}
	void TurnStep(int x)
	{
		//Perform mid turn step functions for actions/abilities and effects
	}
	void TurnEnd(int x)			//ends a battle actors turn and moves to the next actor acting this turn
	{
		//Perform end turn functions for actions/abilities and effects
		battleActors[x]->currentSpeed = 0;
	}
	
	void BattleWindow() 
	{
	
	}

private:
	void SortBattleActors()				//Sort battleactors so playable actors are the first entry, for draw purposes; if a new actor is added mid battle this function is run again
	{
		for (int i = 0; i < battleActors.size(); i++)
		{
			std::sort(battleActors.begin(), battleActors.end(), ComparePlayable);
		}
	}
	std::vector<Player*> battleActors;
	std::vector<int> actorsTurn;		//
	int speedThreshold = 50;			//Reducing this value increases the rate of turns, increasing it decreases the rate
	int stepDelay = 250;				//Reducing this value decreases the time between battle steps
	bool battleState = false;
	UI* battleWindow;					//Point to existing UI object to use as the battle window
};

#endif
#ifndef PLAYER_H
#define PLAYER_H	
#include <string>
#include <map>

enum class Stats {
	STR = 0,
	DEF = 1,
	MAT = 2,
	MDF = 3,
	SPD = 4,
	NELEMENTS = 5
};
static std::map < Stats, std::string > statNames{		{Stats::STR, "Strength"}, 
														{Stats::DEF, "Defence"}, 
														{Stats::MAT, "Magical Attack"},
														{Stats::MDF, "Magical Defence"},
														{Stats::SPD, "Speed"} };

class Player
{
public:
	std::string name;
	int level = 1;
	int currentSpeed = 10;
	bool isPlayable = false;
	int health = 10;
	int mana = 5;

	int FetchBaseStat(Stats x)
	{
		return this->baseStats[(int)x];
	}
	int FetchStat(Stats x)
	{
		return this->stats[(int)x];
	}
	int* FetchAllBaseStats()
	{
		int* statsPointer = new int[(int)Stats::NELEMENTS];
		statsPointer = this->baseStats;
		return statsPointer;
	}
	int* FetchAllStats()
	{
		int* statsPointer = new int[(int)Stats::NELEMENTS];
		statsPointer = this->stats;
		return statsPointer;
	}
	void ReadFromFile(std::string filePath)
	{

	}

private:
	int baseStats[(int)Stats::NELEMENTS];
	int stats[(int)Stats::NELEMENTS];
};

#endif
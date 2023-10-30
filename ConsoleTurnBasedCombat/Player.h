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
std::map < Stats, std::string > statNames{				{Stats::STR, "Strength"}, 
														{Stats::DEF, "Defence"}, 
														{Stats::MAT, "Magical Attack"},
														{Stats::MDF, "Magical Defence"},
														{Stats::SPD, "Speed"} };

class Player
{
public:
	std::string name;
	int level;
	int currentSpeed;
	bool isPlayable = false;

	int fetchBaseStat(Stats x)
	{
		return this->baseStats[(int)x];
	}
	int fetchStat(Stats x)
	{
		return this->stats[(int)x];
	}
	int* fetchAllBaseStats()
	{
		int* statsPointer = new int[(int)Stats::NELEMENTS];
		statsPointer = this->baseStats;
		return statsPointer;
	}
	int* fetchAllStats()
	{
		int* statsPointer = new int[(int)Stats::NELEMENTS];
		statsPointer = this->stats;
		return statsPointer;
	}

private:
	int baseStats[(int)Stats::NELEMENTS];
	int stats[(int)Stats::NELEMENTS];
};

#endif
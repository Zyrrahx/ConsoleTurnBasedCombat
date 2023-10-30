#ifndef BATTLE_H
#define BATTLE_H
#include "Player.h"
#include <vector>
#include <chrono>

class Chronomancy			//:D				<- For test usage of delta time concepts
{
public:
	static std::chrono::time_point<std::chrono::high_resolution_clock> last;
	static std::chrono::time_point<std::chrono::high_resolution_clock> delta;
	static std::chrono::time_point<std::chrono::high_resolution_clock> now;
};

class Battle
{
public:
	void battleStart()		//battle loop goes here
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
			battleStep();
			if (actorsTurn.size() > 0)
			{
				for (int i = 0; i < actorsTurn.size(); i++) 
				{
					turnStart(actorsTurn[i]);
					turnStep(actorsTurn[i]);
					turnEnd(actorsTurn[i]);
				}
			}
		}
	}
	void battleEnd()		//activates when battle ends, used to break loop
	{
		battleState = false;
	}
	void battleStep()		//steps through battle after all current turns have ended
	{
		for(int i = 0; i < battleActors.size(); i++) 
		{
			battleActors[i]->currentSpeed += battleActors[i]->fetchStat(Stats::SPD);
			if (battleActors[i]->currentSpeed >= speedThreshold)
			{
				actorsTurn.push_back(i);
			}
		}
	}
	void turnStart(int x)		//starts a battle actors turn if their speed value reached threshold
	{

	}
	void turnStep(int x)
	{

	}
	void turnEnd(int x)			//ends a battle actors turn and moves to the next actor acting this turn
	{

	}
private:
	std::vector<Player*> battleActors;
	std::vector<int> actorsTurn;
	int speedThreshold = 50;			//Reducing this value increases the rate of turns, increasing it decreases the rate
	bool battleState = false;
};

#endif
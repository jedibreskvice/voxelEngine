#ifndef TURN_SYSTEM_H
#define TURN_SYSTEM_H

#include <vector>
#include <glm/glm.hpp> 

struct playerInfo {
	unsigned int playerID;
};

extern float timeRemaining;
extern unsigned int currentPlayer;
extern std::vector<playerInfo> listOfAllPlayers;

void runGame(unsigned int numOfPlayers, double timeSinceGlfInitialized);
void passToNextPlayer();
void printPlayerList();

#endif // TURN_SYSTEM_H

#include <iostream>
#include <vector>
#include <chrono>  
#include "TurnSystem.h"
#include "UIManager.h"
#include "SpriteList.h"


bool gameInProgress = false;
float turnTimer = 0;  //in seconds, if 0 then the timer is disabled
bool autoSwitchPerspectives = true; //automatically switches to the player whose turn it is
unsigned int currentTurn = 0;
float remainingTurnTime;
std::vector<playerInfo> listOfAllPlayers;
unsigned int currentPlayer;
float timeRemaining;
int secondsInTurn;
static int secondsElapsed = 0;

void initializeTurnSystem(unsigned int numOfPlayers) {
	for (unsigned int i = 0; i < numOfPlayers; i++) {
		playerInfo newPlayerInfo = { i + 1 };
		listOfAllPlayers.push_back(newPlayerInfo);
	}
}

void passToNextPlayer() {
	if (turnTimer != 0) {
		remainingTurnTime = turnTimer;
	}
	if (currentPlayer == listOfAllPlayers.size()) {
		currentPlayer = 1;
		resetMovementValues();
		currentTurn++;
		if (autoSwitchPerspectives) {
			perspective = currentPlayer;
			visionRecalculationRequired = true;
		}
	}
	else {
		currentPlayer++;
		if (autoSwitchPerspectives) {
			perspective = currentPlayer;
			visionRecalculationRequired = true;
		}
	}
	secondsElapsed = 0;
}

void runGame(unsigned int numOfPlayers, double timeSinceGlfInitialized) {
	static bool firstRun = true;
	static double lastTime = 0;
	static double accumulatedTime = 0; 

	if (!gameInProgress) {
		initializeTurnSystem(numOfPlayers);
		std::cout << "Initialized a game with " << numOfPlayers << " players" << std::endl;
		gameInProgress = true;
		printPlayerList();
		currentPlayer = 1;
		lastTime = timeSinceGlfInitialized; 
		accumulatedTime = 0;
		return; 
	}
	double currentTime = timeSinceGlfInitialized;
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	accumulatedTime += deltaTime;

	if (turnTimer != 0 && accumulatedTime >= 1.0) {
		accumulatedTime -= 1.0;
		timeRemaining = turnTimer - ++secondsElapsed;

		if (secondsElapsed >= turnTimer) {
			passToNextPlayer();
		}
	}
}

void printPlayerList() {
	std::cout << "List of Players:" << std::endl;
	for (const auto& player : listOfAllPlayers) {
		std::cout << "Player ID: " << player.playerID << std::endl;
	}
	std::cout << "Total Players: " << listOfAllPlayers.size() << std::endl;
}
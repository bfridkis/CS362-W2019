#ifndef _CARDEFFECTS_H
#define _CARDEFFECTS_H

#include "dominion.h"

int adventurerEffect(int currentPlayer, struct gameState *state);
int smithyEffect(int currentPlayer, struct gameState *state, int handPos);
int cutpurseEffect(int currentPlayer, struct gameState *state, int handPos);
int minionEffect(int currentPlayer, struct gameState *state, int choice1,
	int choice2, int handPos);
int ambassadorEffect(int currentPlayer, struct gameState *state, int choice1,
	int choice2, int handPos);
int council_roomEffect(int currentPlayer, struct gameState *state, int handPos);

#endif
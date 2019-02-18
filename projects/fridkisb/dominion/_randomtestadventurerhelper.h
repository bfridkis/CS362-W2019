/* ----------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 4
 *
 *       _randomtestadventurerhelper.h
 *
 * Header file for _randomtestadventurerhelper.c.
 * -----------------------------------------------
 */

#ifndef _CARDTEST2HELPER_H
#define _CARDTEST2HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 1

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See randomtestadventurer.c for more details.)
#define ADVENTURER_CALLS 1

#if RANDOMIZE 
#define NUM_PLAYERS MAX_PLAYERS
#else 
#define NUM_PLAYERS 4
#endif

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "rngs.h"

struct failedTest {
	int lineNumber;
	char description[256];
	int testNumber;
};

typedef struct failedTest failedTest;

int _randomtestadventurerhelper(int numPlayers, int k[], struct gameState* G, 
	failedTest failures[], int* failCt, int treasureCardCountSpecifier, 
	int isBoundary, int testNumber);


#endif
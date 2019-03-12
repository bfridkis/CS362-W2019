/* ----------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 5
 *
 *       _randomtesthelper_adventurer.h
 *
 * Header file for _randomtesthelper_adventurer.c.
 * -----------------------------------------------
 */

#ifndef _RANDOMTESTADVENTURERHELPER_H
#define _RANDOMTESTADVENTURERHELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 1

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See randomtest_adventurer.c for more details.)
#define ADVENTURER_CALLS 10000

#define NUM_PLAYERS 4

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

int _randomtesthelper_adventurer(int numPlayers, int k[], struct gameState* G, 
	failedTest failures[], int* failCt, int treasureCardCountSpecifier, 
	int isBoundary, int testNumber);


#endif
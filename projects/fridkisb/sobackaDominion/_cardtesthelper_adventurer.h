/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 5
 *
 *       _cardtesthelper_adventurer.h
 *
 * Header file for _cardtesthelper_adventurer.c.
 * -------------------------------------
 */

#ifndef _CARDTESTHELPER_ADVENTURER_H
#define _CARDTESTHELPER_ADVENTURER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 0

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See cardtest_adventurer.c for more details.)
#define ADVENTURER_CALLS 1

#define NUM_PLAYERS 2

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "rngs.h"

struct failedTest {
	int lineNumber;
	char description[256];
	int testNumber;
};

typedef struct failedTest failedTest;

int _cardtesthelper_adventurer(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int treasureCardCountSpecifier, int isBoundary, int testNumber);


#endif
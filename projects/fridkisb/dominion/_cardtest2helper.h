/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _cardtest2helper.h
 *
 * Header file for _cardtest2helper.c.
 * -------------------------------------
 */

#ifndef _CARDTEST2HELPER_H
#define _CARDTEST2HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 0

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See cardtest2.c for more details.)
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

int _cardtest2helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int treasureCardCountSpecifier, int isBoundary, int testNumber);


#endif
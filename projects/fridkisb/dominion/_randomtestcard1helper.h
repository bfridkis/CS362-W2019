/* ------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 4
 *
 *       _randomtestcard1helper.h
 *
 * Header file for _randomtestcard1helper.c.
 * ------------------------------------------
 */

#ifndef _CARDTEST4HELPER_H
#define _CARDTEST4HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 1

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See randomtestcard1.c for more details.)
#define COUNCIL_ROOM_CALLS 1

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "rngs.h"

#if RANDOMIZE 
#define NUM_PLAYERS MAX_PLAYERS
#else 
#define NUM_PLAYERS 4
#endif

struct failedTest {
	int lineNumber;
	char description[256];
	int testNumber;
};

typedef struct failedTest failedTest;

int _randomtestcard1helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int noCopper, int testNumber);


#endif
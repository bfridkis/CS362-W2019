/* ------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 4
 *
 *       _randomtesthelper_councilroom.h
 *
 * Header file for _randomtesthelper_councilroom.c.
 * ------------------------------------------
 */

#ifndef _RANDOMTESTCARD1HELPER_H
#define _RANDOMTESTCARD1HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 1

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See randomtest_councilroom.c for more details.)
#define COUNCIL_ROOM_CALLS 10000

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "rngs.h"

#define NUM_PLAYERS 4

struct failedTest {
	int lineNumber;
	char description[256];
	int testNumber;
};

typedef struct failedTest failedTest;

int _randomtesthelper_councilroom(int numPlayers, int k[], struct gameState* G, 
	failedTest failures[], int* failCt, int noCopper, int testNumber);


#endif
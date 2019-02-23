/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 4
 *
 *       _randomtestcard2helper.h
 *
 * Header file for _randomtestcard2helper.c.
 * -------------------------------------
 */

#ifndef _RANDOMTESTCARD2HELPER_H
#define _RANDOMTESTCARD2HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 1

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See randomtestcard2.c for more details.)
#define CUTPURSE_CALLS 10000

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

int _randomtestcard2helper(int numPlayers, int k[], struct gameState* G, 
	failedTest failures[], int* failCt, int noCopper, int testNumber);


#endif
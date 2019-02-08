/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _unittest4helper.h
 *
 * Header file for _unittest4helper.c.
 * -------------------------------------
 */

#ifndef _UNITTEST4HELPER_H
#define _UNITTEST4HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 10

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 0

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See unittest4.c for more details.)
#define NUM_COIN_UPDATES 100

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <limits.h>

struct failedTest {
	int lineNumber;
	char description[256];
	int testNumber;
};

typedef struct failedTest failedTest;

int _unittest4helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int isNoTreasureTest, int isNoBonusTest, int testNumber);

#endif
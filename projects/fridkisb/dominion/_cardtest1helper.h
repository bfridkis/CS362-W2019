/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _cardtest1helper.h
 *
 * Header file for _cardtest1helper.c.
 * -------------------------------------
 */

#ifndef _CARDTEST1HELPER_H
#define _CARDTEST1HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 0

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See cardtest1.c for more details.)
#define SMITHY_CALLS 1

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

int _cardtest1helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int deckCardCountSpecifier, int isBoundary, int testNumber);


#endif
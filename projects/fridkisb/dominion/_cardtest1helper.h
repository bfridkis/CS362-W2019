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

#define RANDOMIZE 0

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
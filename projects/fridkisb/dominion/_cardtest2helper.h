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

int _cardtest2helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int treasureCardCountSpecifier, int isBoundary, int testNumber);


#endif
/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _cardtest3helper.h
 *
 * Header file for _cardtest3helper.c.
 * -------------------------------------
 */

#ifndef _CARDTEST3HELPER_H
#define _CARDTEST3HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

#define RANDOMIZE 0

#define NUM_PLAYERS 4

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

int _cardtest3helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int noCopper, int testNumber);


#endif
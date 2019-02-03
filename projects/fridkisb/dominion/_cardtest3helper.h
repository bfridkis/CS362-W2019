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
	int isBoundary;
};

typedef struct failedTest failedTest;

int _cardtest3helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int playerCount, int noCopper);


#endif
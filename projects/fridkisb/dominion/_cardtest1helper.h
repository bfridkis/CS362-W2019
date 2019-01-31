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

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

struct failedTest {
	int lineNumber;
	char description[256];
};

typedef struct failedTest failedTest;

struct cardtest1helper2struct {
	int rv;
	int expectedRV;
	int expectedPile;
	int expectedIdx;
};

typedef struct cardtest1helper2struct cardtest1helper2struct;

int _cardtest1helper(int k[], struct gameState* G, failedTest failures[]);
	
void _cardtest1helper2(cardtest1helper2struct ut2h2s, failedTest failures[],
	int* failCt, struct gameState* G, int k[]);

#endif
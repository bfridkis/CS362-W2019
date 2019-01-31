/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _unittest1helper.h
 *
 * Header file for _unittest1helper.c.
 * -------------------------------------
 */

#ifndef _UNITTEST1HELPER_H
#define _UNITTEST1HELPER_H

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

struct unittest1helper2struct {
	int rv;
	int expectedRV;
	int expectedPile;
	int expectedIdx;
};

typedef struct unittest1helper2struct unittest1helper2struct;

int _unittest1helper(int k[], struct gameState* G, failedTest failures[]);
	
void _unittest1helper2(unittest1helper2struct ut2h2s, failedTest failures[],
	int* failCt, struct gameState* G, int k[]);

#endif
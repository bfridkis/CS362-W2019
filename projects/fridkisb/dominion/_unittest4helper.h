/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _unittest4helper.h
 *
 * Header file for _unittest4helper.c.
 * -------------------------------------
 */

#ifndef _UNITEST2HELPER_H
#define _UNITEST2HELPER_H

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
};

typedef struct failedTest failedTest;

int _unittest4helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int isNoTreasureTest, int isNoBonusTest);

#endif
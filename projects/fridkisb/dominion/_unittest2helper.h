/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _unittest2helper.h
 *
 * Header file for _unittest2helper.c.
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

struct failedTest {
	int lineNumber;
	char description[256];
};

typedef struct failedTest failedTest;

struct unittest2helper2struct {
	int rv;
	int expectedRV;
	struct gameState* G;
	int expectedPile;
	int expectedIdx;
	int cardValue; 
	int* k;
	failedTest failures;
	int* failCt;
};

typedef struct unittest2helper2struct unittest2helper2struct;

int _unittest2helper(int testCase, int k[], struct gameState* G,
	failedTest failures[], int* failedTestCount);
	
void _unittest2helper2(unittest2helper2struct ut2h2s);

#endif
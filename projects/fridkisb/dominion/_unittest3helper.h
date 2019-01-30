/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _unittest3helper.h
 *
 * Header file for _unittest3helper.c.
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

struct unittest3stats {
	int _noDev;
	int _gt0to10PercentDev;
	int _11to20PercentDev;
	int _21to30PercentDev;
	int _31to40PercentDev;
	int _41to50PercentDev;
	int _51PlusPercentDev;
};

typedef struct unittest3stats unittest3stats;

int _unittest3helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, unittest3stats* ut3s, int isEmptyDeckTest, int isMaxDeckTest);

#endif
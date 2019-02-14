/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _unittest2helper.h
 *
 * Header file for _unittest2helper.c.
 * -------------------------------------
 */

#ifndef _UNITTEST2HELPER_H
#define _UNITTEST2HELPER_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 10

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

int _unittest2helper(int testCase, int k[], struct gameState* G,
	failedTest failures[], int* failedTestCount);

#endif
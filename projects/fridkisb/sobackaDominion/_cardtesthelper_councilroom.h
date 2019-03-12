/* -------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 5
 *
 *       _cardtest_councilroomhelper.h
 *
 * Header file for _cardtest_councilroomhelper.c.
 * -------------------------------------
 */

#ifndef _CARDTESTHELPER_COUNCILROOM_H
#define _CARDTESTHELPER_COUNCILROOM_H

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
#define MAX_FAILS 20

//Toggle randomized inputs on/off (0 = off, 1 = on)
#define RANDOMIZE 0

//Number of non-boundary test runs, 
//also determines input sizes for each run,
//if RANDOMIZE is set to 0. 
//(See cardtest_councilroom.c for more details.)
#define COUNCIL_ROOM_CALLS 1

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

int _cardtest_councilroomhelper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int noCopper, int testNumber);


#endif
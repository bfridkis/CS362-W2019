/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *               unittest2.c
 *
 *	     Test for "buyCard" function.
 *
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c _unittest2helper.o dominion.o cardEffects.o rngs.o
 * 		gcc -o unittest2 unittest2.c -g dominion.o cardEffects.o 
 * 		_unittest2helper.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_unittest2helper.h"

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in unittest2helper.c also!
#define MAX_FAILS 400

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[501];
	int failedTestCount = 0;

	//Kingdom card types for test game
	int k[10] = {treasure_map, gardens, great_hall, cutpurse, adventurer, smithy,
		   baron, outpost, embargo, remodel};
	   
	printf("Starting unittest2 - Testing 'buyCard' function\n");

	//Initializes game for two players, with seed value of 5000.
	initializeGame(2, k, 5000, &G);
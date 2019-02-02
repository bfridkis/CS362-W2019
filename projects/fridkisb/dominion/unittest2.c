/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                         unittest2.c
 *
 *	     Test for "buyCard" function - See dominion.c line 273
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
#define MAX_FAILS 10

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failedTestCount = 0;

	//Kingdom card types for test game
	int k[10] = {adventurer, ambassador, embargo, village, minion, mine, cutpurse,
		   sea_hag, tribute, smithy};
	   
	printf("Starting unittest2 - Testing 'buyCard' function\n");

	//Initializes game for two players, with seed value of 5000.
	initializeGame(2, k, 5000, &G);
	
	//Test case 1 tests buyCard functionality under normal circumstances.
	//For each of the 10 kingdom cards listed in k[], exhaust
	//the supply pile.
	printf("\n\nTesting buyCard with sufficient buys and coins for 10 kingdom cards");
	int failCtT1 = _unittest2helper(1, k, &G, failures, &failedTestCount);
	if(failCtT1){
		printf("\n%d tests failed for sufficient coins and sufficient buys\n", failCtT1);
	}
	else{
		printf("\n**All tests for sufficient coins and sufficient buys passed!**\n");
	}
	
	printf("\n\nTOTAL FAILED TESTS AFTER T1: %d\n", failedTestCount);
	
	
	//Re-initializes game for test case 2
	initializeGame(2, k, 5000, &G);
	
	//Test case 2 tests buy attempts with insufficient coins
	//(for each kingdom card).
	printf("\n\nTesting buyCard with insufficient coins and sufficient buys for 10 kingdom cards");
	int failCtT2 = _unittest2helper(2, k, &G, failures, &failedTestCount);
	if(failCtT1){
		printf("\n%d tests failed for insufficient coins and sufficient buys\n", failCtT2);
	}
	else{
		printf("\n**All tests for insufficient coins and sufficient buys passed!**\n");
	}
	
	printf("\n\nTOTAL FAILED TESTS AFTER T2: %d\n", failedTestCount);
	
	//Re-initializes game for test case 3
	initializeGame(2, k, 5000, &G);
	
	//Test case 3 tests buy attempts with insufficient buys
	//(for each kingdom card).
	printf("\n\nTesting buyCard with sufficient coins and insufficient buys for 10 kingdom cards");
	int failCtT3 = _unittest2helper(3, k, &G, failures, &failedTestCount);
	if(failCtT1){
		printf("\n%d tests failed for sufficient coins and insufficient buys\n", failCtT3);
	}
	else{
		printf("\n**All tests for sufficient coins and insufficient buys passed!**\n");
	}
	
	printf("\n\nTOTAL FAILED TESTS AFTER T3: %d\n", failedTestCount);
	
	if(failedTestCount == 0){
		printf("\n\n*****************************\n"
				   "******ALL TESTS PASSED!******\n"
				   "*****************************\n\n");
	}
	
	//Print summary of all failed tests (up to MAX_FAILS)
	else{
		if(failedTestCount < MAX_FAILS + 1){
			printf("\n\n\t%d tests failed, as follows:\n\n", failedTestCount);
		}
		else{
			printf("\n\n\t%d tests failed.\n\n\tFirst %d failures documented below:\n\n"
				   "\t\t-Set MAX_FAILS in unittest2.c\n"
				   "\t\t and _unittest2helper.c to\n"
				   "\t\t print more errors.\n\n",
						failedTestCount, MAX_FAILS);
		}
		printf("(Note: See _unittest2helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failedTestCount && i < MAX_FAILS; i++){
			printf("%d - LINE %d: %s\n\n", 
				i + 1, failures[i].lineNumber, failures[i].description);
		}
		printf("\n\n");
	}
	
	return 0;
}
		
		  
		  
		  
		  
		  
		  
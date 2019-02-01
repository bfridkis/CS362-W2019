/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                         unittest1.c
 *
 *	     Test for "gainCard" function - See dominion.c line 1100
 *
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c _unittest1helper.o dominion.o cardEffects.o rngs.o
 * 		gcc -o unittest1 unittest1.c -g dominion.o cardEffects.o 
 * 		_unittest1helper.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_unittest1helper.h"

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in unittest1helper.c also!
#define MAX_FAILS 500

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[501];

	//Kingdom card types for test game
	int k[10] = {treasure_map, gardens, great_hall, cutpurse, adventurer, smithy,
		   baron, outpost, embargo, remodel};
	   
	printf("Starting unittest1 - Testing 'gainCard' function\n");

	//Initializes game for two players, with seed value of 5000.
	initializeGame(2, k, 5000, &G);
	
	int failCt = _unittest1helper(k, &G, failures);
	
	if(!failCt){
		printf("\n\n*****************************\n"
				   "******ALL TESTS PASSED!******\n"
				   "*****************************\n\n");
	}
	
	//Print summary of all failed tests (up to MAX_FAILS)
	else{
		if(failCt < MAX_FAILS + 1){
			printf("\n\n\t%d tests failed, as follows:\n\n", failCt);
		}
		else{
			printf("\n\n\t%d tests failed.\n\n\tFirst %d failures documented below:\n\n"
				   "\t\t-Set MAX_FAILS in unittest1.c\n"
				   "\t\t and _unittest1helper.c to\n"
				   "\t\t print more errors.\n\n",
						failCt, MAX_FAILS);
		}
		printf("(Note: See _unittest1helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - LINE %d: %s\n\n", 
				i + 1, failures[i].lineNumber, failures[i].description);
		}
		printf("\n\n");
	}
	
	return 0;
}
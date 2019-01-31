/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                         cardtest1.c
 *
 *	     Test for "smithy" card effect - See cardEffects.c line 29
 *
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c _cardtest1helper.o dominion.o cardEffects.o rngs.o
 * 		gcc -o cardtest1 cardtest1.c -g dominion.o cardEffects.o 
 * 		_cardtest1helper.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_cardtest1helper.h"

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in cardtest1helper.c also!
#define MAX_FAILS 500

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[501];

	//Kingdom card types for test game
	int k[10] = {treasure_map, gardens, great_hall, cutpurse, adventurer, smithy,
		   baron, outpost, embargo, remodel};
	   
	printf("Starting cardtest1 - Testing 'gainCard' function\n");

	//Initializes game for two players, with seed value of 5000.
	initializeGame(2, k, 5000, &G);
	
	int failCt = _cardtest1helper(k, &G, failures);
	
	//Print summary of all failed tests (max 500)
	if(!failCt){
		printf("\n\n*****************************\n"
				   "******ALL TESTS PASSED!******\n"
				   "*****************************\n\n");
	}
	else{
		if(failCt < MAX_FAILS + 1){
			printf("\n\n\t%d tests failed, as follows:\n\n", failCt);
		}
		else{
			printf("\n\n\t%d tests failed.\n\n\tFirst %d failures documented below:\n\n",
						failCt, MAX_FAILS);
		}
		printf("(Note: See _cardtest1helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - LINE %d: %s\n\n", 
				i + 1, failures[i].lineNumber, failures[i].description);
		}
		printf("\n\n");
	}
	
	return 0;
}
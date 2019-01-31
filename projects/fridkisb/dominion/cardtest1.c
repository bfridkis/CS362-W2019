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

#define NUM_PLAYERS 2

#define SMITHY_CALLS 100

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("Starting unittest4 - Testing 'updateCoins' function\n");
	
	printf("\nExecuting %d Smithy plays using hands with random assortment of \n"
		   "\t supply cards, with at least 3 in hand...\n", SMITHY_CALLS);

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10];
	for(i = 0; i < SMITHY_CALLS; i++){
		
		//Generate set of 10 random Kingdom cards
		for(j = 0; j < 10; j++){		
			k[j] = Random() * 19 + 7;
		}
		
		//Initializes game for two players
		initializeGame(2, k, Random() * INT_MAX, &G);
		
		//Run updateCoin test on random kingdom card set.
		//(see _unittest4helper for more details)
		_cardtest1helper(k, &G, failures, &failCt, 0, 0);
	}
	
	printf("\nAttempting no treasure cards test...\n");
	
	//Attempting shuffle on empty deck
	initializeGame(NUM_PLAYERS, k, 5000, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 1, 0) != 0){
		printf("\nNo treasure card test failed\n");
	}
	
	printf("\nAttempting no bonus test...\n");
	
	//Attempting shuffle on max deck
	initializeGame(NUM_PLAYERS, k, Random() * INT_MAX, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 0, 1) != 0){
		printf("\nNo bonus test failed\n");
	}
	
	printf("\nAttempting no treasure cards and bonus test...\n");
	
	//Attempting no treasure cards and no bonus test
	initializeGame(NUM_PLAYERS, k, Random() * INT_MAX, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 1, 1) != 0){
		printf("\nNo treasure cards and no bonus test failed\n");
	}
	
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
		printf("(Note: See _unittest4helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - LINE %d: %s\n\n", 
				i + 1, failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}
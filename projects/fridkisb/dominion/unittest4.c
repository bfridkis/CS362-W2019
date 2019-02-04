/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *               unittest4.c
 *
 *	     Test for "updateCoins" function - See dominion.c line 1137
 *
 * Include the following lines in your makefile:
 *
 * unittest4: unittest4.c _unittest4helper.o dominion.o cardEffects.o rngs.o
 * 		gcc -o unittest4 unittest4.c -g dominion.o cardEffects.o 
 * 		_unittest4helper.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_unittest4helper.h"

#define NUM_COIN_UPDATES 250

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("Starting unittest4 - Testing 'updateCoins' function\n");
	
	printf("\nExecuting %d coin updates using hands with random assortment of treasure\n"
		   "\tcards and bonus values for each update...\n", NUM_COIN_UPDATES);

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10];
	for(i = 0; i < NUM_COIN_UPDATES; i++){
		
		//Generate set of 10 random Kingdom cards
		for(j = 0; j < 10; j++){		
			k[j] = Random() * 19 + 7;
		}
		
		//Initializes game for two players
		initializeGame(2, k, 5000, &G);
		
		//Run updateCoin test on random kingdom card set.
		//(see _unittest4helper for more details)
		_unittest4helper(k, &G, failures, &failCt, 0, 0, i + 1);
	}
	
	printf("\nAttempting no treasure cards test...\n");
	
	//Attempting shuffle on empty deck
	initializeGame(2, k, 5000, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 1, 
			0, NUM_COIN_UPDATES + 1) != 0){
		printf("\nNo treasure card test failed\n");
	}
	
	printf("\nAttempting no bonus test...\n");
	
	//Attempting shuffle on max deck
	initializeGame(2, k, Random() * INT_MAX, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 0, 
			1, NUM_COIN_UPDATES + 2) != 0){
		printf("\nNo bonus test failed\n");
	}
	
	printf("\nAttempting no treasure cards and bonus test...\n");
	
	//Attempting no treasure cards and no bonus test
	initializeGame(2, k, Random() * INT_MAX, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 1, 
			1, NUM_COIN_UPDATES + 3) != 0){
		printf("\nNo treasure cards and no bonus test failed\n");
	}
	
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
				   "\t\t-Set MAX_FAILS in _unittest4helper.h\n"
				   "\t\t to print more errors.\n\n",
						failCt, MAX_FAILS);
		}
		printf("(Note: See _unittest4helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - TEST #%d @ LINE %d: %s\n\n", 
				i + 1, failures[i].testNumber, 
				failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}

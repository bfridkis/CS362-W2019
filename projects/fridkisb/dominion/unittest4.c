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

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("\t\t\t\t\t" "Starting unittest4 - Testing 'updateCoins' function\n");
	
	if(RANDOMIZE){
		printf("\n  Executing %d coin updates using hands with random assortment of treasure\n"
			   "\t cards and bonus values for each update...\n", NUM_COIN_UPDATES);
	}
	else{	   
		printf("\n" "  Executing %d coin updates using hands with equal distribution\n"
			   "\t"  " of treasure cards (i.e. copper count = silver count = gold count).\n"
			   "\t"  " Total treasure card count starts at 3 and increments by 3 for each\n"
			   "\t"  " successive (non-boundary) test. If the treasure card count exceeds\n"
			   "\t"  " %d (MAX_HAND) when calculated this way, use the next lowest multiple of 3\n"
			   "\t"  " that is greater than or equal to the current test number %% %d, and if this\n"
			   "\t"  " value exceeds %d or is 0, use 3.\n\n"
			   "\t\t"  "-e.g. treasure card count for test 1 = 3, test 2 = 6, test 3 = 9...\n"
			   "\t\t"  "      treasure card count for test 101 with MAX_HAND @ 75 = 27\n\n"
			   "  Kingdom cards are adventurer through great_hall, as enumerated in dominion.h.\n\n"
			   "\t" "  Bonus value for each update is the current\n"
			   "\t" "  test number * 2.\n\n"
			   "\t\t"  "-Set 'NUM_COIN_UPDATES' in _unittest4helper.h\n"
			   "\t\t"  " to modify number of tests.\n\n"
			   "\t\t"  "-Random test generator can be turned on\n"
			   "\t\t"  " by setting the constant 'RANDOMIZE' to 1\n"
			   "\t\t"  " in _unittest4helper.h.\n\n", NUM_COIN_UPDATES, 
			   MAX_HAND, MAX_HAND, MAX_HAND);
	}			

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10], m, seed = 5000;
	for(i = 0; i < NUM_COIN_UPDATES; i++){
		
		if(RANDOMIZE){
			//Generate set of 10 random Kingdom cards
			for(j = 0; j < 10; j++){		
				k[j] = Random() * 19 + 7;
			}
			seed = Random() * INT_MAX;
		}
		else{
			for(m = 0, j = 7; j < 17; m++, j++){		
				k[m] = j;
			}
		}
		
		//Initializes game for two players
		initializeGame(2, k, seed, &G);
		
		//Run updateCoin test on random kingdom card set.
		//(see _unittest4helper for more details)
		_unittest4helper(k, &G, failures, &failCt, 0, 0, i + 1);
	}
	
	printf("\n  Attempting no treasure cards test...\n");
	
	//Attempting shuffle on empty deck
	initializeGame(2, k, 5000, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 1, 
			0, NUM_COIN_UPDATES + 1) != 0){
		printf("\n\tNo treasure card test failed\n");
	}
	
	printf("\n  Attempting no bonus test...\n");
	
	//Attempting shuffle on max deck
	initializeGame(2, k, Random() * INT_MAX, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 0, 
			1, NUM_COIN_UPDATES + 2) != 0){
		printf("\n\tNo bonus test failed\n");
	}
	
	printf("\n  Attempting no treasure cards and no bonus test...\n");
	
	//Attempting no treasure cards and no bonus test
	initializeGame(2, k, Random() * INT_MAX, &G);
	if(_unittest4helper(k, &G, failures, &failCt, 1, 
			1, NUM_COIN_UPDATES + 3) != 0){
		printf("\n\tNo treasure cards and no bonus test failed\n");
	}
	
	printf("\n\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ RESULTS SUMMARY ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	
	if(!failCt){
		printf("\n\n\t\t\t\t\t\t\t*****************************\n"
				   "\t\t\t\t\t\t\t***** ALL TESTS PASSED! *****\n"
				   "\t\t\t\t\t\t\t*****************************\n\n");
	}
	
	//Print summary of all failed tests (up to MAX_FAILS)
	else{
		if(failCt < MAX_FAILS + 1){
			printf("\n\n\t%d test%s failed, as follows:\n\n", failCt, 
				failCt > 0 ? "s" : "");
		}
		else{
			printf("\n\n\t%d test%s failed.\n\n\tFirst %d failures documented below:\n\n"
				   "\t\t-Set MAX_FAILS in _unittest4helper.h\n"
				   "\t\t to print more errors.\n\n",
						failCt, failCt > 0 ? "s" : "", MAX_FAILS);
		}
		printf("  (Note: See _unittest4helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - TEST #%d @ LINE %d: %s\n\n", 
				i + 1, failures[i].testNumber, 
				failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}

/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                         cardtest2.c
 *
 *	     Test for "adventurer" card effect - See cardEffects.c line 5
 *
 * Include the following lines in your makefile:
 *
 * cardtest2: cardtest2.c _cardtest2helper.o dominion.o cardEffects.o rngs.o
 * 		gcc -o cardtest2 cardtest2.c -g dominion.o cardEffects.o 
 * 		_cardtest2helper.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_cardtest2helper.h"

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in cardtest2helper.c also!
#define MAX_FAILS 10

#define NUM_PLAYERS 2

#define ADVENTURER_CALLS 1

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("Starting cardtest2 - Testing 'adventurer' card\n");
	
	printf("\nExecuting %d Adventurer plays using hands with random assortment of \n"
		   "\t supply cards, with at least 2 treasures in deck...\n", ADVENTURER_CALLS);

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10];
	for(i = 0; i < ADVENTURER_CALLS; i++){
		
		//Generate set of 10 random Kingdom cards
		for(j = 0; j < 10; j++){		
			k[j] = Random() * 19 + 7;
		}
		
		//Initializes game for two players
		initializeGame(2, k, Random() * INT_MAX, &G);
		
		//Play Adventurer with random kingdom card set with a deck
		//containing at least 2 treasure cards.
		//(see _cardtest2helper for more details)
		_cardtest2helper(k, &G, failures, &failCt, 2);
	}
	
	printf("\nBOUNDARY: Executing Adventurer play using hand with random assortment of \n"
		   "\t supply cards, with 1 treasure card in deck...\n");
	
	//Test with only 1 treasure card in the player's deck
	//Generate set of 10 random Kingdom cards
	for(j = 0; j < 10; j++){		
		k[j] = Random() * 19 + 7;
	}
	initializeGame(2, k, Random() * INT_MAX, &G);
	_cardtest2helper(k, &G, failures, &failCt, 1);
	
	printf("\nBOUNDARY: Executing Adventurer play using hand with random assortment of \n"
		   "\t supply cards, with 0 treasure cards in deck...\n");
	
	//Test with only 2 cards in the player's deck
	//Generate set of 10 random Kingdom cards
	for(j = 0; j < 10; j++){		
		k[j] = Random() * 19 + 7;
	}
	initializeGame(2, k, Random() * INT_MAX, &G);
	_cardtest2helper(k, &G, failures, &failCt, 0);
	
	printf("\n\tEach test (that is not marked 'BOUNDARY') verifies proper game state\n"
		   "\tmodification, reporting a failure if any of the following conditions are NOT met:\n"
		   "\t\t"       "1. Current player variable (whoseTurn) is unchanged\n"
		   "\t\t"       "2. Player's hand gains only 2 treasure cards from deck\n"
		   "\t\t\t"         "a. Same 2 treasure card types are removed from deck and added to hand.\n"
		   "\t\t\t"         "b. Deck count and hand counts are updated correctly\n"
		   "\t\t"       "3. Hand does not gain any non-treasure cards\n"
		   "\t\t"       "4. Discard does not gain any treasure cards\n"
		   "\t\t"       "5. All non-treasure cards removed from deck are added to hand\n"
		   "\t\t"       "6. All of the following game states are unchanged:\n"
		   "\t\t\t"          "a. coins\n"
		   "\t\t\t"          "b. numActions\n"
		   "\t\t\t"          "c. numBuys\n"
		   "\t\t\t"          "d. embargoTokens[]\n"
		   "\t\t\t"          "e. outpostPlayed\n"
		   "\t\t\t"          "f. outpostTurn\n"
		   "\t\t"       "7. Played card count is unchanged\n"
		   "\t\t"       "8. Played cards is unchanged\n"
		   "\n"
		   "\t\t"       "* BOUNDARY tests verify all of the above except for #2a, where instead *\n"
		   "\t\t"       "* the boundary case checks for the appropriate number of treasure card *\n"
		   "\t\t"       "* removals and insertions based on the desiganted treasure card amount *\n"
		   "\t\t"       "*  i.e. As is passed in via the 'treasureCardCountSpecifier' variable. *\n");

	
	
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
				   "\t\t-Set MAX_FAILS in cardtest2.c\n"
				   "\t\t and _cardtest2helper.c to\n"
				   "\t\t print more errors.\n\n",
						failCt, MAX_FAILS);
		}
		printf("(Note: See _cardtest2helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - LINE %d: %s\n\n", 
				i + 1, failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}
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

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("\t\t\t\t\t" "Starting cardtest1 - Testing 'smithy' card\n");
	
	if(RANDOMIZE){
		printf("\n  Executing %d Smithy %s using decks and hands with random assortment of \n"
			   "\t  supply cards, with at least 3 cards active player's in deck...\n\n"
			   "\t\t" " -Set 'SMITHY_CALLS' in _cardtest1helper.h\n"
			   "\t\t" "  to modify number of plays.\n", SMITHY_CALLS,
			   SMITHY_CALLS > 1 ? "plays" : "play");
	}
	else{
		printf("\n" "  Executing %d Smithy %s using deck and hand sizes starting at 5 and incrementing\n"
			   "\t" "  by multiples of 5 with each successive test number, up to %d/%d (MAX_DECK/MAX_HAND).\n"
			   "\t" "  If the number of tests cause the deck/hand size to equal or exceed %d/%d when\n"
			   "\t" "  calculated in this way, the deck/hand size will reset to 1 and begin\n"
			   "\t" "  incrementing by 1 with each additional successive test.\n\n"
			   "\t\t" "-e.g. deck size for test 1 = 5, test 2 = 10, test 3 = 15...\n"
			   "\t\t" "      deck size for test 100 with MAX_DECK @ 500 = 1, test 101 = 2...\n\n"
			   "  Kingdom cards are adventurer through great_hall, as enumerated in dominion.h.\n\n"
			   "\t\t" " -Set 'SMITHY_CALLS' in cardtest1.c\n"
			   "\t\t" "   to modify number of plays.\n\n"
			   "\t\t" " -Random test generator can be turned on\n"
			   "\t\t" "  by setting the constant 'RANDOMIZE' to 1\n"
			   "\t\t" "  in _cardtest1helper.h.\n\n", SMITHY_CALLS, 
			   SMITHY_CALLS > 1 ? "plays" : "play", MAX_DECK, MAX_HAND, MAX_DECK, MAX_HAND);
	}

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10], m, seed = 5000;
	for(i = 0; i < SMITHY_CALLS; i++){
		
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
		
		//Initializes game for two players with random seed value.
		initializeGame(2, k, seed, &G);
		
		//Play Smithy with random kingdom card set.
		//(see _cardtest1helper for more details)
		_cardtest1helper(k, &G, failures, &failCt, 3, 0, i + 1);
	}
	
	if(RANDOMIZE){
		printf("\n  BOUNDARY: Executing Smithy play using hand with random assortment of \n"
			   "\t supply cards, with 1 card in deck...\n");
	}
	else{
		printf("\n  BOUNDARY: Executing Smithy play using hand with only 1 card in deck...\n");
	}
	
	//Test with only 1 card in the player's deck
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
	initializeGame(2, k, seed, &G);
	_cardtest1helper(k, &G, failures, &failCt, 1, 1, SMITHY_CALLS + 1);
	
	if(RANDOMIZE){
		printf("\n  BOUNDARY: Executing Smithy play using hand with random assortment of \n"
			   "\t            supply cards, with 2 cards in deck...\n");
	}
	else{
		printf("\n  BOUNDARY: Executing Smithy play with only 2 cards in deck...\n");
	}
	
	//Test with only 2 cards in the player's deck
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
	initializeGame(2, k, seed, &G);
	_cardtest1helper(k, &G, failures, &failCt, 2, 1, SMITHY_CALLS + 2);
	
	if(RANDOMIZE){
		printf("\n  BOUNDARY: Executing Smithy play using hand with random assortment of \n"
			   "\t            supply cards, with 0 cards in deck...\n");
	}
	else{
		printf("\n  BOUNDARY: Executing Smithy play using hand with 0 cards in deck...\n");
	}
	
	//Test with 0 cards in the player's deck
	//Generate set of 10 random Kingdom cards
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
	initializeGame(2, k, seed, &G);
	_cardtest1helper(k, &G, failures, &failCt, 0, 1, SMITHY_CALLS + 3);
	
	printf("\n\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ RESULTS SUMMARY ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	
	printf("\n\t" "Each test (that is not marked 'BOUNDARY') verifies proper game state\n"
		   "\t"   "modification, reporting a failure if any of the following conditions are met:\n"
		   "\t\t"       "1. Current player variable (whoseTurn) is changed\n"
		   "\t\t"       "2. Active player's hand does not gain 3 cards from the top of Player's deck\n"
		   "\t\t\t"         "a. Active player's deck count is not decremented by 3.\n"
		   "\t\t\t"         "b. Active player's hand count is not incremented by 2\n"
		   "\t\t\t\t\t"          "(A total of 3 cards are gained but the Smithy\n"
		   "\t\t\t\t\t"          "itself is discarded so the net gain from the previous\n"
		   "\t\t\t\t\t"          "is 2.)\n"
		   "\t\t\t"         "c. Active player's deck contents are not the same before and after\n"
		   "\t\t\t\t\t"			 "the play. (Besides the 3 less cards gained therefrom.\n"
		   "\t\t\t\t\t\t\t\t"			 "i.e. The deck order is not changed.)\n"
		   "\t\t\t"         "d. Active player's hand order and content are changed\n"
		   "\t\t\t\t\t"          "(Besides the removal/insertion of the Smithy played/last card drawn\n"
		   "\t\t\t\t\t"          " and 2 additional cards gained to the end of the hand\n"
		   "\t\t"       "4. Active player's discard pile does not gain a Smithy, or count is not 1\n"
		   "\t\t\t\t"       "(starts at 0)\n"
		   "\t\t"       "5. Inactive player's discard piles are changed, or count is not 0 (starts at 0)\n"
		   "\t\t"       "4. Inactive player's deck, hand, deck count, and/or hand count is changed\n"
		   "\t\t"       "5. Any supply pile count (curses, victory cards, or kingdom cards) is changed\n"
		   "\t\t"       "6. Any of the following game states are changed:\n"
		   "\t\t\t"          "a. coins\n"
		   "\t\t\t"          "b. numActions\n"
		   "\t\t\t"          "c. numBuys\n"
		   "\t\t\t"          "d. embargoTokens[]\n"
		   "\t\t\t"          "e. outpostPlayed\n"
		   "\t\t\t"          "f. outpostTurn\n"
		   "\t\t"       "7. Played card count is not 1\n"
		   "\t\t"       "8. Played cards does not have Smithy @ idx 1 and is unchanged otherwise\n"
		   "\n"
		   "\t\t"       "* BOUNDARY tests verify all of the above except for #2a & #2b. #2c verifies    *\n"
		   "\t\t"       "* that only the number of starting deck cards is decremented (since this value *\n"
		   "\t\t"       "* is < 3), and #2d verifies that hand only increases by deck starting count in *\n"
		   "\t\t"		"* likewise.                                                                    *\n");

	
	
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
				   "\t\t-Set MAX_FAILS in _cardtest1helper.h\n"
				   "\t\t to print more errors.\n\n",
						failCt, failCt > 0 ? "s" : "", MAX_FAILS);
		}
		printf("  (Note: See _cardtest1helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - TEST #%d @ LINE %d: %s\n\n", 
				i + 1, failures[i].testNumber, 
				failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}

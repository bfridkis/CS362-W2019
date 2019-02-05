/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                         cardtest4.c
 *
 *	     Test for "council_room" card effect - See cardEffects.c line 29
 *
 * Include the following lines in your makefile:
 *
 * cardtest4: cardtest4.c _cardtest4helper.o dominion.o cardEffects.o rngs.o
 * 		gcc -o cardtest4 cardtest4.c -g dominion.o cardEffects.o 
 * 		_cardtest4helper.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_cardtest4helper.h"

#define COUNCIL_ROOM_CALLS 100

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("Starting cardtest4 - Testing 'council_room' card\n");
	
	printf("\nExecuting %d Council_Room %s using hands with random assortment of \n"
		   "\t supply cards, with at least 3 in deck...\n\n"
		   "\t\t" "-Set 'COUNCIL_ROOM_CALLS' in cardtest4.c\n"
		   "\t\t" " to modify number of plays.\n", COUNCIL_ROOM_CALLS,
		   COUNCIL_ROOM_CALLS > 1 ? "plays" : "play");

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10];
	for(i = 0; i < COUNCIL_ROOM_CALLS; i++){
		
		//Generate set of 10 random Kingdom cards
		for(j = 0; j < 10; j++){		
			k[j] = Random() * 19 + 7;
		}
		
		//Initializes game for two players
		initializeGame(2, k, Random() * INT_MAX, &G);
		
		//Play Council_Room with random kingdom card set.
		//(see _cardtest4helper for more details)
		_cardtest4helper(k, &G, failures, &failCt, 3, i + 1);
	}
	
	printf("\nBOUNDARY: Executing Council_Room play using hand with random assortment of \n"
		   "\t supply cards, with 1 card in deck...\n");
	
	//Test with only 1 card in the player's deck
	//Generate set of 10 random Kingdom cards
	for(j = 0; j < 10; j++){		
		k[j] = Random() * 19 + 7;
	}
	initializeGame(2, k, Random() * INT_MAX, &G);
	_cardtest4helper(k, &G, failures, &failCt, 1, COUNCIL_ROOM_CALLS + 1);
	
	printf("\nBOUNDARY: Executing Council_Room play using hand with random assortment of \n"
		   "\t supply cards, with 2 cards in deck...\n");
	
	//Test with only 2 cards in the player's deck
	//Generate set of 10 random Kingdom cards
	for(j = 0; j < 10; j++){		
		k[j] = Random() * 19 + 7;
	}
	initializeGame(2, k, Random() * INT_MAX, &G);
	_cardtest4helper(k, &G, failures, &failCt, 2, COUNCIL_ROOM_CALLS + 2);
	
	printf("\nBOUNDARY: Executing Council_Room play using hand with random assortment of \n"
		   "\t supply cards, with 0 cards in deck...\n");
	
	//Test with 0 cards in the player's deck
	//Generate set of 10 random Kingdom cards
	for(j = 0; j < 10; j++){		
		k[j] = Random() * 19 + 7;
	}
	initializeGame(2, k, Random() * INT_MAX, &G);
	_cardtest4helper(k, &G, failures, &failCt, 0, COUNCIL_ROOM_CALLS + 3);
	
	printf("\n\tEach test (that is not marked 'BOUNDARY') verifies proper game state\n"
		   "\tmodification, reporting a failure if any of the following conditions are met:\n"
		   "\t\t"       "1. Current player variable (whoseTurn) is changed\n"
		   "\t\t"       "2. Player's hand does not gain 3 cards from the top of Player's deck\n"
		   "\t\t\t"         "a. Player's deck count is not decremented by 3.\n"
		   "\t\t\t"         "b. Player's hand count is not incremented by 2\n"
		   "\t\t\t\t\t"          "(A total of 3 cards are gained but the Council_Room\n"
		   "\t\t\t\t\t"          "itself is discarded so the net gain from the previous\n"
		   "\t\t\t\t\t"          "is 2.)\n"
		   "\t\t\t"         "c. Player's deck contents are not the same before and after the play\n"
		   "\t\t\t\t\t"          "(Besides the 3 less cards gained therefrom)\n"
		   "\t\t\t"         "d. Player's hand order and content are changed\n"
		   "\t\t\t\t\t"          "(Besides the removal/insertion of the Council_Room played/last card drawn\n"
		   "\t\t\t\t\t"          " and 2 additional cards gained to the end of the hand\n"
		   "\t\t"       "3. Player 1's or 2's discard pile or count is changed\n"
		   "\t\t"       "4. Player 2's deck, hand, deck count, and/or hand count is changed\n"
		   "\t\t"       "5. Any supply pile count (curses, victory cards, or kingdom cards) is changed\n"
		   "\t\t"       "6. Any of the following game states are changed:\n"
		   "\t\t\t"          "a. coins\n"
		   "\t\t\t"          "b. numActions\n"
		   "\t\t\t"          "c. numBuys\n"
		   "\t\t\t"          "d. embargoTokens[]\n"
		   "\t\t\t"          "e. outpostPlayed\n"
		   "\t\t\t"          "f. outpostTurn\n"
		   "\t\t"       "7. Played card count is not 1\n"
		   "\t\t"       "8. Played cards does not have Council_Room @ idx 1 and is unchanged otherwise\n"
		   "\n"
		   "\t\t"       "* BOUNDARY tests verify all of the above except for #2a & #2b. #2c verifies    *\n"
		   "\t\t"       "* that only the number of starting deck cards is decremented (since this value *\n"
		   "\t\t"       "* is < 3), and #2d verifies that hand only increases by deck starting count in *\n"
		   "\t\t"		"* likewise.                                                                    *\n");

	
	
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
				   "\t\t-Set MAX_FAILS in _cardtest4helper.h\n"
				   "\t\t to print more errors.\n\n",
						failCt, MAX_FAILS);
		}
		printf("(Note: See _cardtest4helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - TEST #%d @ LINE %d: %s\n\n", 
				i + 1, failures[i].testNumber, 
				failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}
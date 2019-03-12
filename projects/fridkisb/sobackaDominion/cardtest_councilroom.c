/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                         cardtest_councilroom.c
 *
 *	     Test for "council_room" card effect - See dominion.c line 697
 *
 * Include the following lines in your makefile:
 *
 * cardtest_councilroom: cardtest_councilroom.c _cardtesthelper_councilroom.o \
 *						 dominion.o rngs.o
 * 		gcc -o cardtest_councilroom cardtest_councilroom.c -g dominion.o \
 * 		_cardtesthelper_councilroom.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_cardtesthelper_councilroom.h"

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("\t\t\t\t\t" "Starting cardtest_councilroom - Testing 'council_room' card\n");
	
	if(RANDOMIZE){
		printf("\n  Executing %d Council_Room %s using hands with random assortment of \n"
			   "\t  supply cards, with at least 1 in deck for non-active players,\n"
			   "\t  and 4 in deck for active players.\n\n"
			   "\t\t" " -Set 'COUNCIL_ROOM_CALLS' in cardtest_councilroom.c\n"
			   "\t\t" "  to modify number of plays.\n", COUNCIL_ROOM_CALLS,
			   COUNCIL_ROOM_CALLS > 1 ? "plays" : "play");
	}
	else{
	printf("\n" "  Executing %d Council_Room %s using deck and hand sizes starting at 5 and incrementing\n"
		   "\t" "  by multiples of 5 with each successive test number, up to %d (MAX_DECK).\n"
		   "\t" "  If the number of tests cause the deck size to exceed %d when\n"
		   "\t" "  calculated in this way, the deck size will reset to 1 and begin incrementing\n"
		   "\t" "  by 1 with each additional successive test, but if the active player's\n\n"
		    "\t" "  deck size < 4, active player's deck size will be set to 6 (to ensure enough draw cards).\n"
		   "\t\t" "-e.g. deck size for test 1 = 5, test 2 = 10, test 3 = 15...\n"
		   "\t\t" "      deck size for test 100 with MAX_DECK @ 500 = 1, test 101 = 2...\n\n"
		   "  Kingdom cards are adventurer through great_hall, as enumerated in dominion.h.\n\n"
		   "\t\t" " -Set 'COUNCIL_ROOM_CALLS' in cardtest_smithy.c\n"
		   "\t\t" "  to modify number of plays.\n\n"
		   "\t\t" " -Random test generator can be turned on\n"
		   "\t\t" "  by setting the constant 'RANDOMIZE' to 1\n"
		   "\t\t" "  in _cardtesthelper_councilroom.h.\n\n", COUNCIL_ROOM_CALLS, 
		   COUNCIL_ROOM_CALLS > 1 ? "plays" : "play", MAX_DECK, MAX_DECK);
	}

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10], m, seed = 5000;
	for(i = 0; i < COUNCIL_ROOM_CALLS; i++){
		
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
		initializeGame(NUM_PLAYERS, k, seed, &G);
		
		//Play Council_Room with random kingdom card set.
		//(see _cardtesthelper_councilroom for more details)
		_cardtesthelper_councilroom(k, &G, failures, &failCt, 0, i + 1);
	}
	
	if(RANDOMIZE){
		printf("\n  BOUNDARY: Executing Council_Room play using hand with random assortment of\n"
			   "\t            supply cards, with empty decks for all players...\n");
	}
	else{
		printf("\n  BOUNDARY: Executing Council_Room play using hands\n"
			   "\t          with empty decks for all players...\n");
	}
	
	//Test with empty decks (all players)
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
	initializeGame(NUM_PLAYERS, k, seed, &G);
	_cardtesthelper_councilroom(k, &G, failures, &failCt, 1, COUNCIL_ROOM_CALLS + 1);
	
	printf("\n\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ RESULTS SUMMARY ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	
	printf("\n\tEach test (that is not marked 'BOUNDARY') verifies proper game state\n"
		   "\tmodification, reporting a failure if any of the following conditions are met:\n"
		   "\t\t"       "1. Current player variable (whoseTurn) is changed\n"
		   "\t\t"       "2. Active player's hand does not gain 4 cards from the top of active player's deck\n"
		   "\t\t\t"         "a. Active player's deck count is not decremented by 4.\n"
		   "\t\t\t"         "b. Active player's hand count is not incremented by 3\n"
		   "\t\t\t\t\t"          "(A total of 4 cards are gained from the deck but the Council_Room\n"
		   "\t\t\t\t\t"          "itself is discarded so the net gain from the hand should be 3.)\n"
		   "\t\t\t"         "c. Active player's deck contents are not the same before and after the play\n"
		   "\t\t\t\t\t"          "(Besides the 4 less cards gained therefrom.\n"
		   "\t\t\t\t\t\t"				"i.e. The deck order is not changed.)\n"
		   "\t\t\t"         "d. Active player's hand content is changed\n"
		   "\t\t\t\t\t"          "(Besides the 4 additional cards gained to the end of the hand from\n"
		   "\t\t\t\t\t\t"		 	"the top of the deck)\n"
		   "\t\t"       "3. Inactive players' hands do not gain 1 card from the top of each respective\n"
		   "\t\t\t"				"player's own deck\n"
		   "\t\t\t"         "a. Inactive players' deck counts are not decremented by 1.\n"
		   "\t\t\t"         "b. Inactive players' hand counts are not incremented by 1\n"
		   "\t\t\t"         "c. Inactive players' deck contents are not the same before and after the play\n"
		   "\t\t\t\t\t"          "(Besides the 1 less cards gained therefrom.\n"
		   "\t\t\t\t\t\t"				"i.e. The deck order is not changed.)\n"
		   "\t\t\t"         "d. Inactive players' hand contents are changed\n"
		   "\t\t\t\t\t"          "(Besides the additional card gained to the end of the hand from the top\n"
		   "\t\t\t\t\t\t"				"of the deck\n"
		   "\t\t"       "4. Active player's discard pile does not gain a council_room, or count is not 1\n"
		   "\t\t\t\t"				"(starts at 0)\n"
		   "\t\t"       "5. Inactive players' discard piles are changed, or counts are not 0\n"
		   "\t\t\t\t"				"(all start at 0)\n"
		   "\t\t"       "6. numBuys game state is not incremented by 1 (starts at 0)\n"
		   "\t\t"       "7. Any supply pile count (curses, victory cards, or kingdom cards) is changed\n"
		   "\t\t"       "8. Any of the following game states are changed:\n"
		   "\t\t\t"          "a. coins\n"
		   "\t\t\t"          "b. numActions\n"
		   "\t\t\t"          "c. embargoTokens[]\n"
		   "\t\t\t"          "d. outpostPlayed\n"
		   "\t\t\t"          "e. outpostTurn\n"
		   "\t\t"       "9. Played card count is not 1\n"
		   "\t\t"       "10. Played cards does not have Council_Room @ idx 1 and is unchanged otherwise\n"
		   "\n"
		   "\t\t"       "* BOUNDARY test verifies all of the above except for #2 & #3. For the empty    *\n"
		   "\t\t"       "* deck boundary test, no deck or hand count or contents should change for any  *\n"
		   "\t\t"		"* player, with the one exception of the active player's hand, which should be  *\n"
		   "\t\t"       "* decremented by exactly 1 council_room.                                       *\n");

	
	
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
		printf("  (Note: See _cardtesthelper_councilroom.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - TEST #%d @ LINE %d: %s\n\n", 
				i + 1, failures[i].testNumber, 
				failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}
/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 4
 *
 *                         randomtestcard2.c
 *
 *	     Test for "cutpurse" card effect - See cardEffects.c line 5
 *
 * Include the following lines in your makefile:
 *
 * randomtestcard2: randomtestcard2.c _randomtestcard2helper.o dominion.o \
					cardEffects.o rngs.o
 * 		gcc -o randomtestcard2 randomtestcard2.c -g dominion.o cardEffects.o 
 * 		_randomtestcard2helper.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_randomtestcard2helper.h"

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("\t\t\t\t " "Starting randomtestcard2 - Testing 'cutpurse' card\n");
	
	if(RANDOMIZE){
		printf("\n  Executing %d Cutpurse %s using hands with random assortment of \n"
			   "\t  cards for each player, with at least 1 copper in each hand...\n\n"
			   "\t  Hand and deck sizes are random for each player, up to sizes MAX_HAND\n"
			   "\t  and MAX_DECK respectively. The number of players is random up to MAX_PLAYER.\n"
			   "\t  The active player is chosen randomly, as well as the starting hand position\n"
			   "\t  for the active player's council_room card. The entire game state is randomized\n"
			   "\t  before play, and only pertinent data structures are initialized to known values\n"
			   "\t  thereafter. The seed value for the random number generator is based on the\n"
			   "\t  system clock to ensure non-determinism.\n\n"
			   "\t\t" " -Set 'CUTPURSE_CALLS' in _randomtestcard2helper.h\n"
			   "\t\t" "  to modify number of plays.\n", CUTPURSE_CALLS,
			   CUTPURSE_CALLS > 1 ? "plays" : "play");
	}
	else{
		printf("\n" "  Executing %d Cutpurse %s using hand sizes starting at 5 and incrementing\n"
			   "\t" "  by multiples of 5 with each successive test number, up to %d (MAX_HAND).\n"
			   "\t" "  If the number of tests cause the hand size to exceed %d when\n"
			   "\t" "  calculated in this way, the hand size will reset to 1 and begin\n"
			   "\t" "  incrementing by 1 with each additional successive test.\n\n"
			   "\t\t" "-e.g. hand size for test 1 = 5, test 2 = 10, test 3 = 15...\n"
			   "\t\t" "      hand size for test 100 with MAX_HAND @ 500 = 1, test 101 = 2...\n\n"
			   "  Kingdom cards are adventurer through great_hall, as enumerated in dominion.h.\n\n"
			   "\t\t" " -Set 'CUTPURSE_CALLS' in randomtestcard2.c\n"
			   "\t\t" "  to modify number of plays.\n\n"
			   "\t\t" " -Random test generator can be turned on\n"
			   "\t\t" "  by setting the constant 'RANDOMIZE' to 1\n"
			   "\t\t" "  in _randomtestcard2helper.h.\n\n", CUTPURSE_CALLS, 
			   CUTPURSE_CALLS > 1 ? "plays" : "play", MAX_HAND, MAX_HAND);
	}

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10], m, seed = 5000, numPlayers;
	for(i = 0; i < CUTPURSE_CALLS; i++){
		
		if(RANDOMIZE){
			//Generate set of 10 random Kingdom cards
			for(j = 0; j < 10; j++){		
				k[j] = Random() * 19 + 7;
			}
			seed = Random() * INT_MAX;
			
			//Generate random player count between 2 and MAX_PLAYERS
			numPlayers = floor(Random() * (MAX_PLAYERS - 1)) + 2;
		}
		else{
			for(m = 0, j = 7; j < 17; m++, j++){		
				k[m] = j;
			}
			numPlayers = NUM_PLAYERS;
		}
		
		//Initializes game for two players with random seed value.
		initializeGame(numPlayers, k, seed, &G);
		
		//Play Cutpurse with random kingdom card set with a deck
		//containing at least 2 treasure cards.
		//(see _randomtestcard2helper for more details)
		_randomtestcard2helper(numPlayers, k, &G, failures, &failCt, 0, i + 1);
	}
	
	if(RANDOMIZE){
		printf("\n  BOUNDARY: Executing Cutpurse play using random game state as described\n"
			   "\t            above but with no copper cards in hands (all players...\n");
	}
	else{
		printf("\n  BOUNDARY: Executing Cutpurse play using hands with no copper (all players)...\n");
	}
	
	//Test with no copper cards in each player's hand
	if(RANDOMIZE){
		//Generate set of 10 random Kingdom cards
		for(j = 0; j < 10; j++){		
			k[j] = Random() * 19 + 7;
		}
		seed = Random() * INT_MAX;
		
		//Generate random player count between 2 and MAX_PLAYERS
		numPlayers = floor(Random() * (MAX_PLAYERS - 1)) + 2;
	}
	else{
		for(m = 0, j = 7; j < 17; m++, j++){		
			k[m] = j;
		}
		numPlayers = NUM_PLAYERS;
	}
	initializeGame(numPlayers, k, seed, &G);
	_randomtestcard2helper(numPlayers, k, &G, failures, &failCt, 1, 
		CUTPURSE_CALLS + 1);
	
	printf("\n\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ RESULTS SUMMARY ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	
	printf("\n\tEach test (that is not marked 'BOUNDARY') verifies proper game state\n"
		   "\tmodification, reporting a failure if any of the following conditions are NOT met:\n"
		   "\t\t"       "1.  Current player variable (whoseTurn) is unchanged\n"
		   "\t\t"       "2.  Player who plays cutpurse has same hand after play less 1 cutpurse\n"
		   "\t\t"       "3.  Players who did not play cutpurse have same hand after play less 1 copper\n"
		   "\t\t\t"           "EXCEPTION: Boundary case of no copper in which players who did not play\n"
		   "\t\t\t"           "           copper have same hand after cutpurse is played\n"
		   "\t\t"       "4.  Hand counts are decremented by 1 for each player\n"
		   "\t\t\t\t"          "(1 cutpurse discarded for active player or 1 copper\n" 
		   "\t\t\t\t"           " discarded for inactive players)\n"
		   "\t\t"       "5.  The game state's 'coins' variable is incremented by 2\n"
		   "\t\t"       "6.  All of the following game states are unchanged:\n"
		   "\t\t\t"           "a. numActions\n"
		   "\t\t\t"           "b. numBuys\n"
		   "\t\t\t"           "c. embargoTokens[]\n"
		   "\t\t\t"           "d. outpostPlayed\n"
		   "\t\t\t"           "e. outpostTurn\n"
		   "\t\t\t"           "f. All players' decks are unchanged\n"
		   "\t\t"       "7.  Active player's discard pile gains a cutpurse, and count is updated to 1\n"
		   "\t\t\t\t"              "(starts at 0)\n"
		   "\t\t"       "8.  Inactive players' discard piles are unchanged, and counts remain 0\n"
		   "\t\t\t\t"				"(all start at 0)\n"
		   "\t\t"       "9.  Played card count is incremented by 1 (for cutpurse played)\n"
		   "\t\t"       "10. Played cards gains one and only cutpurse\n");
	
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
		printf("  (Note: See _randomtestcard2helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - TEST #%d @ LINE %d: %s\n\n", 
				i + 1, failures[i].testNumber, 
				failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}

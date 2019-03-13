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

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	printf("\t\t\t\t\t" "Starting cardtest2 - Testing 'adventurer' card\n");
	
	if(RANDOMIZE){
		printf("\n  Executing %d Adventurer %s using hands with random assortment of \n"
			   "\t  supply cards, with at least 2 treasures in deck...\n\n"
				"\t\t" " -Set 'ADVENTURER_CALLS' in _cardtest2helper.h\n"
			   "\t\t"  "  to modify number of plays.\n", ADVENTURER_CALLS,
			   ADVENTURER_CALLS > 1 ? "plays" : "play");
	}
	else{
		printf("\n" "  Executing %d Adventurer %s using deck and hand sizes starting at 6 and incrementing\n"
			   "\t" "  by multiples of 5 with each successive test number, up to %d (MAX_DECK).\n"
			   "\t" "  If the number of tests cause the deck size to equal or exceed %d when\n"
			   "\t" "  calculated in this way, the deck size will reset to 1 and begin\n"
			   "\t" "  incrementing by 1 with each additional successive test, but if\n"
			   "\t" "  hand size < 6, hand size will be set to 6 (see _cardtest2helper.c line 90).\n"
			   "\t\t" "-e.g. deck size for test 1 = 6, test 2 = 11, test 3 = 16...\n"
			   "\t\t" "      deck size for test 100 with MAX_DECK @ 500 = 1, test 101 = 2...\n\n"
			   "  Kingdom cards are adventurer through great_hall, as enumerated in dominion.h.\n\n"
			   "\t\t" " -Set 'ADVENTURER_CALLS' in cardtest2.c\n"
			   "\t\t" "  to modify number of plays.\n\n"
			   "\t\t" " -Random test generator can be turned on\n"
			   "\t\t" "  by setting the constant 'RANDOMIZE' to 1\n"
			   "\t\t" "  in _cardtest2helper.h.\n\n", ADVENTURER_CALLS, 
			   ADVENTURER_CALLS > 1 ? "plays" : "play", MAX_DECK, MAX_DECK);
	}

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10], m, seed = 5000;
	for(i = 0; i < ADVENTURER_CALLS; i++){
		
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
		
		//Play Adventurer with random kingdom card set with a deck
		//containing at least 2 treasure cards.
		//(see _cardtest2helper for more details)
		_cardtest2helper(k, &G, failures, &failCt, 2, 0, i + 1);
	}
	
	if(RANDOMIZE){
		printf("\n  BOUNDARY: Executing Adventurer play using hand with random assortment of \n"
			   "\t            supply cards, with 1 treasure card in deck...\n");
	}
	else{
		printf("\n  BOUNDARY: Executing Adventurer play with only 1 treasure card in deck...\n");
	}
	
	//Test with only 1 treasure card in the player's deck
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
	_cardtest2helper(k, &G, failures, &failCt, 1, 1, ADVENTURER_CALLS + 1);
	
	if(RANDOMIZE){
		printf("\n  BOUNDARY: Executing Adventurer play using hand with random assortment of \n"
			   "\t            supply cards, with 0 treasure cards in deck...\n");
	}
	else{
		printf("\n  BOUNDARY: Executing Adventurer play with 0 treasure cards in deck...\n");
	}
	
	//Test with only 0 treasure cards in the player's deck
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
	_cardtest2helper(k, &G, failures, &failCt, 0, 1, ADVENTURER_CALLS + 2);
	
	if(RANDOMIZE){
		printf("\n  BOUNDARY: Executing Adventurer play using hand with random assortment of \n"
			   "\t            supply cards, with 0 cards in deck (should be same outcome as no\n"
			   "\t			  treasure cards...)\n");
	}
	else{
		printf("\n  BOUNDARY: Executing Adventurer play with 0 cards in deck\n"
				 "            (should be same outcome as no treasure cards...)\n");
	}
	
	//Test with 0 cards in the player's deck
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
	_cardtest2helper(k, &G, failures, &failCt, -1, 1, ADVENTURER_CALLS + 3);
	
	printf("\n\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ RESULTS SUMMARY ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	
	printf("\n\t" "Each test (that is not marked 'BOUNDARY') verifies proper game state\n"
		   "\t"   "modification, reporting a failure if any of the following conditions are NOT met:\n"
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
		   "\t\t"       "*  BOUNDARY tests verify all of the above except for #2a, where instead  *\n"
		   "\t\t"       "*  the boundary case checks for the appropriate number of treasure card  *\n"
		   "\t\t"       "*  removals and insertions based on the designated treasure card amount  *\n"
		   "\t\t"       "*  (i.e. as is passed in via the 'treasureCardCountSpecifier' variable.) *\n"
		   "\t\t"
		   "\n"
		   "\t\t"		"********************* NOTE FOR BOUNDARY CASE / BUG REPORT ****************** \n"
		   "\t\t" 		"**																		   ** \n"
		   "\t\t" 		"** Technically, the Adventurer card does not explicitly state what should ** \n"
		   "\t\t" 		"**    be done in the event that the set of the player's deck and discard  ** \n"
		   "\t\t"		"**     piles do not contain a total of at least 2 treasure cards. For a   ** \n"
		   "\t\t"		"**       'literal minded' machine such as a computer, this creates an     ** \n"
		   "\t\t"		"** infinite loop of sorts, that must be handled accordingly. The findings ** \n"
		   "\t\t"		"**  of this testing exercise have indicated a bug related to this aspect  ** \n"
		   "\t\t"		"**  of adventurer, one which does not result in a stuck loop but rather   ** \n"
		   "\t\t"		"** incorrect behavior in some conditions (i.e. when discard and deck do   ** \n"
		   "\t\t"		"**  not contain at least 2 treasures, but hand has enough to make up the  ** \n" 
		   "\t\t"		"** difference) or a segmentation fault in others (i.e. when there are not ** \n" 
		   "\t\t"		"**   at least 2 treasures between deck, discard, and hand combined). This ** \n"
		   "\t\t"		"**   program (cardtest2) has been deliberately set up to avoid a full on  ** \n"
		   "\t\t"		"**    crash due to this error, so that all remaining tests and code will  ** \n"
		   "\t\t"		"**     be processed to completion. The incorrect behavior (that does not  ** \n"
		   "\t\t" 		"** result in a full crash) caused by this bug is captured in the BOUNDARY ** \n"
		   "\t\t" 		"**  test results. Please see cardEffects.c and the assignment write-up    ** \n"
		   "\t\t" 		"**                  for a complete description of the error.              ** \n"
		   "\t\t" 		"**                                                                        ** \n"
		   "\t\t" 		"**  (The bug mentioned above was NOT introduced as part of assignment 2.  ** \n"
		   "\t\t" 		"**    The test results here should indicate additional findings which     ** \n"
		   "\t\t" 		"**          ARE the result of bugs introduced for assignment 2.)          ** \n" 
		   "\t\t" 		"**                                                                        ** \n"
		   "\t\t" 		"**************************************************************************** \n");
	
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
		printf("  (Note: See _cardtest2helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - TEST #%d @ LINE %d: %s\n\n", 
				i + 1, failures[i].testNumber, 
				failures[i].lineNumber, failures[i].description);
		}
	}

	return 0;
}

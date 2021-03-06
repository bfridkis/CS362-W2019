/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                         unittest3.c
 *
 *	     Test for "shuffle" function - See dominion.c line 202
 *
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c _unittest3helper.o dominion.o cardEffects.o rngs.o
 * 		gcc -o unittest3 unittest3.c -g dominion.o cardEffects.o 
 * 		_unittest3helper.o rngs.o $(CFLAGS)
 * ---------------------------------------------------------------------------
 */

#include "_unittest3helper.h"

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[MAX_FAILS];
	int failCt = 0;
	
	//Unit test 3 'stats' struct (see _unittest3helper.h)
	unittest3stats ut3s = {0};
	
	printf("\t\t\t\t\t" "Starting unittest3 - Testing 'shuffle' function\n");
	
	if(RANDOMIZE){
		printf("\n  Executing %d shuffles using random deck sizes in range 2 - %d (MAX_DECK),\n"
			   "\t each with a random set of kingdom cards...\n", NUM_SHUFFLES, MAX_DECK);
	}
	else{
		printf("\n" "  Executing %d shuffles using deck sizes starting at 5 and incrementing\n"
			   "\t"  " by multiples of 5 with each successive test number, up to %d (MAX_DECK).\n"
			   "\t"  " If the number of tests cause the deck size to equal or exceed %d when\n"
			   "\t"  " calculated in this way, the deck size will reset to 1 and begin\n"
			   "\t"  " incrementing by 1 with each additional successive test.\n"
			   "\t\t" "-e.g. deck size for test 1 = 5, test 2 = 10, test 3 = 15...\n"
			   "\t\t" "      deck size for test 100 with MAX_DECK @ 500 = 1, test 101 = 2...\n\n"
			   "  Kingdom cards are adventurer through great_hall, as enumerated in dominion.h.\n\n"
			   "\t\t"  "-Set 'NUM_SHUFFLES' in _unittest3helper.h.c\n"
			   "\t\t"  " to modify number of tests.\n\n"
			   "\t\t"  "-Random test generator can be turned on\n"
			   "\t\t"  " by setting the constant 'RANDOMIZE' to 1\n"
			   "\t\t"  " in _unittest3helper.h.\n\n", NUM_SHUFFLES, MAX_DECK, MAX_DECK);
	}

	//Use stream 2 to generate random number based on system time. (See rngs.c)
	//This random number will be used as the game's seed.
	//(Use a different stream than is used by the 'initializeGame' function.)
	SelectStream(2);
	PutSeed(-1);
		   
	int i, j, k[10], m, seed = 5000;
	for(i = 0; i < NUM_SHUFFLES; i++){
		
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
		
		//Run shuffle test on random kingdom card set.
		//(see _unittest3helper for more details)
		_unittest3helper(k, &G, failures, &failCt, &ut3s, 0, 0, i + 1);
	}
	
	printf("\n  Attempting empty deck shuffle...\n");
	
	//Attempting shuffle on empty deck
	initializeGame(2, k, Random() * INT_MAX, &G);
	if(_unittest3helper(k, &G, failures, &failCt, &ut3s, 1, 
			0, NUM_SHUFFLES + 1) != 0){
		printf("\n  Empty deck test failed\n");
	}
	
	printf("\n  Attempting max deck size shuffle...\n");
	
	//Attempting shuffle on max deck
	initializeGame(2, k, Random() * INT_MAX, &G);
	if(_unittest3helper(k, &G, failures, &failCt, &ut3s, 0,
			1, NUM_SHUFFLES + 2) != 0){
		printf("\nMax deck size test failed\n");
		
		printf("\n\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ RESULTS SUMMARY ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	}
	
	if(!failCt){
		printf("\n\n\t\t\t\t\t\t*****************************\n"
				   "\t\t\t\t\t\t***** ALL TESTS PASSED! *****\n"
				   "\t\t\t\t\t\t*****************************\n\n");
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
		printf("  (Note: See _unittest3helper.c when referencing line #)\n\n");
		int i;
		for(i = 0; i < failCt && i < MAX_FAILS; i++){
			printf("%d - TEST #%d @ LINE %d: %s\n\n", 
				i + 1, failures[i].testNumber, 
				failures[i].lineNumber, failures[i].description);
		}
	}
	
	printf("\n  ***************************************************************************\n");
	printf(  "  ** %% Variations (Relative to Deck Size) After Shuffles with no Failures **\n");
	printf(  "  ***************************************************************************\n\n");
	
	printf("\t\t\t\t\t     0       : %d\n"
		   "\t\t\t\t\t> 0  - <11%%  : %d\n"
		   "\t\t\t\t\t 11%% - <21%%  : %d\n"
		   "\t\t\t\t\t 21%% - <31%%  : %d\n"
		   "\t\t\t\t\t 31%% - <41%%  : %d\n"
		   "\t\t\t\t\t 41%% - <51%%  : %d\n"
		   "\t\t\t\t\t   >51%%      : %d\n\n",
			ut3s._noDev, ut3s._gt0to10PercentDev,
			ut3s._11to20PercentDev, ut3s._21to30PercentDev,
			ut3s._31to40PercentDev, ut3s._41to50PercentDev,
			ut3s._51PlusPercentDev);
			
	printf("\tThe above values represent the number of tests for which the MINIMUM\n" 
			"\tnumber of observed changes divided by the deck size used produced the\n"
			"\tgiven percentage. For example, if a deck size of 20 was compared before\n"
			"\tand after shuffle, and 10 deck indexes contained the same card type both\n"
			"\tbefore and after shuffle, while the other 10 deck indexes contained different\n"
			"\tcard types (when comparing the before and after decks), the test generated a\n"
			"\tvariance value of 50%%. The above values are the incidence of tests in each\n"
			"\tpercentage range.\n\n"
			"\tNote that a few tests in the lower ranges may not indicate a faulty shuffle\n"
			"\tfunction, as these may be the result of very small deck sizes. However,\n"
			"\ta large incidence in the lower ranges may indicate that shuffle is not\n"
			"\tadequately redistributing the cards within the deck, while a large incidence\n"
			"\tat the '0' range specifically may indicate that shuffle is not redistributing\n"
			"\tthe cards at all.\n\n"
			"\tFinally, it is important to note that this variance test DOES NOT ACCOUNT FOR\n"
			"\tCARD EXCHANGES OF THE SAME CARD TYPE. In other words, more shuffling may be\n"
			"\t(and likely is) occurring than is accounted for by these numbers (and hence\n"
			"\tthe emphasis on 'minimum' above). This is an important consideration to take\n"
			"\tinto account when evaluating these statistics, especially when the low variance\n"
			"\tranges have low incidence. (In other words, a relatively low incidence for the\n"
			"\tlow variance ranges may not necessarily indicate an issue with shuffle.)\n\n"
			"\t(Max number of tests in this breakdown is NUM_SHUFFLES + 1, as the MAX_DECK\n"
			"\t deck size shuffle test is included but the empty deck shuffle test is not.)\n\n");

	return 0;
}

/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *               _unittest2helper.c
 *
 *		   Helper function for unittest2.c
 *
 *			(Tests "gainCard" function)
 *
 * Include the following lines in your makefile:
 *
 * _unittest2helper.o: _unittest2helper.c _unittest2helper.h dominion.o
 *		gcc -c _unittest2helper.c -g  $(CFLAGS)
 * ---------------------------------------------------------------------------
 */
 
#include "_unittest2helper.h"
 
#define GAINCARDCALL1
#define GAINCARDCALL2

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in unittest1.c also!
#define MAX_FAILS 400
 
int _unittest2helper(int testCase, int k[], struct gameState* G,
	failedTest failures[]){
	 
	//Test value variables	   
	int i, j, returnValue, failCt = 0;
	
	//Ensure player 0's deck, discard, and hand are cleared
	memset(G->deck[0], -1, sizeof(int) * MAX_DECK);
	memset(G->discard[0], -1, sizeof(int) * MAX_DECK);
	memset(G->hand[0], -1, sizeof(int) * MAX_HAND);
	G->deckCount[0] = 0;
	G->discardCount[0] = 0;
	G->handCount[0] = 0;
	
	//Declare testing struct
	unittest2helper2struct ut2h2s;
	ut2h2s.expectedRV = 0;
	
	//Test gain each supply card (curse, estate, duchy, province, copper
	//silver, gold, and all kingdom cards) to each pile (i == 0 for discard,
	//i == 1 for deck, and i == 2 for hand).
	for(i = 0; i < 3; i++){
		for(j = 0; j < 17; j++){
			ut2h2s.expectedPile = i;
			ut2h2s.expectedIdx = j;
			ut2h2s.rv = gainCard(j, G, 0, 0);
			_unittest2helper2(ut2h2s, failures, &failCt, G, k);
		}
	}
	
	//Check supplyPos values, each of which should now be 3 less
	//(as the previous for loop attempted to gain 1 of each to each of
	//the 3 piles [discard, deck, hand] respectively) than their 
	//respective starting values for all cards in play, and their original 
	//(starting) values for those cards not in play.
	for(i = 0; i < 27; i++){
		if((i == treasure_map || cutpurse || adventurer || 
				smithy || baron || outpost || embargo || remodel) && 
			G->supplyCount[i] != 7){
			if(++(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Supply pile count not as expected.\n"
				"Expected: 7 for card %d ; Observed %d", i, G->supplyCount[i]);
			}
		}
		else if((i == gardens || great_hall)) && G->supplyCount[i] != 5){
			if(++(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Supply pile count not as expected.\n"
				"Expected: 5 for card %d ; Observed %d", i, G->supplyCount[i]);
			}
		}
		else if(G->supplyCount[i] != 10){
			if(++(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Supply pile count not as expected.\n"
				"Expected: 10 for card %d ; Observed %d", i, G->supplyCount[i]);
			}
		}
	}
	
	//Try to gain all cards which are
	//not in the game (return value should == -1)
	for(i = 0; i < 27; i++){
		int rv = gainCard(i, G, 0, 0)
		if(i != k[i] && rv != -1){
			if(++(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Return value not as expected after attempting "
				"to gain card not in game.\n"
				"Expected: -1 ; Observed %d when gaining card %d", rv, i);
			}
		}
	}
	
	//Empty all supply piles and try to gain each card
	//Try to gain all cards which are
	//not in the game (return value should == -1)
	memset(G->supplyCount, 0, sizeof(int) * treasure_map+1);
	for(i = 0; i < 27; i++){
		int rv = gainCard(i, G, 0, 0)
		if(rv != -1){
			if(++(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Return value not as expected after emptying supply piles.\n"
				"Expected: -1 ; Observed %d when gaining card %d", rv, i);
			}
		}
	}
}
	
	
void _unittest2helper2(unittest2helper2struct ut2h2s, failedTest failures,
	int* failCt, struct gameState* G, int k[]){

	//Check to make sure return value is as expected
	if(ut2h2s.rv != ut2h2s.expectedRV){
		if(++(*failCt) <= MAX_FAILS){
		  failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
		  sprintf(failures[(*failCt)-1].description,
			"Return value not as expected.\n"
			"Expected: %d ; Observed %d when gaining kingdom card at idx: %d",
			ut2h2s.rv, ut2h2s.expectedRV, ut2h2s.expectedIdx);
		}
	}
	
	//Check to make sure card was gained to expected pile
	if(ut2h2s.expectedPile == 0 && 
		G->discard[0][ut2h2s.expectedIdx] != k[ut2h2s.expectedIdx]){
		if(++(*failCt) <= MAX_FAILS){
		  failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
		  sprintf(failures[(*failCt)-1].description,
			"Card not gained to proper location.\n"
			"Expected: %d at idx %d of discard pile ; Observed %d",
			k[ut2h2s.expectedIdx], ut2h2s.expectedIdx, 
			G->discard[0][ut2h2s.expectedIdx]);
		}
	}
	else if(ut2h2s.expectedPile == 1 && 
		G->deck[0][ut2h2s.expectedIdx] != k[ut2h2s.expectedIdx]){
		if(++(*failCt) <= MAX_FAILS){
		  failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
		  sprintf(failures[(*failCt)-1].description,
			"Card not gained to proper location.\n"
			"Expected: %d at idx %d of deck ; Observed %d",
			k[ut2h2s.expectedIdx], ut2h2s.expectedIdx, 
			G->discard[0][ut2h2s.expectedIdx]);
		}
	}
	else if(ut2h2s.expectedPile == 2 && 
		G->hand[0][ut2h2s.expectedIdx] != k[ut2h2s.expectedIdx]){
		if(++(*failCt) <= MAX_FAILS){
		  failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
		  sprintf(failures[(*failCt)-1].description,
			"Card not gained to proper location.\n"
			"Expected: %d at idx %d of hand ; Observed %d",
			k[ut2h2s.expectedIdx], ut2h2s.expectedIdx, 
			G->discard[0][ut2h2s.expectedIdx]);
		}
	}
	
	//Check to make sure no cards are in unexpected locations
	for(j = 0; (ut2h2s.expectedPile == 0 || ut2h2s.expectedPile == 1) && 
			j < MAX_DECK; j++){
		//Indexes in each pile (discard, deck, and hand) should have
		//cards corresponding to the corresponding index values of k[]
		//(e.g. if k[0] == adventurer, then discard[0] should == adventurer)
		//up to the index "expectedIdx". Otherwise, all remaining indexes
		//beyond this value should be -1.
		
		//For discard pile...
		if(ut2h2s.expectedPile = 0 && 
		   ((j <= ut2h2s.expectedIdx && G->discard[0][j] != k[j]) ||
		    (j > ut2h2s.expectedIdx && G->discard[0][j] != -1))){

			//If issue is with index at or before expectedIdx...
			if(++(*failCt) <= MAX_FAILS && j <= ut2h2s.expectedIdx){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected card %d at idx %d of discard pile ; Observed %d",
				k[j], ut2h2s.expectedIdx, G->discard[0][j]);
				break;
			}
			//If issue is at index beyond expectedIdx...
			else if(++(*failCt) <= MAX_FAILS && j > ut2h2s.expectedIdx){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected -1 at idx %d of discard pile ; Observed %d",
				ut2h2s.expectedIdx, G->discard[0][j]);
				break;
			}
		}
		
		//For deck...
		else if(ut2h2s.expectedPile = 1 && 
		   ((j <= ut2h2s.expectedIdx && G->deck[0][j] != k[j]) ||
		    (j > ut2h2s.expectedIdx && G->deck[0][j] != -1))){

			if(++(*failCt) <= MAX_FAILS && j <= ut2h2s.expectedIdx){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected card %d at idx %d of deck ; Observed %d",
				k[j], ut2h2s.expectedIdx, G->deck[0][j]);
				break;
			}
			else if(++(*failCt) <= MAX_FAILS && j > ut2h2s.expectedIdx){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected -1 at idx %d of deck pile ; Observed %d",
				ut2h2s.expectedIdx, G->deck[0][j]);
				break;
			}
		}
	}
	
	//For hand... (separate for loop because constant MAX_HAND could be
	//different than constant MAX_DECK, the latter of which is used for both
	//discard and deck, which are covered in the previous for loop accordingly.
	for(j = 0; ut2h2s.expectedPile == 2 && j < MAX_HAND; j++){	
		if((j <= ut2h2s.expectedIdx && G->hand[0][j] != k[j]) ||
		    (j > ut2h2s.expectedIdx && G->hand[0][j] != -1)){

			if(++(*failCt) <= MAX_FAILS && j <= ut2h2s.expectedIdx){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected card %d at idx %d of hand ; Observed %d",
				k[j], ut2h2s.expectedIdx, G->hand[0][j]);
				break;
			}
			else if(++(*failCt) <= MAX_FAILS && j > ut2h2s.expectedIdx){
				failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected -1 at idx %d of hand ; Observed %d",
				ut2h2s.expectedIdx, G->hand[0][j]);
				break;
			}
		}
	}
	
	//Check to make sure all pile counts are as expected
	if(G->discardCount[0] != ut2h2s.expectedIdx + 1 && 
		++(*failCt) <= MAX_FAILS){
		failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
		sprintf(failures[(*failCt)-1].description,
		"Discard pile count updated incorrectly.\n"
		"Expected %d ; Observed %d", ut2h2s.expectedIdx + 1, G->discountCount[0]);
	}
	
	if(G->deckCount[0] != ut2h2s.expectedIdx + 1 && 
		++(*failCt) <= MAX_FAILS){
		failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
		sprintf(failures[(*failCt)-1].description,
		"Deck count updated incorrectly.\n"
		"Expected %d ; Observed %d", ut2h2s.expectedIdx + 1, G->deckCount[0]);
	}
	
	if(G->handCount[0] != ut2h2s.expectedIdx + 1 && 
		++(*failCt) <= MAX_FAILS){
		failures[(*failCt)-1].lineNumber = BUYCARDCALL1;
		sprintf(failures[(*failCt)-1].description,
		"Hand pile count updated incorrectly.\n"
		"Expected %d ; Observed %d", ut2h2s.expectedIdx + 1, G->handCount[0]);
	}
}
			
				
	
	
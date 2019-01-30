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
 *
 * ---------------------------------------------------------------------------
 */
 
#include "_unittest2helper.h"
 
#define NUM_PLAYERS 2

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in unittest1.c also!
#define MAX_FAILS 500
 
int _unittest2helper(int k[], struct gameState* G, failedTest failures[]){
	 
	//Test value variables	   
	int i, j, failCt = 0, rv;
	
	//Ensure both players' decks, discards, and hands are cleared
	memset(G->discard[0], -1, sizeof(int) * MAX_DECK);
	memset(G->discard[1], -1, sizeof(int) * MAX_DECK);
	memset(G->deck[0], -1, sizeof(int) * MAX_DECK);
	memset(G->deck[1], -1, sizeof(int) * MAX_DECK);
	memset(G->hand[0], -1, sizeof(int) * MAX_HAND);
	memset(G->hand[1], -1, sizeof(int) * MAX_HAND);
	G->discardCount[0] = 0;
	G->discardCount[1] = 0;
	G->deckCount[0] = 0;
	G->deckCount[1] = 0;
	G->handCount[0] = 0;
	G->handCount[1] = 0;
	
	//Declare testing struct
	unittest2helper2struct ut2h2s;
	ut2h2s.expectedRV = 0;
	
	//Test gain each supply card (curse, estate, duchy, province, copper
	//silver, gold, and all kingdom cards) to each pile (i == 0 for discard,
	//i == 1 for deck, and i == 2 for hand).
	printf("\n\nGaining each supply card to each pile for player 0...\n");
	for(i = 0; i < 3; i++){
		for(j = 0; j < 17; j++){
			ut2h2s.expectedPile = i;
			ut2h2s.expectedIdx = j;
			//Gain curse, estate, duchy, province, copper, silver, gold
			if(j < 7){
				ut2h2s.rv = gainCard(j, G, i, 0);
			}
			//Gain kingdom cards in play
			else{
				ut2h2s.rv = gainCard(k[j-7], G, i, 0);
			}
			_unittest2helper2(ut2h2s, failures, &failCt, G, k);
		}
	}
	
	//Check supplyPos values, each of which should now be 3 less
	//(since the previous 'for' loop attempted to gain 1 of each to each of
	//the 3 piles [discard, deck, hand] respectively) than their 
	//respective starting values for all cards in play, and their original 
	//(starting) values for those cards not in play.
	for(i = 0; i < 27; i++){
		if((i == treasure_map || i == cutpurse || i == adventurer || 
				i == smithy || i == baron || i == outpost || 
				i == embargo || i == remodel || i == curse) && 
			G->supplyCount[i] != 7){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Supply pile count not as expected.\n"
				"Expected: 7 for card %d ; Observed %d", i, G->supplyCount[i]);
			}
		}
		else if((i == gardens || i == great_hall) && G->supplyCount[i] != 5){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Supply pile count not as expected.\n"
				"Expected: 5 for card %d ; Observed %d", i, G->supplyCount[i]);
			}
		}
		else if(i == copper && G->supplyCount[i] != 60 - (7 * NUM_PLAYERS) - 3){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Supply pile count not as expected.\n"
				"Expected: %d for card %d ; Observed %d",
				60 - (7 * NUM_PLAYERS) - 3, i, G->supplyCount[i]);
			}
		}
		else if(i == silver && G->supplyCount[i] != 37){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Supply pile count not as expected.\n"
				"Expected: 37 for card %d ; Observed %d",
				i, G->supplyCount[i]);
			}
		}
		else if(i == gold && G->supplyCount[i] != 27){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Supply pile count not as expected.\n"
				"Expected: 27 for card %d ; Observed %d",
				i, G->supplyCount[i]);
			}
		}
		else if((i == estate || i == duchy || i == province) && 
					G->supplyCount[i] != 5){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Supply pile count not as expected.\n"
				"Expected: 5 for card %d ; Observed %d",
				i, G->supplyCount[i]);
			}
		}
		//Unused cards...
		else if((i == council_room || i == feast || i == mine ||
				 i == village || i == minion || i == steward ||
				 i == tribute || i == ambassador || i == salvager ||
				 i == sea_hag) && G->supplyCount[i] != -1){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Supply pile count not as expected.\n"
				"Expected: -1 for card %d ; Observed %d", i, G->supplyCount[i]);
			}
		}
	}
	
	//Check to make sure all final pile counts are as expected after gaining
	//each supply card to each pile
	if(G->discardCount[0] != 17 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Discard pile count updated incorrectly.\n"
		"Expected %d ; Observed %d", ut2h2s.expectedIdx + 1, G->discardCount[0]);
	}
	
	if(G->deckCount[0] != 17 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Deck count updated incorrectly.\n"
		"Expected %d ; Observed %d", ut2h2s.expectedIdx + 1, G->deckCount[0]);
	}
	
	if(G->handCount[0] != 17 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Hand pile count updated incorrectly.\n"
		"Expected %d ; Observed %d", ut2h2s.expectedIdx + 1, G->handCount[0]);
	}
	
	printf("\n*Running Fail Count: %d...\n", failCt);
	
	//Try to gain all cards which are
	//not in the game (return value should == -1)
	printf("\n\nAttempting to gain each supply card not in play (player 0)...\n");
	for(i = 7; i < 27; i++){
		
		//To discard...
		if(i == council_room || i == feast || i == mine ||
				 i == village || i == minion || i == steward ||
				 i == tribute || i == ambassador || i == salvager ||
				 i == sea_hag){
			rv = gainCard(i, G, 0, 0);
			if(rv != -1 && ++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Return value not as expected after attempting\n"
				" \t\t\t  to gain card not in game to discard pile.\n"
				"  Expected: -1 ; Observed %d when gaining card %d", rv, i);
			}
		}
		
		//To deck...
		if(i == council_room || i == feast || i == mine ||
				 i == village || i == minion || i == steward ||
				 i == tribute || i == ambassador || i == salvager ||
				 i == sea_hag){
			rv = gainCard(i, G, 1, 0);		 
			if(rv != -1 && ++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Return value not as expected after attempting\n"
				"\t\t\t  to gain card not in game to deck.\n"
				"  Expected: -1 ; Observed %d when gaining card %d", rv, i);
			}
		}
		
		//To hand...
		if(i == council_room || i == feast || i == mine ||
				 i == village || i == minion || i == steward ||
				 i == tribute || i == ambassador || i == salvager ||
				 i == sea_hag){
			rv = gainCard(i, G, 2, 0);
			if(rv != -1 && ++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Return value not as expected after attempting\n"
				" \t\t\t  to gain card not in game to hand.\n"
				"  Expected: -1 ; Observed %d when gaining card %d", rv, i);
			}
		}
	}
	
	//Check to make sure all final pile counts are as expected after attempting
	//to gain cards not in game
	if(G->discardCount[0] != 17 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Discard pile count updated incorrectly.\n"
		"Expected 17 ; Observed %d", G->discardCount[0]);
	}
	
	if(G->deckCount[0] != 17 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Deck count updated incorrectly.\n"
		"Expected 17 ; Observed %d", G->deckCount[0]);
	}
	
	if(G->handCount[0] != 17 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Hand pile count updated incorrectly.\n"
		"Expected 17 ; Observed %d", G->handCount[0]);
	}
	
	printf("\n*Running Fail Count: %d...\n", failCt);
	
	//Empty all supply piles and try to gain each card
	printf("\n\nSetting each supply card pile count to 0 and\n"
		   "  attempting to gain each supply pile card (player 0)...\n");
	G->discardCount[0] = 0;
	G->deckCount[0] = 0;
	G->handCount[0] = 0;	   
	memset(G->supplyCount, 0, sizeof(int) * treasure_map+1);
	for(i = 0; i < 27; i++){
		//To discard...
		gainCard(i, G, 0, 0);
		if(rv != -1){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Return value not as expected after emptying supply piles.\n"
				"Expected: -1 ; Observed %d when gaining card %d", rv, i);
			}
		}
		//To deck...
		gainCard(i, G, 1, 0);
		if(rv != -1){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Return value not as expected after emptying supply piles.\n"
				"Expected: -1 ; Observed %d when gaining card %d", rv, i);
			}
		}
		//To hand...
		gainCard(i, G, 2, 0);
		if(rv != -1){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Return value not as expected after emptying supply piles.\n"
				"Expected: -1 ; Observed %d when gaining card %d", rv, i);
			}
		}
	}
	
	//Check to make sure all final pile counts are as expected after emptying all
	//piles and attempting to gain each card
	if(G->discardCount[0] != 0 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Discard pile count updated incorrectly.\n"
		"Expected 0 ; Observed %d", G->discardCount[0]);
	}
	
	if(G->deckCount[0] != 0 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Deck count updated incorrectly.\n"
		"Expected 0 ; Observed %d", G->deckCount[0]);
	}
	
	if(G->handCount[0] != 0 && 
		++failCt <= MAX_FAILS){
		failures[failCt-1].lineNumber = __LINE__;
		sprintf(failures[failCt-1].description,
		"Hand pile count updated incorrectly.\n"
		"Expected 0 ; Observed %d", G->handCount[0]);
	}
	
	printf("\n*Running Fail Count: %d...\n", failCt);
	
	//Check player 1's pile states (nothing should have changed!)
	printf("\n\nChecking player 1's game state (should be unchanged)...\n\n");
	//Check player 1's discard...
	for(i = 0; i < MAX_DECK; i++){
		if(G->discard[1][i] != -1){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Discard pile state for player 1 incorrect.\n"
				"Check discard pile idx %d", i);
			}
			break;
		}
	}
	
	//Check player 1's deck...
	for(i = 0; i < MAX_DECK; i++){
		if(G->deck[1][i] != -1){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Deck state for player 1 incorrect.\n"
				"Check deck idx %d", i);
			}
			break;
		}
	}
	
	//Check player 1's hand...
	for(i = 0; i < MAX_HAND; i++){
		if(G->hand[1][i] != -1){
			if(++failCt <= MAX_FAILS){
				failures[failCt-1].lineNumber = __LINE__;
				sprintf(failures[failCt-1].description,
				"Hand state for player 1 incorrect.\n"
				"Check hand idx %d", i);
			}
			break;
		}
	}
	
	printf("*Running Fail Count: %d...\n", failCt);
	
	return failCt;
}
	
	
void _unittest2helper2(unittest2helper2struct ut2h2s, failedTest failures[],
	int* failCt, struct gameState* G, int k[]){
		
	//Test variable declaration
	int j;

	//Check to make sure return value is as expected
	if(ut2h2s.rv != ut2h2s.expectedRV){
		if(++(*failCt) <= MAX_FAILS){
		  failures[(*failCt)-1].lineNumber = __LINE__;
		  sprintf(failures[(*failCt)-1].description,
			"Return value not as expected.\n"
			"Expected: %d ; Observed %d when gaining kingdom card at idx: %d",
			ut2h2s.rv, ut2h2s.expectedRV, ut2h2s.expectedIdx);
		}
	}
	
	//Check to make sure card was gained to expected pile at expected location
	
	//Check discard...
	if(ut2h2s.expectedPile == 0){
		if(ut2h2s.expectedIdx < 7 && 
			G->discard[0][ut2h2s.expectedIdx] != ut2h2s.expectedIdx){
			if(++(*failCt) <= MAX_FAILS){
			  failures[(*failCt)-1].lineNumber = __LINE__;
			  sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location.\n"
				"Expected: %d at idx %d of discard pile ; Observed %d",
				ut2h2s.expectedIdx, ut2h2s.expectedIdx, 
				G->discard[0][ut2h2s.expectedIdx]);
			}
		}
		else if(ut2h2s.expectedIdx >= 7 &&
				G->discard[0][ut2h2s.expectedIdx] != k[ut2h2s.expectedIdx - 7]){
			if(++(*failCt) <= MAX_FAILS){
			  failures[(*failCt)-1].lineNumber = __LINE__;
			  sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location.\n"
				"Expected: %d at idx %d of discard pile ; Observed %d",
				k[ut2h2s.expectedIdx], ut2h2s.expectedIdx, 
				G->discard[0][ut2h2s.expectedIdx]);
			}
		}
	}
	//Check deck...
	if(ut2h2s.expectedPile == 1){
		if(ut2h2s.expectedIdx < 7 && 
			G->deck[0][ut2h2s.expectedIdx] != ut2h2s.expectedIdx){
			if(++(*failCt) <= MAX_FAILS){
			  failures[(*failCt)-1].lineNumber = __LINE__;
			  sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location.\n"
				"Expected: %d at idx %d of deck ; Observed %d",
				k[ut2h2s.expectedIdx], ut2h2s.expectedIdx, 
				G->deck[0][ut2h2s.expectedIdx]);
			}
		}
		else if(ut2h2s.expectedIdx >= 7 &&
					G->deck[0][ut2h2s.expectedIdx] != k[ut2h2s.expectedIdx - 7]){
			if(++(*failCt) <= MAX_FAILS){
			  failures[(*failCt)-1].lineNumber = __LINE__;
			  sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location.\n"
				"Expected: %d at idx %d of deck ; Observed %d",
				k[ut2h2s.expectedIdx], ut2h2s.expectedIdx, 
				G->deck[0][ut2h2s.expectedIdx]);
			}
		}
	}
	//Check hand...
	if(ut2h2s.expectedPile == 2){
		if(ut2h2s.expectedIdx < 7 && 
			G->deck[0][ut2h2s.expectedIdx] != ut2h2s.expectedIdx){
			if(++(*failCt) <= MAX_FAILS){
			  failures[(*failCt)-1].lineNumber = __LINE__;
			  sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location.\n"
				"Expected: %d at idx %d of hand ; Observed %d",
				k[ut2h2s.expectedIdx], ut2h2s.expectedIdx, 
				G->hand[0][ut2h2s.expectedIdx]);
			}
		}
		else if(ut2h2s.expectedIdx >= 7 &&
					G->hand[0][ut2h2s.expectedIdx] != k[ut2h2s.expectedIdx - 7]){
			if(++(*failCt) <= MAX_FAILS){
			  failures[(*failCt)-1].lineNumber = __LINE__;
			  sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location.\n"
				"Expected: %d at idx %d of hand ; Observed %d",
				k[ut2h2s.expectedIdx], ut2h2s.expectedIdx, 
				G->hand[0][ut2h2s.expectedIdx]);
			}
		}
	}
	
	//Check to make sure no cards are in unexpected locations
	for(j = 0; (ut2h2s.expectedPile == 0 || ut2h2s.expectedPile == 1) && 
			j < MAX_DECK; j++){
		//Indexes in each pile (discard, deck, and hand) should have
		//cards corresponding to the matching index values of k[]
		//(e.g. if k[0] == adventurer, then discard[0] should == adventurer)
		//up to the index "expectedIdx". Otherwise, all remaining indexes
		//beyond this value should be -1.
		
		//For discard pile...
		if(ut2h2s.expectedPile == 0){
			//If issue is with index at or before 6 (corresponding to curse - gold)...
			if(j < 7 && j <= ut2h2s.expectedIdx && G->discard[0][j] != j &&
					++(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = __LINE__;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected card %d at idx %d of discard pile ; Observed %d",
				j, j, G->discard[0][j]);
				break;
			}
			
			//If issue is with index between 7 and expectedIdx 
			//(corresponding to kingdom cards added so far)...
			else if(j >= 7 && j <= ut2h2s.expectedIdx && 
						G->discard[0][j] != k[j-7] && (*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = __LINE__;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected card %d at idx %d of discard pile ; Observed %d",
				k[j-7], j, G->discard[0][j]);
				break;
			}
			
			//If issue is with index where no card should be...
			else if(j > ut2h2s.expectedIdx && G->discard[0][j] != -1 &&
						(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = __LINE__;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected -1 at idx %d of discard pile ; Observed %d",
				j, G->discard[0][j]);
				break;
			}
		}
		
		//For deck...
		else if(ut2h2s.expectedPile == 1){
			//If issue is with index at or before 6 (corresponding to curse - gold)...
			if(j < 7 && j <= ut2h2s.expectedIdx && G->deck[0][j] != j &&
					++(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = __LINE__;
				sprintf(failures[(*failCt)-1].description,
				"Caabrd not gained to proper location or additional card gained.\n"
				"Expected card %d at idx %d of deck ; Observed %d",
				j, j, G->deck[0][j]);
				break;
			}
			
			//If issue is with index between 7 and expectedIdx 
			//(corresponding to kingdom cards added so far)...
			else if(j >= 7 && j <= ut2h2s.expectedIdx && 
						G->deck[0][j] != k[j-7] && (*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = __LINE__;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected card %d at idx %d of deck ; Observed %d",
				k[j-7], j, G->deck[0][j]);
				break;
			}
			
			//If issue is with index where no card should be...
			else if(j > ut2h2s.expectedIdx && G->deck[0][j] != -1 &&
						(*failCt) <= MAX_FAILS){
				failures[(*failCt)-1].lineNumber = __LINE__;
				sprintf(failures[(*failCt)-1].description,
				"Card not gained to proper location or additional card gained.\n"
				"Expected -1 at idx %d of deck ; Observed %d",
				j, G->deck[0][j]);
				break;
			}
		}
	}
	
	//For hand... (separate 'for' loop here because constant MAX_HAND could be
	//different than constant MAX_DECK, the latter of which is used for both
	//discard and deck, which are covered in the previous 'for' loop accordingly.
	for(j = 0; ut2h2s.expectedPile == 2 && j < MAX_HAND; j++){	
		
		//For hand...
		
		//If issue is with index at or before 6 (corresponding to curse - gold)...
		if(j < 7 && j <= ut2h2s.expectedIdx && G->hand[0][j] != j &&
				++(*failCt) <= MAX_FAILS){
			failures[(*failCt)-1].lineNumber = __LINE__;
			sprintf(failures[(*failCt)-1].description,
			"Card not gained to proper location or additional card gained.\n"
			"Expected card %d at idx %d of hand ; Observed %d",
			j, j, G->hand[0][j]);
			break;
		}
		
		//If issue is with index between 7 and expectedIdx 
		//(corresponding to kingdom cards added so far)...
		else if(j >= 7 && j <= ut2h2s.expectedIdx && 
					G->deck[0][j] != k[j-7] && (*failCt) <= MAX_FAILS){
			failures[(*failCt)-1].lineNumber = __LINE__;
			sprintf(failures[(*failCt)-1].description,
			"Card not gained to proper location or additional card gained.\n"
			"Expected card %d at idx %d of hand ; Observed %d",
			k[j-7], j, G->hand[0][j]);
			break;
		}
		
		//If issue is with index where no card should be...
		else if(j > ut2h2s.expectedIdx && G->hand[0][j] != -1 &&
					(*failCt) <= MAX_FAILS){
			failures[(*failCt)-1].lineNumber = __LINE__;
			sprintf(failures[(*failCt)-1].description,
			"Card not gained to proper location or additional card gained.\n"
			"Expected -1 at idx %d of hand ; Observed %d",
			j, G->hand[0][j]);
			break;
		}
	}
}
			
				
	
	
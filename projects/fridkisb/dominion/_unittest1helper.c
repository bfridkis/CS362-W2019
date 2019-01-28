/* --------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *       _unittest1helper.c
 *
 *     Helper function for unittest1.c
 *
 *		(Tests "buyCard" function)
 *
 *	Include the following lines in your make file:
 * 
 * _unittest1helper.o: _unittest1helper.c _unittest1helper.h dominion.o
 *	gcc -c _unittest1helper.c -g  $(CFLAGS)
 * ---------------------------------------------------------------------
 */
 
 #include "_unittest1helper.h"
 
#define NUM_PLAYERS 2

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in unittest1.c also!
#define MAX_FAILS 500
 
int _unittest1helper(int testCase, int k[], struct gameState* G,
	failedTest failures[], int* failedTestCount){
	 
	//Test value variables	   
	int i, j, m, n, returnValue, failCtT1 = 0, failCtT2 = 0, failCtT3 = 0;
	
	//These next two lines are just in case initializeGame 
	//(in unittest1.c) is not functioning properly
	G->whoseTurn = 0; 
	G->numBuys = 1;
	
	//Set entire discard, deck, and hand piles for player 0 and 1 
	//to -1, so it can be checked for erroneous changes
	memset(G->discard[0], -1, sizeof(int) * MAX_DECK);
	memset(G->discard[1], -1, sizeof(int) * MAX_DECK);
	memset(G->deck[0], -1, sizeof(int) * MAX_DECK);
	memset(G->deck[1], -1, sizeof(int) * MAX_DECK);
	memset(G->hand[0], -1, sizeof(int) * MAX_HAND);
	memset(G->hand[1], -1, sizeof(int) * MAX_HAND);
	
	for(i = 0; i < 10; i++){
		
		if(testCase == 1){
			//Test case 1 tests all buyCard functionality under normal 
			//circumstances. For each of the 10 kingdom cards listed in 
			//k[], exhaust the supply pile.
			for(j = 1; j <= 10; j++){
				G->numBuys = 1;
				G->coins = 6;
				returnValue = buyCard(k[i], G);
				
				//Check to make sure G->whoseTurn has not been changed
				if(G->whoseTurn != 0){
				  if(++(*failedTestCount) <= MAX_FAILS){
					  failures[(*failedTestCount)-1].lineNumber = __LINE__;
					  sprintf(failures[(*failedTestCount)-1].description,
						"whoseTurn unexpectedly changed.\n"
						"Expected: 0 ; Observed %d when buying kingdom card at idx: %d", 
						G->whoseTurn, i);
				  }
					failCtT1++;
				}
				
				//Check to make sure return value is as expected (0)
				if(returnValue != 0){
					if(++(*failedTestCount) <= MAX_FAILS){
					  failures[(*failedTestCount)-1].lineNumber = __LINE__;
					  sprintf(failures[(*failedTestCount)-1].description,
						"Return value not as expected.\n"
						"Expected: 0 ; Observed %d when buying kingdom card at idx: %d",
						returnValue, i);
					}
				  failCtT1++;	
				}
				
				//Check to make sure purchased card is placed onto discard pile
				if(G->discard[0][j - 1 + (i * 10)] != k[i]){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Purchased card not in expected discard pile location,\n"
						"when buying kingdom card at idx: %d", i);
					}
					failCtT1++;
				}
				
				//Check to make sure discardCount is updated as expected
				if(G->discardCount[0] != j + (i * 10)){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Discard (card) count not updated as expected.\n"
						"Expected: %d ; Observed %d when buying kingdom card at idx: %d", 
						j + (i * 10), G->discardCount[0], i);
					}
					failCtT1++;
				}
				
				//Check to make sure coin count is updated as expected,
				//depending on card type.
				
				//For cards costing 6 coins (adventurer)...
				if(i == 0 && G->coins != 0){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Coin value not updated as expected.\n"
						"Expected: 0; Observed %d when buying kingdom card at idx: %d", 
						G->coins, i);
					}
					failCtT1++;
				}
				
				//For cards costing 5 coins (minion, mine, & tribute)...
				else if((i == 4 || i == 5 || i == 8) && G->coins != 1){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Coin value not updated as expected.\n"
						"Expected: 1; Observed %d when buying kingdom card at idx: %d", 
						G->coins, i);
					}
					failCtT1++;
				}
				
				//For cards costing 4 coins (cutpurse, sea hag, & smithy)...
				else if((i == 6 || i == 7 || i == 9) && G->coins != 2){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Coin value not updated as expected.\n"
						"Expected: 2; Observed %d when buying kingdom card at idx: %d", 
						G->coins, i);
					}
					failCtT1++;
				}
				
				//For cards costing 3 coins (ambassador & village)...
				else if((i == 1 || i == 3) && G->coins != 3){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Coin value not updated as expected.\n"
						"Expected: 3; Observed %d when buying kingdom card at idx: %d",
						G->coins, i);
					}
					failCtT1++;
				}
				
				//For cards costing 2 coins (embargo)...
				else if(i == 2 && G->coins != 4){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Coin value not updated as expected.\n"
						"Expected: 4; Observed %d when buying kingdom card at idx: %d", 
						G->coins, i);
					}
					failCtT1++;
				}
				
				//Check to make sure numBuys is updated correctly
				if(G->numBuys != 0){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"numBuys not updated as expected.\n"
						"Expected: 0; Observed %d when buying kingdom card at idx: %d", 
						G->numBuys, i);
					}
					failCtT1++;
				}
				
				//Check to make sure the supplyCount is updated correctly
				if(G->supplyCount[k[i]] != 10 - j){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Supply pile count not updated as expected.\n"
						"Expected: %d; Observed %d when buying kingdom card at idx: %d",
						10 - j, G->supplyCount[k[i]], i);
					}
					failCtT1++;
				}
			}
		}
		
		else{
			
			//Test case 2 tests buy attempts with insufficient coins
			//(for each kingdom card).
			if(testCase == 2){
				G->coins = 0;
			}
			
			//Test case 3 tests buy attempts with insufficient buys
			//(for each kingdom card).
			else{
				G->coins = 6;
				G->numBuys = 0;
			}
			
			returnValue = buyCard(k[i], G);
			
			//Check to make sure G->whoseTurn has not been changed
			if(G->whoseTurn != 0){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"whoseTurn unexpectedly changed.\n"
					"Expected: 0 ; Observed %d when buying kingdom card at idx: %d", 
					G->whoseTurn, i);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
			}
			
			//Check to make sure return value is as expected (-1)
			if(returnValue != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Return value not as expected.\n"
					"Expected: 0 ; Observed %d when buying kingdom card at idx: %d", 
					returnValue, i);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
			}
			
			//Check to make sure no card is added to discard pile
			for(j = 0; j < MAX_DECK; j++){
				if(G->discard[0][j] != -1){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Card erroneously added to discard pile at position %d,\n"
						"\tdespite insufficient coins when buying kingdom card at idx: %d",
						j, i);
					}
					testCase == 2 ? failCtT2++ : failCtT3++;
					break;
				}
			}
			
			//Check to make sure discardCount is not changed
			if(G->discardCount[0] != 0){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Discard count erroneously changed.\n"
					"Should remain 0 when coins are insufficient for purchase.\n"
					"Expected: 0 ; Observed %d when buying kingdom card at idx: %d,",
					G->discardCount[0], i);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
			}
				
			if(testCase == 2){
				//Check to make sure coin count is not changed
				if(G->coins != 0){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Coin value erroneously changed.\n"
						"Should remain 0 when coins are insufficient for purchase.\n"
						"Expected: 0 ; Observed %d when buying kingdom card at idx: %d",
						G->coins, i);
					}
					failCtT2++;
				}
				
				//Check to make sure numBuys is not changed
				if(G->numBuys != 1){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"numBuys erroneously changed.\n"
						"Should remain 0 when coins are insufficient for purchase.\n"
						"Expected: 0; Observed %d when buying kingdom card at idx: %d",
						G->numBuys, i);
					}
					failCtT2++;
				}
			}
			else{
				//Check to make sure coin count is not changed
				if(G->coins != 6){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Coin value erroneously changed.\n"
						"Should remain 6 when numBuys = 0.\n"
						"Expected: 0 ; Observed %d when buying kingdom card at idx: %d",
						G->coins, i);
					}
					failCtT3++;
				}
				
				//Check to make sure numBuys is not changed
				if(G->numBuys != 0){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"numBuys erroneously changed.\n"
						"Should remain 0 when numBuys = 0.\n"
						"Expected: 0; Observed %d when buying kingdom card at idx: %d",
						G->numBuys, i);
					}
					testCase == 2 ? failCtT2++ : failCtT3++;
				}
			}
		}	
	}
	
	if(testCase == 1){
		//Check to make sure the supplyCount is changed only as expected
		//for each supply card
		for(j = 0; j < 27; j++){
			if((j == 0 && G->supplyCount[adventurer] != 0) ||
			   (j == 1 && G->supplyCount[ambassador] != 0) ||
			   (j == 2 && G->supplyCount[embargo] != 0) ||
			   (j == 3 && G->supplyCount[village] != 0) ||
			   (j == 4 && G->supplyCount[minion] != 0) ||
			   (j == 5 && G->supplyCount[mine] != 0) ||
			   (j == 6 && G->supplyCount[cutpurse] != 0) ||
			   (j == 7 && G->supplyCount[sea_hag] != 0) ||
			   (j == 8 && G->supplyCount[tribute] != 0) ||
			   (j == 9 && G->supplyCount[smithy] != 0) ||
			   (j == 10 && G->supplyCount[curse] != 10) ||
			   (j == 11 && G->supplyCount[copper] != 60 - 
					(7 * NUM_PLAYERS)) ||
			   (j == 12 && G->supplyCount[silver] != 40) ||
			   (j == 13 && G->supplyCount[gold] != 30) ||
			   (j == 14 && G->supplyCount[estate] != 8) ||
			   (j == 15 && G->supplyCount[duchy] != 8) ||
			   (j == 16 && G->supplyCount[province] != 8) ||
			   (j == 17 && G->supplyCount[council_room] != -1) ||
			   (j == 18 && G->supplyCount[feast] != -1) ||
			   (j == 19 && G->supplyCount[gardens] != -1) ||
			   (j == 20 && G->supplyCount[remodel] != -1) ||
			   (j == 21 && G->supplyCount[baron] != -1) ||
			   (j == 22 && G->supplyCount[great_hall] != -1) ||
			   (j == 23 && G->supplyCount[steward] != -1) ||
			   (j == 24 && G->supplyCount[outpost] != -1) ||
			   (j == 25 && G->supplyCount[salvager] != -1) ||
			   (j == 26 && G->supplyCount[treasure_map] != -1)){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Supply pile count erroneously changed.\n"
						"Check supply pile at idx: %d", j);
					}
					failCtT1++;
					break;
			}
		}
		
		//Final checks of player 0's discard, hand, and deck states...

		//Check to make sure the discard pile state 
		//(for player 0) is as expected
		for(m = 0, n = 0; m < MAX_DECK; m++){
			if((m <= 99 && G->discard[0][m] != k[n]) ||
			   (m > 99 && G->discard[0][m] != -1)){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Discard pile state for player 0 incorrect.\n"
					"Check discard pile idx %d", m);
				}
				failCtT1++;
				break;
			}
			if(m > 8 && (m + 1) % 10 == 0){
				n++;
			}
		}
		
		//Check to make sure the deck pile state 
		//(for player 0) is as expected
		for(m = 0; m < MAX_DECK; m++){
			if(G->deck[0][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Deck state for player 0 incorrect.\n"
					"Check deck idx %d", m);
				}
				failCtT1++;
				break;
			}
		}
		//Check to make sure the hand pile state 
		//(for player 0) is as expected
		for(m = 0; m < MAX_HAND; m++){
			if(G->hand[0][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Hand state for player 0 incorrect.\n"
					"Check hand idx %d", m);
				}
				failCtT1++;
				break;
			}
		}
		
		//Make sure other player's (player 1's) discard, deck, and hand
		//states have not changed
		
		//Check player 1's discard...
		for(m = 0; m < MAX_DECK; m++){
			if(G->discard[1][i] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Discard pile state for player 1 incorrect.\n"
					"Check discard pile idx %d", m);
				}
				failCtT1++;
				break;
			}
		}
		
		//Check player 1's deck...
		for(m = 0; m < MAX_DECK; m++){
			if(G->deck[1][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Deck state for player 1 incorrect.\n"
					"Check deck idx %d", m);
				}
				failCtT1++;
				break;
			}
		}
		
		//Check player 1's hand...
		for(m = 0; m < MAX_HAND; m++){
			if(G->hand[1][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Hand state for player 1 incorrect.\n"
					"Check hand idx %d", m);
				}
				failCtT1++;
				break;
			}
		}
		return failCtT1;
	}
	else{
		//Check to make sure the supplyCount is not changed for each supply card
		for(j = 0; j < 27; j++){
			if((j == 0 && G->supplyCount[adventurer] != 10) ||
			   (j == 1 && G->supplyCount[ambassador] != 10) ||
			   (j == 2 && G->supplyCount[embargo] != 10) ||
			   (j == 3 && G->supplyCount[village] != 10) ||
			   (j == 4 && G->supplyCount[minion] != 10) ||
			   (j == 5 && G->supplyCount[mine] != 10) ||
			   (j == 6 && G->supplyCount[cutpurse] != 10) ||
			   (j == 7 && G->supplyCount[sea_hag] != 10) ||
			   (j == 8 && G->supplyCount[tribute] != 10) ||
			   (j == 9 && G->supplyCount[smithy] != 10) ||
			   (j == 10 && G->supplyCount[curse] != 10) ||
			   (j == 11 && G->supplyCount[copper] != 60 - 
					(7 * NUM_PLAYERS)) ||
			   (j == 12 && G->supplyCount[silver] != 40) ||
			   (j == 13 && G->supplyCount[gold] != 30) ||
			   (j == 14 && G->supplyCount[estate] != 8) ||
			   (j == 15 && G->supplyCount[duchy] != 8) ||
			   (j == 16 && G->supplyCount[province] != 8) ||
			   (j == 17 && G->supplyCount[council_room] != -1) ||
			   (j == 18 && G->supplyCount[feast] != -1) ||
			   (j == 19 && G->supplyCount[gardens] != -1) ||
			   (j == 20 && G->supplyCount[remodel] != -1) ||
			   (j == 21 && G->supplyCount[baron] != -1) ||
			   (j == 22 && G->supplyCount[great_hall] != -1) ||
			   (j == 23 && G->supplyCount[steward] != -1) ||
			   (j == 24 && G->supplyCount[outpost] != -1) ||
			   (j == 25 && G->supplyCount[salvager] != -1) ||
			   (j == 26 && G->supplyCount[treasure_map] != -1)){
					if(++(*failedTestCount) <= MAX_FAILS){
						failures[(*failedTestCount)-1].lineNumber = __LINE__;
						sprintf(failures[(*failedTestCount)-1].description,
						"Supply pile count erroneously changed.\n"
						"Check supply pile at idx: %d", j);
					}
					testCase == 2 ? failCtT2++ : failCtT3++;
					break;
			}
		}
		
		//Final checks of player 0's discard, hand, and deck states...
		
		//Check to make sure the discard pile state 
		//(for player 0) is as expected
		for(m = 0; m < MAX_DECK; m++){
			if(G->discard[0][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Discard pile state for player 0 incorrect.\n"
					"Check discard pile idx %d", m);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
				break;
			}
		}
		//Check to make sure the deck pile state 
		//(for player 0) is as expected
		for(m = 0; m < MAX_DECK; m++){
			if(G->deck[0][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Deck state for player 0 incorrect.\n"
					"Check deck idx %d", m);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
				break;
			}
		}
		//Check to make sure the hand pile state 
		//(for player 0) is as expected
		for(m = 0; m < MAX_HAND; m++){
			if(G->hand[0][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Hand state for player 0 incorrect.\n"
					"Check hand idx %d", m);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
				break;
			}
		}
		
		//Make sure other player's (player 1's) discard, deck, and hand
		//states have not changed
		
		//Check player 1's discard...
		for(m = 0; m < MAX_DECK; m++){
			if(G->discard[1][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Discard pile state for player 1 incorrect.\n"
					"Check discard pile idx %d", m);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
				break;
			}
		}
		
		//Check player 1's deck...
		for(m = 0; m < MAX_DECK; m++){
			if(G->deck[1][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Deck state for player 1 incorrect.\n"
					"Check deck idx %d", m);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
				break;
			}
		}
		
		//Check player 1's hand...
		for(m = 0; m < MAX_HAND; m++){
			if(G->hand[1][m] != -1){
				if(++(*failedTestCount) <= MAX_FAILS){
					failures[(*failedTestCount)-1].lineNumber = __LINE__;
					sprintf(failures[(*failedTestCount)-1].description,
					"Hand state for player 1 incorrect.\n"
					"Check hand idx %d", m);
				}
				testCase == 2 ? failCtT2++ : failCtT3++;
				break;
			}
		}
		if(testCase == 2){
			return failCtT2;
		}
		else{
			return failCtT3;
		}
	}
 }
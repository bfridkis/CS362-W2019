/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *               _cardtest1helper.c
 *
 *		   Helper function for cardtest1.c
 *
 *	 (Tests "shuffle" function - See dominion.c line 202)
 *
 * Include the following lines in your makefile:
 *
 * _cardtest1helper.o: _cardtest1helper.c _cardtest1helper.h dominion.o
 *		gcc -c _cardtest1helper.c -g  $(CFLAGS)
 *
 * ---------------------------------------------------------------------------
 */
 
#include "_cardtest1helper.h"

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in cardtest1.c also!
#define MAX_FAILS 500
 
int _cardtest1helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, cardtest1stats* ut3s, int isEmptyDeckTest, int isMaxDeckTest){
	 
	//Ensure discard, deck, and hand are cleared for players 0 and 1
	memset(G->discard[0], -1, sizeof(int) * MAX_DECK);
	memset(G->deck[0], -1, sizeof(int) * MAX_DECK);
	memset(G->hand[0], -1, sizeof(int) * MAX_DECK);
	
	//Ensure discardCount, deckCount, and handCount are all set to 0
	//for players 0 and 1
	G->discardCount[0] = 0;
	G->deckCount[0] = 0;
	G->handCount[0] = 0;
	G->discardCount[1] = 0;
	G->deckCount[1] = 0;
	G->handCount[1] = 0;
	
	//Test value variables	   
	int i, j, rv, minNumExchanges = 0, deckSize;
	
	if(!isEmptyDeckTest && !isMaxDeckTest){
		//Determine a random deck size in range 2 to MAX_DECK
		deckSize = 2 + (Random() * (MAX_DECK - 2));
	}
	else if(isEmptyDeckTest){
		deckSize = 0;
	}
	else{
		deckSize = MAX_DECK;
	}
	
	//Load player 0's deck with an equal number of each card,
	//plus an extra starting at curse for each remainder after 
	//final multiple of 17. (e.g. a 20 card deck will have 1 
	//of each card plus 1 extra curse, estate, and duchy.)
	for(i = 0, j = 0; i < deckSize; i++){
		if(j < 7){
			G->deck[0][i] = j++;
		}
		else{
			G->deck[0][i] = k[j++ - 7];
		}
		if(j == 17){
			j = 0;
		}
	}
	G->deckCount[0] = deckSize;
	
	//Store deck state info prior to smithy call
	int cardCountByTypeBeforeSmithy[27] = {0};
	int deckCountBeforeShuffle = G->deckCount[0];
	int deckBeforeShuffle[MAX_DECK];
	for(i = 0; i < G->deckCount[0]; i++){
		deckBeforeShuffle[i] = G->deck[0][i];
		cardCountByTypeBeforeShuffle[G->deck[0][i]]++;
	}
	
	//int for bonus (should remain unchanged)
	int coin_bonus = 0;
	
	//Assign a random hand position for smithy,
	//And set hand size accordingly.
	int handPos = Random() * MAX_HAND;
	if(handPos == 500){
		handPos--;
	}
	G->hand[0][handPos] = smithy;
	G->handCount = handPos + 1;
	
	//Call Smithy
	rv = cardEffect(smithy, -1, -2, -3, G, handPos, &coin_bonus);
	
	int cardCountByTypeAfterSmithy[27] = {0};
	
	//********* Left off here... *************
	
	//Check for proper return value
	if(rv != 0 && !isEmptyDeckTest && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Return value not as expected after shuffle\n"
		"  Expected: 0 ; Observed %d\n"
		"  **Aborting remaining tests for this shuffle...**", rv);
		return -1;
	}
	
	//Empty deck tests...
	
	//Check for proper return value (empty deck test)
	if(rv != -1 && isEmptyDeckTest && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Return value not as expected after shuffle\n"
		"  Expected: -1 ; Observed %d\n"
		"  **Aborting remaining tests for this shuffle...**", rv);
		return -1;
	}
	//Check that deckCount remains unchanged after attempting to shuffle
	//empty deck. Return before completing additional tests when testing
	//an empty deck
	else if(isEmptyDeckTest && G->deckCount[0] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Deck count updated (not 0) after empty deck test\n");
		return -1;
	}
	else if(isEmptyDeckTest){
		return 0;
	}
	
	//Check that deck count did not change after shuffle
	if(G->deckCount[0] != deckCountBeforeShuffle 
		&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Deck count changed after shuffle\n"
			"  Expected: %d ; Observed %d\n"
			"  **Aborting remaining tests for this shuffle...**", 
			deckCountBeforeShuffle, G->deckCount[0]);
		return -1;
	}
	
	//Count card types after shuffle
	for(i = 0; i < G->deckCount[0]; i++){
		if(G->deck[0][i] < 27){
			cardCountByTypeAfterShuffle[G->deck[0][i]]++;
		}
		else{
			if(++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Deck contains invalid card number '%d' after shuffle\n"
				"  **Aborting remaining tests for this shuffle...**", 
				G->deck[0][i]);
			return -1;
			}
		}			
	}
	
	//Compare card type counts before and after shuffle
	for(i = 0; i < 27; i++){
		if(cardCountByTypeBeforeShuffle[i] != 
			cardCountByTypeAfterShuffle[i] 
			&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Deck does not contain same number of each card type after shuffle\n"
				"  **Aborting statistics report for this shuffle...**");
				return -1;
		}
	}
	
	//Determine the minimum number of exchanges having occurred between the deck 
	//before and after the shuffle (i.e. as a result of the shuffle).
	//NOTE: This does NOT account for card exchanges taking place between two
	//		cards of the SAME type (and hence the variable name 'minNumExchanges').
	//		(Use stats only as a rough metric accordingly!)
	for(i = 0; i < G->deckCount[0]; i++){
		if(G->deck[0][i] != deckBeforeShuffle[i]){
			minNumExchanges++;
		}
	}

	double percentDeviation = (double)minNumExchanges / (double)G->deckCount[0];
	
	if(minNumExchanges == 0){
		ut3s->_noDev++;
	}
	else if(percentDeviation <= 0.10){
		ut3s->_gt0to10PercentDev++;
	}
	else if(percentDeviation < 0.21){
		ut3s->_11to20PercentDev++;
	}
	else if(percentDeviation < 0.31){
		ut3s->_21to30PercentDev++;
	}	
	else if(percentDeviation < 0.41){
		ut3s->_31to40PercentDev++;
	}
	else if(percentDeviation < 0.51){
		ut3s->_41to50PercentDev++;
	}
	else{
		ut3s->_51PlusPercentDev++;
	}
	
	//Make sure player 1's deckCount and deck cards are unchanged
	if(G->deckCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Player one's deckCount changed after shuffle");
	}
	for(i = 0; i < MAX_DECK; i++){
		if(G->deck[1][i] != -1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Player one's deck changed after shuffle");
			break;
		}
	}
	
	return 0;
}
	
	
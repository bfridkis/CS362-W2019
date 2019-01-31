/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                       _cardtest1helper.c
 *
 *		            Helper function for cardtest1.c
 *
 *	      (Test for "smithy" card effect - See cardEffects.c line 29)
 *
 *            Include the following lines in your makefile:
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
	memset(G->hand[0], -1, sizeof(int) * MAX_HAND);
	memset(G->discard[1], -1, sizeof(int) * MAX_DECK);
	memset(G->deck[1], -1, sizeof(int) * MAX_DECK);
	memset(G->hand[1], -1, sizeof(int) * MAX_HAND);
	
	//Ensure discardCount, deckCount, and handCount are all set to 0
	//for players 0 and 1
	G->discardCount[0] = 0;
	G->deckCount[0] = 0;
	G->handCount[0] = 0;
	G->discardCount[1] = 0;
	G->deckCount[1] = 0;
	G->handCount[1] = 0;
	
	//Ensure whoseTurn, coin, numBuys, numActions, embargoTokens, outpostPlayed,
	//outpostTurn are known values before Smithy play
	G->whoseTurn = 0;
	G->coin = 0;
	G->numBuys = 1;
	G->numActions = 1;
	memset(G->embargoTokens, 0, sizeof(int) * treasure_map + 1);
	G->outpostPlayed = 0;
	G->outpostTurn = 0;
	
	//Ensure supply pile counts are of known value before Smithy play
	//Set each to 10
	for(i = 0, j = 0; i < 27; i++){
		G->supplyCount[i] = 10;
	}
	
	//Test value variables	   
	int i, j, rv, deckSize, handSize;
	
	//Determine random deck and hand sizes in range 1 to MAX_DECK/MAX_HAND
	deckSize = 1 + (Random() * (MAX_DECK - 1));
	handSize = 1 + (Random() * (MAX_HAND - 1));
	
	
	//Load player 0's deck and hand with an equal number of each card,
	//plus an extra starting at curse for each remainder after 
	//final multiple of 17, for each respective random card count 
	//(i.e. deckSize & handSize) as determined above 
	//(e.g. a 20 card deck will have 1 of each card plus 1 extra 
	//curse, estate, and duchy.)
	
	//For deck...
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
	
	//For hand...
	for(i = 0, j = 0; i < handSize; i++){
		if(j < 7){
			G->hand[0][i] = j++;
		}
		else{
			G->hand[0][i] = k[j++ - 7];
		}
		if(j == 17){
			j = 0;
		}
	}
	G->handCount[0] = handSize;
	
	//Store deck and hand state info prior to smithy call
	
	//For deck...
	int deckCountBeforeSmithy = G->deckCount[0];
	int deckBeforeSmithy[MAX_DECK];
	for(i = 0; i < G->deckCount[0]; i++){
		deckBeforeShuffle[i] = G->deck[0][i];
	}
	
	//For hand...
	int handCountBeforeSmithy = G->handCount[0];
	int handBeforeSmithy[MAX_HAND];
	for(i = 0; i < G->handCount[0]; i++){
		handBeforeShuffle[i] = G->hand[0][i];
	}
	
	//int for coin bonus (should remain unchanged after Smithy call)
	int coin_bonus = 0;
	
	//Assign a random hand position for Smithy
	int handPos = Random() * G->handCount[0];
	if(handPos == G->handCount[0]){
		handPos--;
	}
	G->hand[0][handPos] = smithy;
	
	//Call Smithy
	rv = cardEffect(smithy, -1, -1, -1, G, handPos, &coin_bonus);
	
	//Check to make sure G->handCount[0] has been incremented by 2
	//(Smithy should gain 3 cards but discard the played Smithy for 
	//a total net gain of 2)
	if(handCountBeforeSmithy + 2 != G->handCount[0] &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Hand count not updated properly after Smithy play\n"
		"  Expected: %d ; Observed %d\n", 
		handCountBeforeSmithy + 2, G->handCount[0]);
	}

	//Check to make sure G->deckCount[0] has been decremented by 3
	//(Smithy gains 3 cards from deck to hand)
	if(deckCountBeforeSmithy - 3 != G->deckCount[0] &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Deck count not updated properly after Smithy play\n"
		"  Expected: %d ; Observed %d\n", 
		deckCountBeforeSmithy - 3, G->deckCount[0]);
	}	
	
	//Check to make sure that hand (up to the indexes holding the newly 
	//gained cards) is same as before Smithy call, except that all cards 
	//at and after handPos have been slid down one index (i.e. towards index 0). 
	//(This is handled by discardCard, which is called from Smithy. 
	// See dominion.c line 1062).
	for(i = 0, j = 0; i < handCountBeforeSmithy - 1; i++){
		if(((i < handPos && j < 7 && and G->hand[0][i] != i) ||
			(i < handPos && j >= 7 && and G->hand[0][i] != k[j-7]) ||
			(i == handPos && G->hand[0][i]] != handBeforeSmithy[handPos+1])) &&
			++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Hand contents not updated properly after Smithy play\n"
				"  Incorrect card at idx %d ; Observed %d\n", 
				handCountBeforeSmithy, G->handCount[0]);
				break;
		}
		if(i == handPos){
			j++;
		}
		if(j == 16){
			j = 0;
		}
		else{
			j++;
		}
	}
	
	//Make sure top three cards of hand after Smithy play 
	//correspond to top 3 cards of deck before Smithy play
	//(Ordering should be reversed. i.e. First card drawn
	//from deck to hand should be hand's 3rd to last card,
	//second drawn from deck to hand should be hand's 2nd 
	//to last, and third drawn from deck to hand should be 
	//hand's last.
	for(i = 1; i <= 3; i++){
		if(G->hand[0][handCountBeforeSmithy - 1 + (i - 1)] != 
			deckBeforeSmithy[deckCountBeforeSmithy - 1 - (i - 1)] &&
			++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Incorrect card types gained to hand from deck\n"
				"  Expected #%d card gained to be %d ; Observed %d\n", 
				i, deckBeforeSmithy[deckCountBeforeSmithy - 1 - (i - 1)],
				G->hand[0][handCountBeforeSmithy - 1 + (i - 1)]);
				break;
		}
	}
	
	//Make sure deck hasn't changed (i.e. is same as before Smithy
	//play less the top three cards)
	for(i = 0; i < deckCountBeforeSmithy - 3; i++){
		if(G->deck[0][i] != deckBeforeSmithy[i] && 
			++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Deck changed at unexpected index %d\n"
			"  Expected %d ; Observed %d\n", 
			deckBeforeSmithy[i], G->hand[0][i]);
			break;
		}
	}
	
	//Make sure discard pile count hasn't changed
	for(i = 0; i < MAX_DECK; i++){
		if(G->discardCount[0][i] != 0 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile count changed unexpectedly\n"
			"  Expected 0 ; Observed %d\n", G->discardCount[0]);
			break;
		}
	}
	
	//Make sure discard pile hasn't changed
	for(i = 0; i < MAX_DECK; i++){
		if(G->discard[0][i] != -1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile changed unexpectedly at index %d\n"
			"  Expected %d ; Observed %d\n", 
			i, deckBeforeSmithy[i], G->hand[0][i]);
			break;
		}
	}
	
	//Make sure supply piles haven't changed
	for(i = 0; i < 27; i++){
		if(G->supplyCount[i] != 10 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Supply pile count changed unexpectedly at pile %d\n"
			"  Expected 10 ; Observed %d\n", 
			i, G->supplyCount[i]);
			break;
		}
	}
	
	//Check numActions has been decremented by 1
	if(G->numActions != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of actions not updated correctly\n"
		"  Expected 0 ; Observed %d\n", 
		G->numActions);
	}
	
	//Make sure other game state values haven't changed
	
	//Check whoseTurn...
	if(G->whoseTurn != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Whose turn changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->whoseTurn);
	}
	
	//Check coin...
	if(G->coin != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Coin value changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->coin);
	}
	
	//Check numBuys...
	if(G->numBuys != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of buys changed unexpectedly\n"
		"  Expected 1 ; Observed %d\n", 
		G->numBuys);
	}
	
	//Check embargo tokens...
	for(i = 0; i < treasure_map + 1; i++){
		if(G->embargoTokens[i] != 0 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Number of embargo tokens changed unexpectedly for card %d\n"
			"  Expected 0 ; Observed %d\n", 
			i, G->embargoTokens[i]);
		}
	}
	
	//Check outpost played...
	if(G->outpostPlayed != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Outpost played changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->outpostPlayed);
	}
	
	//Check outpost turn...
	if(G->outpostTurn != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Outpost turn changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->outpostTurn);
	}
	
	//Check player 1 values (all should be unchanged)
	
	//Check player 1 discard count...
	if(G->discardCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Player 1 discard count changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->discardCount[1]);
	}
	
	//Check player 1 deck count...
	if(G->deckCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Player 1 deck count changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->deckCount[1]);
	}
	
	//Check player 1 hand count...
	if(G->handCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Player 1 hand count changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->handCount[1]);
	}
	
	//Check player 1 discard pile...
	for(i = 0; i < MAX_DECK; i++){
		if(G->discard[1][i] != -1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Player 1 deck changed unexpectedly at idx %d\n"
			"  Expected -1 ; Observed %d\n", 
			i, G->discard[1][i]);
		}
		break;
	}
	
	//Check player 1 deck...
	for(i = 0; i < MAX_DECK; i++){
		if(G->deck[1][i] != -1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Player 1 deck changed unexpectedly at idx %d\n"
			"  Expected -1 ; Observed %d\n", 
			i, G->deck[1][i]);
		}
		break;
	}
	
	//Check player 1 hand...
	for(i = 0; i < MAX_HAND; i++){
		if(G->hand[1][i] != -1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Player 1 hand changed unexpectedly at idx %d\n"
			"  Expected -1 ; Observed %d\n", 
			i, G->hand[1][i]);
		}
		break;
	}
	
	return 0;
}
	
	
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
#define MAX_FAILS 10
 
int _cardtest1helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int deckCardCountSpecifier){
		
	//Test value variables	   
	int i, j, deckSize, handSize;
	 
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
	
	//Ensure whoseTurn, coins, numBuys, numActions, embargoTokens, outpostPlayed,
	//outpostTurn, playedCard, and playedCardCount are known values before Smithy play
	G->whoseTurn = 0;
	G->coins = 0;
	G->numBuys = 1;
	G->numActions = 1;
	memset(G->embargoTokens, 0, sizeof(int) * treasure_map + 1);
	G->outpostPlayed = 0;
	G->outpostTurn = 0;
	memset(G->playedCards, -1, sizeof(int) * MAX_DECK);
	G->playedCardCount = 0;
	
	//Ensure supply pile counts are of known value before Smithy play
	//Set each to 10
	for(i = 0, j = 0; i < 27; i++){
		G->supplyCount[i] = 10;
	}
	
	//Re-select random stream 2 (since initializeGame will have selected
	//stream 1 in parent function (main, see cardtest1.c)
	SelectStream(2);
	
	//Determine random deck size, based on deckCardCountSpecifier
	//(deckCardCountSpecifier == 3 guarantees a deck size in range
	//3 - MAX_DECK. Otherwise, deckSize == deckCardCountSpecifier.
	//This is so "boundary" conditions of decks with less than 3 cards
	//can be tested.
	if(deckCardCountSpecifier == 3){
		deckSize = 3 + (Random() * (MAX_DECK - 3));
	}
	else{
		deckSize = deckCardCountSpecifier;
	}
	
	//Determine random hand size, in range 1 - MAX_HAND
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
	
	//Store deck and hand info prior to smithy call
	
	//For deck...
	int deckCountBeforeSmithy = G->deckCount[0];
	int deckBeforeSmithy[MAX_DECK];
	for(i = 0; i < G->deckCount[0]; i++){
		deckBeforeSmithy[i] = G->deck[0][i];
	}
	
	//For hand...
	int handCountBeforeSmithy = G->handCount[0];
	int handBeforeSmithy[MAX_HAND];
	for(i = 0; i < G->handCount[0]; i++){
		handBeforeSmithy[i] = G->hand[0][i];
	}
	
	//int for coin bonus (should remain unchanged after Smithy call)
	int coin_bonus = 0;
	
	//Assign a random hand position for Smithy
	int handPos = Random() * G->handCount[0] - 1;
	if(handPos == -1){
		handPos = 0;
	}
	G->hand[0][handPos] = smithy;
	
	/*************************  SET UP ENDS HERE  **************************/
	
	/************************* CHECKS BEGIN HERE  **************************/
	
	//Call Smithy
	cardEffect(smithy, -1, -1, -1, G, handPos, &coin_bonus);
	
	//Check to make sure G->handCount[0] has been incremented by the
	//appropriate amount. Smithy should gain 3 cards but discard the 
	//played Smithy for a total net gain of 2, if the deck contains at 
	//least 3 cards. If the deck contains less than three, the total
	//net gain to the hand will be the deck count - 1, unless the deck
	//count is 0, in which the hand will have one less card than starting
	//(as the Smithy can still technically be played without any cards to draw).
	if(((deckCardCountSpecifier == 0 && 
		handCountBeforeSmithy - 1 != G->handCount[0]) ||
		(deckCardCountSpecifier > 0 && 
		handCountBeforeSmithy + deckCardCountSpecifier - 1
		!= G->handCount[0])) &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Hand count not updated properly after Smithy play\n"
		"  Expected: %d ; Observed %d\n", 
		deckCardCountSpecifier == 0 ? handCountBeforeSmithy - 1 : 
		handCountBeforeSmithy + deckCardCountSpecifier - 1, 
		G->handCount[0]); 
	}

	//Check to make sure G->deckCount[0] has been decremented by 3
	//(Smithy gains 3 cards from deck to hand), or is zero if deck
	//count before Smithy is less than 3
	if(((deckCountBeforeSmithy <= 3 && G->deckCount[0] != 0) ||
		 (deckCountBeforeSmithy > 3 && deckCountBeforeSmithy - 3 
		 != G->deckCount[0])) &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Deck count not updated properly after Smithy play\n"
		"  Expected: %d ; Observed %d\n", 
		deckCountBeforeSmithy < 3 ? 0 :
		deckCountBeforeSmithy - 3, G->deckCount[0]);
	}	
	
	//Check to make sure that hand is same as before Smithy call, except 
	//for the gained cards. The last gained card will go to index handPos,
	//while any others (up to 2) will be at the "top" of the deck.
	//(There is no way to know what is expected at handPos 
	//[the index from which the Smithy is played] vs. what is expected
	//at the "top" of the hand without "breaking into" the smithy function itself.
	//(See dominion.c line 1062 for the details of the discardCard function and
	//cardEffects.c line 29 for more details concerning the smithyEffect function.).
	for(i = 0, j = 0; i < handCountBeforeSmithy - 1; i++){
		if(((i != handPos && j < 7 && G->hand[0][i] != j) ||
			(i != handPos && j >= 7 && G->hand[0][i] != k[j-7])) &&
			++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Hand contents not updated properly after Smithy play\n"
				"  Incorrect card at idx %d ; Expected %d : Observed %d\n", i,
				i != handPos && j < 7 ? i : k[j-7],
				G->hand[0][i]);
				break;
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
	//put into hand at handPos, i.e. where the Smithy that was 
	//just played previously resided. (It is the discardCard 
	//function that puts the last card of the hand after Smithy's 
	//3 draws into handPos.)
	//(Does not check boundary cases [i.e. deck sizes less than 3]).
	for(i = 1; deckCardCountSpecifier == 3 && i <= 3; i++){
		if(((i == 3 && G->hand[0][handPos] != 
			deckBeforeSmithy[deckCountBeforeSmithy - 1 - (i - 1)]) ||
			(i != 3 && G->hand[0][handCountBeforeSmithy + (i - 1)] != 
			deckBeforeSmithy[deckCountBeforeSmithy - 1 - (i - 1)])) &&
			++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Incorrect card types gained to hand from deck,\n"
				"\t\t\t\tor placed incorrectly in hand\n"
				"\tExpected #%d card gained from deck to be %d, and to be\n"
				"\tplaced to %d idx of hand : Observed %d at idx %d\n", 
				i, deckBeforeSmithy[deckCountBeforeSmithy - 1 - (i - 1)],
				i == 3 ? handPos :
				handCountBeforeSmithy + (i - 1),
				i == 3 ? G->hand[0][handPos] :
				G->hand[0][handBeforeSmithy[handCountBeforeSmithy - 1 - (i - 1)]],
				i == 3 ? handPos :
				handCountBeforeSmithy + (i - 1));
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
			"Deck changed unexpectedly at index %d\n"
			"  Expected %d ; Observed %d\n", 
			i, deckBeforeSmithy[i], G->hand[0][i]);
			break;
		}
	}
	
	//Check that Smithy is placed in player 0's discard pile
	if(G->discardCount != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Discard card count not updated correctly\n"
		"  Expected 1 ; Observed %d\n", 
		G->playedCardCount);
	}
	//Check player 0 discard (should have Smithy at index 0, 
	// -1 all other indexes)
	for(i = 0; i < MAX_DECK; i++){
		if(i == 0 && G->discard[0][i] != smithy 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile not updated as expected at idx %d\n"
			"  Expected smithy ; Observed %d\n", 
			i, G->discard[0][i]);
		}
		else if(i != 0 && G->discard[0][i] != -1 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile not updated as expected at idx %d\n"
			"  Expected -1 ; Observed %d\n", 
			i, G->discard[0][i]);
		}
		break;
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
	
	//Make sure other game state values haven't changed
	
	//Check numActions (numActions is updated by parent function
	//playCard, not smithyEffect or any function called by it)...
	if(G->numActions != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of actions not updated correctly\n"
		"  Expected 1 ; Observed %d\n", 
		G->numActions);
	}
	
	//Check whoseTurn...
	if(G->whoseTurn != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Whose turn changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->whoseTurn);
	}
	
	//Check coins...
	if(G->coins != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"coins value changed unexpectedly\n"
		"  Expected 0 ; Observed %d\n", 
		G->coins);
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
	
	//NOTE: I initially wrote this test assuming the discardCard
	//		function was functioning properly (i.e. the Smithy
	//		was supposed to end up in playedCard after its use.
	//		Upon further investigation and consideration, I 
	//		realize that the test here should be to make sure
	//		Smithy gets to the proper discard pile, as this is
	//		its final destination per the game specifications.
	//		There appears to be a bug in discardCard accordingly,
	//		which I discuss in more detail in the assignment writeup.
	
	/* //Check playedCardCount (should be 1)
	if(G->playedCardCount != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Played card count not updated correctly\n"
		"  Expected 1 ; Observed %d\n", 
		G->playedCardCount);
	}
	//Check playedCards (should have Smithy at index 0, -1 all other indexes)
	for(i = 0; i < MAX_DECK; i++){
		if(i == 0 && G->playedCards[i] != smithy 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected smithy ; Observed %d\n", 
			i, G->playedCards[0]);
		}
		else if(i != 0 && G->playedCards[i] != -1 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected -1 ; Observed %d\n", 
			i, G->playedCards[i]);
		}
		break;
	} */
	
	return 0;
}
	
	
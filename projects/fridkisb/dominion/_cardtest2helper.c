/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                       _cardtest2helper.c
 *
 *		            Helper function for cardtest2.c
 *
 *	      (Test for "adventurer" card effect - See cardEffects.c line 5)
 *
 *            Include the following lines in your makefile:
 *
 * _cardtest2helper.o: _cardtest2helper.c _cardtest2helper.h dominion.o
 *		gcc -c _cardtest2helper.c -g  $(CFLAGS)
 *
 * ---------------------------------------------------------------------------
 */
 
#include "_cardtest2helper.h"
 
int _cardtest2helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int treasureCardCountSpecifier, int isBoundary, int testNumber){
		
	//Test value variables	   
	int i, j, deckCountBeforeAdventurer, handCountBeforeAdventurer;
	 
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
	//outpostTurn, playedCard, and playedCardCount are known values before Adventurer play
	G->whoseTurn = 0;
	G->coins = 0;
	G->numBuys = 1;
	G->numActions = 1;
	memset(G->embargoTokens, 0, sizeof(int) * treasure_map + 1);
	G->outpostPlayed = 0;
	G->outpostTurn = 0;
	memset(G->playedCards, -1, sizeof(int) * MAX_DECK);
	G->playedCardCount = 0;
	
	//Ensure supply pile counts are of known value before Adventurer play
	//Set each to 10
	for(i = 0; i < 27; i++){
		G->supplyCount[i] = 10;
	}
	
	//Re-select random stream 2 (since initializeGame will have selected
	//stream 1 in parent function (main, see cardtest2.c)
	SelectStream(2);
	
	//   Determine Deck Size
	if(RANDOMIZE && treasureCardCountSpecifier >= 2){
		//Determine random deck sizes, based on treasureCardCountSpecifier
		//(treasureCardCountSpecifier == 2 guarantees a deck size in range
		//6 - MAX_DECK, which will have at minimum one copper and one silver. 
		deckCountBeforeAdventurer = 6 + (Random() * (MAX_DECK - 6));
	}
	else if(treasureCardCountSpecifier >= 2){
		deckCountBeforeAdventurer = (testNumber * 5) + 1;
		if(deckCountBeforeAdventurer >= MAX_DECK){
			deckCountBeforeAdventurer = (deckCountBeforeAdventurer % MAX_DECK) + 1;
		}
	}
	//A deck size of 5 will contain only one treasure card (copper), and 
	//a deck size of 4 or less will contain no treasure cards. (This is so 
	//"boundary" conditions of decks with less than 2 treasure cards can be 
	//tested.)
	else if(treasureCardCountSpecifier >= 0) {
		deckCountBeforeAdventurer = treasureCardCountSpecifier + 4;
	}
	else{
		deckCountBeforeAdventurer = 0;
	}
	
	//   Determine Hand Size
	//A hand size with a minimum of 6 guarantees at least 2 treasures 
	//in the hand, so that the boundary test cases (i.e. decks with less 
	//than 2 treasures) will not crash the program if the hand also ends 
	//up without at least enough treasures to result in a total of 2 (between 
	//the 2... i.e. there must be at least 2 treasures in the set of total 
	//player cards [deck, discard, hand] to avoid a segmentation fault
	//due to a bug present in the adventurer function [adventurerEffect]).
	//(This bug is detailed in a note above the implementation of 
	//adventurerEffect in cardEffects.c, and in the assignment write-up.)
	if(RANDOMIZE){
		//Determine random hand size, in range 6 - MAX_HAND
		handCountBeforeAdventurer = 6 + (Random() * (MAX_HAND - 6));
	}
	else{
		handCountBeforeAdventurer = (testNumber * 5) + 1;
		if(handCountBeforeAdventurer >= MAX_HAND){
			handCountBeforeAdventurer = (handCountBeforeAdventurer % MAX_HAND) + 1;
			if(handCountBeforeAdventurer < 6){
				handCountBeforeAdventurer = 6;
			}
		}
	}
	
	
	//handCountBeforeAdventurer = 0; <- Testing with handCountBeforeAdventurer 0 fails with Seg Fault or Bus Error!
	//								   <-- -- At latest this occurs when the BOUNDARY test for a deck 
	//								   <-- -- with no treasure cards is executed.
	
	//Load player 0's deck and hand with an equal number of each card,
	//plus an extra starting at curse for each remainder after 
	//final multiple of 17, for each respective random card count 
	//(i.e. deckCountBeforeAdventurer & handCountBeforeAdventurer) as determined above 
	//(e.g. a 20 card deck will have 1 of each card plus 1 extra 
	//curse, estate, and duchy.)
	
	//For deck...
	for(i = 0, j = 0; i < deckCountBeforeAdventurer; i++){
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
	G->deckCount[0] = deckCountBeforeAdventurer;
	
	//For hand...
	for(i = 0, j = 0; i < handCountBeforeAdventurer; i++){
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
	G->handCount[0] = handCountBeforeAdventurer;
	
	//Shuffle player 0's hand 
	//--(shuffle has been tested via unittest3.
	//-- see unittest3.c and _unittest3helper.c
	//-- for additional details.)
	//-- (This is deterministic if RANDOMIZE is not enabled,
	//-- since the same seed value is used with each run.)
	shuffle(0, G);
	
	//Assign hand position for adventurer, either randomly or
	//according to test number
	int handPos;
	if(RANDOMIZE){
		//Assign a random hand position for Adventurer
		handPos = Random() * (G->handCount[0] - 1);
		if(handPos == -1){
			handPos = 0;
		}
	}
	else{
		handPos = 0;
		
		//For test #1, replace province with gold so there is
		//a third treasure card in the deck.
		if(testNumber == 1){
			i = 0;
			while(G->deck[0][i] != province){i++;}
			G->deck[0][i] = gold;
		}
	}
	G->hand[0][handPos] = adventurer;
	
	//Store deck and hand info prior to adventurer call
	
	//For deck...
	int deckCardCountByTypeBeforeAdventurer[27] = {0};
	int deckBeforeAdventurer[MAX_DECK];
	for(i = 0; i < G->deckCount[0]; i++){
		deckBeforeAdventurer[i] = G->deck[0][i];
		deckCardCountByTypeBeforeAdventurer[G->deck[0][i]]++;
	}
	
	//For hand...
	int handCardCountByTypeBeforeAdventurer[27] = {0};
	//int handBeforeAdventurer[MAX_HAND];				//Not needed, since order of hand doesn't matter
	for(i = 0; i < G->handCount[0]; i++){
		//handBeforeAdventurer[i] = G->hand[0][i];
		handCardCountByTypeBeforeAdventurer[G->hand[0][i]]++;
	}
	
	//int for coin bonus (unused by Adventurer call)
	int coin_bonus = 0;
	
	/*************************  SET UP ENDS HERE  **************************/
	
	/************************* CHECKS BEGIN HERE  **************************/
	
	//Call Adventurer
	cardEffect(adventurer, -1, -1, -1, G, handPos, &coin_bonus);
	
	//Check to make sure G->handCount[0] has been incremented by the
	//appropriate amount (i.e. number of treasures gained). Adventurer should 
	//gain 2 treasure cards to hand (so long as two are present in the deck).
	//If only 1 treasure is in the deck, only 1 treasure should be gained
	//and handCount should only increment once, and if no treasures are in 
	//the deck, no treasures should be gained and handCount should remain
	//unchanged.
	if(((treasureCardCountSpecifier == 2 && 
		G->handCount[0] != handCountBeforeAdventurer + 2) ||
		(treasureCardCountSpecifier == 1 && 
		G->handCount[0] != handCountBeforeAdventurer + 1) ||
		(treasureCardCountSpecifier == 0 && 
		G->handCount[0] != handCountBeforeAdventurer)) &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Hand count not updated properly after Adventurer play\n"
		"  Expected: %d ; Observed %d %s\n", 
		treasureCardCountSpecifier == 2 ? handCountBeforeAdventurer + 2 : 
		treasureCardCountSpecifier == 1 ? handCountBeforeAdventurer + 1 : 
		handCountBeforeAdventurer,
		G->handCount[0],
		isBoundary ? "(Boundary)" : "(Non-Boundary)"); 
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Determine how many of each card type have been removed from (or 
	//erroneously added to) the deck
	int deckCardCountByTypeAfterAdventurer[27] = {0};
	for(i = 0; i < G->deckCount[0]; i++){
		deckCardCountByTypeAfterAdventurer[G->deck[0][i]]++;
	}
	int deckDiffsAfterAdventurer[27] = {0};
	for(i = 0; i < 27; i++){
		deckDiffsAfterAdventurer[i] =
			deckCardCountByTypeBeforeAdventurer[i] -
			deckCardCountByTypeAfterAdventurer[i];
	}
	
	//Determine how many of each card type have been added to (or erroneously
	//removed from) the hand
	int handCardCountByTypeAfterAdventurer[27] = {0};
	for(i = 0; i < G->handCount[0]; i++){
		handCardCountByTypeAfterAdventurer[G->hand[0][i]]++;
	}
	int handDiffsAfterAdventurer[27] = {0};
	for(i = 0; i < 27; i++){
		handDiffsAfterAdventurer[i] =
			handCardCountByTypeAfterAdventurer[i] -
			handCardCountByTypeBeforeAdventurer[i];
	}
	
	//Make sure correct number of treasure cards were removed from deck
	if(((treasureCardCountSpecifier == 2 &&
		deckDiffsAfterAdventurer[copper] + 
		deckDiffsAfterAdventurer[silver] + 
		deckDiffsAfterAdventurer[gold] != 2) ||
	   (treasureCardCountSpecifier == 1 &&
		deckDiffsAfterAdventurer[copper] + 
		deckDiffsAfterAdventurer[silver] + 
		deckDiffsAfterAdventurer[gold] != 1) ||
		(treasureCardCountSpecifier == 0 &&
		deckDiffsAfterAdventurer[copper] + 
		deckDiffsAfterAdventurer[silver] + 
		deckDiffsAfterAdventurer[gold] != 0)) &&
		++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Incorrect number of treasure cards removed from deck\n"
			"\tafter Adventurer play\n"
			"  Expected: %d Removed ; Observed %d %s\n", 
			treasureCardCountSpecifier == 2 ? 2 : 
			treasureCardCountSpecifier == 1 ? 1 : 0, 
			deckDiffsAfterAdventurer[copper] + 
			deckDiffsAfterAdventurer[silver] + 
			deckDiffsAfterAdventurer[gold],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Make sure correct number of treasure cards were added to hand
	if(((treasureCardCountSpecifier == 2 &&
		handDiffsAfterAdventurer[copper] + 
		handDiffsAfterAdventurer[silver] + 
		handDiffsAfterAdventurer[gold] != 2) ||
	   (treasureCardCountSpecifier == 1 &&
		handDiffsAfterAdventurer[copper] + 
		handDiffsAfterAdventurer[silver] + 
		handDiffsAfterAdventurer[gold] != 1) ||
		(treasureCardCountSpecifier == 0 &&
		handDiffsAfterAdventurer[copper] + 
		handDiffsAfterAdventurer[silver] + 
		handDiffsAfterAdventurer[gold] != 0)) &&
		++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Incorrect number of treasure cards added to hand\n"
			"  Expected: %d ; Observed %d %s\n", 
			treasureCardCountSpecifier == 2 ? 2 : 
			treasureCardCountSpecifier == 1 ? 1 : 0, 
			handDiffsAfterAdventurer[copper] + 
			handDiffsAfterAdventurer[silver] + 
			handDiffsAfterAdventurer[gold],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Make sure no non-treasure cards have been added to or removed from the hand,
	//besides a single adventurer (card being played)
	for(i = 0; i < 27; i++){
		if((((i < copper || i > gold) && i != adventurer &&
			handDiffsAfterAdventurer[i] != 0) ||
		   (i == adventurer && handDiffsAfterAdventurer[i] != -1)) &&
			++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Non treasure card added to (or removed from) hand,\n"
				"\tOr number of adventurers removed from hand does not equal 1\n\n"
				"  %d %d(s) added to (or removed from) hand %s\n"
				"    (negative value indicates removal)\n", 
				handDiffsAfterAdventurer[i], i,
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
				break;
		}
	}
	
	//Make sure hand count is updated correctly
	if(((treasureCardCountSpecifier == 2 && 
	     G->handCount[0] != handCountBeforeAdventurer + 2) || 
	    (treasureCardCountSpecifier == 1 && 
	     G->handCount[0] != handCountBeforeAdventurer + 1) || 
	    (treasureCardCountSpecifier == 0 && 
	   G->handCount[0] != handCountBeforeAdventurer)) &&
	   ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Hand count not as expected\n"
			"  Expected: %d ; Observed %d %s\n", 
			treasureCardCountSpecifier == 2 ? handCountBeforeAdventurer + 2 : 
			treasureCardCountSpecifier == 1 ? handCountBeforeAdventurer + 1 : 
			handCountBeforeAdventurer, 
			G->handCount[0],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Make sure the same treasure cards that were removed from the
	//deck were those that were added to the hand
	if((handDiffsAfterAdventurer[copper] != deckDiffsAfterAdventurer[copper] ||
	    handDiffsAfterAdventurer[silver] != deckDiffsAfterAdventurer[silver] ||
	    handDiffsAfterAdventurer[gold] != deckDiffsAfterAdventurer[gold]) &&
	   ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Different treasure cards added to hand than removed from deck\n"
			"    Cards added to hand   : Copper - %d Silver - %d Gold - %d \n"
			"  Cards removed from deck : Copper - %d Silver - %d Gold - %d %s\n", 
			handDiffsAfterAdventurer[copper], handDiffsAfterAdventurer[silver], handDiffsAfterAdventurer[gold],
			deckDiffsAfterAdventurer[copper], deckDiffsAfterAdventurer[silver], deckDiffsAfterAdventurer[gold],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Make sure the discard pile has only copies of non-treasure cards 
	//that have been removed from deck (besides adventurer, to be 
	//tested below)
	int discardCardCountByTypeAfterAdventurer[27] = {0};
	for(i = 0; i < G->discardCount[0]; i++){
		discardCardCountByTypeAfterAdventurer[G->discard[0][i]]++;
	}
	for(i = 0; i < 27; i++){
		if(i != copper && i != silver && i != gold && i != adventurer &&
		   discardCardCountByTypeAfterAdventurer[i] !=
		   deckDiffsAfterAdventurer[i] &&
		    ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Different cards or card quantities added to discard pile\n"
				"\t\t\t\t" "than removed from deck\n"
				"    %d '%d' cards added to discard : %d of these removed from deck %s\n", 
				discardCardCountByTypeAfterAdventurer[i], i, 
				deckDiffsAfterAdventurer[i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
				break;
		}
		else if((i >= copper && i <= gold) && 
				discardCardCountByTypeAfterAdventurer[i] != 0 && 
				++(*failCt) <= MAX_FAILS){
					failures[*failCt-1].lineNumber = __LINE__;
					sprintf(failures[*failCt-1].description,
					"Treasure card added to discard pile\n"
					" %d %d(s) observed in discard pile %s\n", 
					discardCardCountByTypeAfterAdventurer[i], i,
					isBoundary ? "(Boundary)" : "(Non-Boundary)");
					failures[*failCt-1].testNumber = testNumber;
					break;
		}
	}
	
	//Make sure discard has adventurer cards numbering
	//1 + the number of adventurers drawn from the deck
	//(1 should come from the hand since it is being played).
	if(discardCardCountByTypeAfterAdventurer[adventurer] != 
		1 + deckDiffsAfterAdventurer[adventurer] &&
		++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile updated incorrectly\n"
			"  Expected %d adventurer in discard pile : Observed %d %s\n",
			1 + deckDiffsAfterAdventurer[adventurer],
			discardCardCountByTypeAfterAdventurer[adventurer],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Make sure deck count has been updated in accordance with the number
	//of cards removed.
	int totalNumCardsRemovedFromDeck = 0;
	for(i = 0; i < 27; i++){
		totalNumCardsRemovedFromDeck += deckDiffsAfterAdventurer[i];
	}
	if(deckCountBeforeAdventurer - totalNumCardsRemovedFromDeck !=
	   G->deckCount[0] && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Deck count after Adventurer does not correspond to cards removed\n"
			" %d cards removed, but resulting deck count is %d %s\n", 
			totalNumCardsRemovedFromDeck, G->deckCount[0],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Make sure the deck order is not changed (for cards remaining in deck)
	for(i = 0; i < deckCountBeforeAdventurer - totalNumCardsRemovedFromDeck &&
			i < G->deckCount[0]; i++){
		if(deckBeforeAdventurer[i] != G->deck[0][i]
			&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Deck after Adventurer re-ordered unexpectedly\n"
				" Expected %d at idx %d ; Observed %d %s\n", 
				deckBeforeAdventurer[i], i, G->deck[0][i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
				break;
		}
	}
	
	//Make sure discard count is updated correctly
	//(Also account for the adventurer itself which should be discarded,
	// such that if two treasures are gained to hand, all other
	// cards removed from the deck - 2 treasures + 1 adventurer should
	// equal the current deck count, and so forth.)
	if(((treasureCardCountSpecifier >= 2 && 
		G->discardCount[0] != totalNumCardsRemovedFromDeck - 1) ||
	   (treasureCardCountSpecifier == 1 &&
		G->discardCount[0] != totalNumCardsRemovedFromDeck) ||
		(treasureCardCountSpecifier <= 0 &&
		G->discardCount[0] != totalNumCardsRemovedFromDeck + 1)) &&
		++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile count updated incorrectly\n"
			"  Expected %d cards in discard pile ; Observed % d %s\n", 
			treasureCardCountSpecifier >= 2 ? totalNumCardsRemovedFromDeck - 1 :
			totalNumCardsRemovedFromDeck == 1 ? totalNumCardsRemovedFromDeck :
			totalNumCardsRemovedFromDeck + 1, G->discardCount[0],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Make sure supply piles haven't changed
	for(i = 0; i < 27; i++){
		if(G->supplyCount[i] != 10 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Supply pile count changed unexpectedly at pile %d\n"
			"  Expected 10 ; Observed %d %s\n", 
			i, G->supplyCount[i],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
			break;
		}
	}
	
	//Make sure other game state values haven't changed
	
	//Check numActions (numActions is updated by parent function
	//playCard, not adventurerEffect or any function called by it)...
	if(G->numActions != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of actions not updated correctly\n"
		"  Expected 1 ; Observed %d\n %s", 
		G->numActions,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check whoseTurn...
	if(G->whoseTurn != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Whose turn changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->whoseTurn,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check coins...
	if(G->coins != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"coins value changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->coins,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check numBuys...
	if(G->numBuys != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of buys changed unexpectedly\n"
		"  Expected 1 ; Observed %d %s\n", 
		G->numBuys,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check embargo tokens...
	for(i = 0; i < treasure_map + 1; i++){
		if(G->embargoTokens[i] != 0 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Number of embargo tokens changed unexpectedly for card %d\n"
			"  Expected 0 ; Observed %d %s\n", 
			i, G->embargoTokens[i],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//Check outpost played...
	if(G->outpostPlayed != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Outpost played changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->outpostPlayed,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check outpost turn...
	if(G->outpostTurn != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Outpost turn changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->outpostTurn,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check player 1 values (all should be unchanged)
	
	//Check player 1 discard count...
	if(G->discardCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Player 1 discard count changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->discardCount[1],
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check player 1 deck count...
	if(G->deckCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Player 1 deck count changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->deckCount[1],
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check player 1 hand count...
	if(G->handCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Player 1 hand count changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->handCount[1],
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check player 1 discard pile...
	for(i = 0; i < MAX_DECK; i++){
		if(G->discard[1][i] != -1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Player 1 deck changed unexpectedly at idx %d\n"
			"  Expected -1 ; Observed %d %s\n", 
			i, G->discard[1][i],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	//Check player 1 deck...
	for(i = 0; i < MAX_DECK; i++){
		if(G->deck[1][i] != -1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Player 1 deck changed unexpectedly at idx %d\n"
			"  Expected -1 ; Observed %d %s\n", 
			i, G->deck[1][i],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	//Check player 1 hand...
	for(i = 0; i < MAX_HAND; i++){
		if(G->hand[1][i] != -1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Player 1 hand changed unexpectedly at idx %d\n"
			"  Expected -1 ; Observed %d %s\n", 
			i, G->hand[1][i],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	//Check playedCardCount (should be 0)
	if(G->playedCardCount != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Played card count not updated correctly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->playedCardCount,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	//Check playedCards (should be unchanged)
	for(i = 0; i < MAX_DECK; i++){
		if(G->playedCards[i] != -1 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected -1 ; Observed %d %s\n", 
			i, G->playedCards[i],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	return 0;
}
	
	

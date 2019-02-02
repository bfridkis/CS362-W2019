/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                       _cardtest3helper.c
 *
 *		            Helper function for cardtest3.c
 *
 *	      (Test for "cutpurse" card effect - See cardEffects.c line 5)
 *
 *            Include the following lines in your makefile:
 *
 * _cardtest3helper.o: _cardtest3helper.c _cardtest3helper.h dominion.o
 *		gcc -c _cardtest3helper.c -g  $(CFLAGS)
 *
 * ---------------------------------------------------------------------------
 */
 
#include "_cardtest3helper.h"

//This max is only for documenting failure specifics.
//i.e. Failures in excess of this number are still counted, but not documented.
//Set this in cardtest3.c also!
#define MAX_FAILS 10
 
int _cardtest3helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int treasureCardCountSpecifier, int isBoundary){
		
	//Test value variables	   
	int i, j, m, deckSize, handSize[MAX_PLAYERS];
	
	//Ensure discardCount, deckCount, and handCount are all set to 0
	//and discard, deck, and hand are cleared for all players
	for(i = 0; i < playerCount; i++){
		G->discardCount[i] = 0;
		G->deckCount[i] = 0;
		G->handCount[i] = 0;
		memset(G->discard[i], -1, sizeof(int) * MAX_DECK);
		memset(G->deck[i], -1, sizeof(int) * MAX_DECK);
		memset(G->hand[i], -1, sizeof(int) * MAX_HAND);
	}
	
	//Ensure whoseTurn, coins, numBuys, numActions, embargoTokens, outpostPlayed,
	//outpostTurn, playedCard, and playedCardCount are known values before Cutpurse play
	G->whoseTurn = 0;
	G->coins = 0;
	G->numBuys = 1;
	G->numActions = 1;
	memset(G->embargoTokens, 0, sizeof(int) * treasure_map + 1);
	G->outpostPlayed = 0;
	G->outpostTurn = 0;
	memset(G->playedCards, -1, sizeof(int) * MAX_DECK);
	G->playedCardCount = 0;
	
	//Ensure supply pile counts are of known value before Cutpurse play
	//Set each to 10
	for(i = 0, j = 0; i < 27; i++){
		G->supplyCount[i] = 10;
	}
	
	//Re-select random stream 2 (since initializeGame will have selected
	//stream 1 in parent function (main, see cardtest3.c)
	SelectStream(2);
	
	
	//Determine random hand size for each player in 
	//range 1 - MAX_HAND
	for(i = 0; i < playerCount; i++){
		handSize[i] = 1 + Random() * (MAX_HAND - 1);
	}
	
	//Load each player's hand with an equal number of each card,
	//plus an extra starting at curse for each remainder after 
	//final multiple of 17, for handSize as determined above 
	//(e.g. a 20 card deck will have 1 of each card plus 1 extra 
	//curse, estate, and duchy.)
	for(m = 0; m < playerCount; m++){
		for(i = 0, j = 0; i < handSize[m]; i++){
			if(j < 7){
				G->hand[k][i] = j++;
			}
			else{
				G->hand[k][i] = k[j++ - 7];
			}
			if(j == 17){
				j = 0;
			}
		}
		G->handCount[k] = handSize[k];
	}
	
	//Assign a random hand position for Cutpurse 
	//for each player
	int handPos[MAX_PLAYERS];
	for(i = 0 ; i < playerCount; i++){
		handPos[i] = Random() * G->handCount[i] - 1;
		if(handPos == -1){
			handPos = 0;
		}
		G->hand[i][handPos[i]] = cutpurse;
	}
	
	//If this is not the boundary test in which players
	//have no copper, add a copper to any player with a 
	//hand size less than 4 (any deck size greater or
	//equal to 4 is guaranteed to have at least 1 copper.
	if(!noCopper){
		for(i = 0; i < playerCount; i++){
			if(handSize[i] < 4){
				G->hand[i][Random() * (handSize[i] - 1)] = copper;
			}
		}
	}
	//If this is the boundary test (no copper in hand) replace
	//all copper with silver
	else{
		for(i = 0; i < playerCount; i++){
			for(j = 4; j < handSize[i]; j += 17){
				G->hand[i][j] = silver;
			}
		}
	}
	
	//Shuffle each player's hand 
	//--(shuffle has been tested via unittest3.
	//-- see unittest3.c and _unittest3helper.c
	//-- for additional details.)
	for(i = 0; i < playerCount; i++){
		shuffle(i, G);
	}
	
	//Store hand card counts prior to cutpurse call for each player
	int handCardCountByTypeBeforeCutpurse[MAX_PLAYERS][27] = {0};
	for(i = 0; i < playerCount; i++){
		for(j = 0; j < G->handCount[i]; j++){
			handCardCountByTypeBeforeCutpurse[i][G->hand[i][j]]++;
		}
	}
	
	//Determine random player to play cut
	int activePlayer = Random() * playerCount - 1;
	G->whoseTurn = activePlayer;
	
	//int for coin bonus (should remain unchanged after Cutpurse call)
	int coin_bonus = 0;
	
	/*************************  SET UP ENDS HERE  **************************/
	
	/************************* CHECKS BEGIN HERE  **************************/
	
	//Call Cutpurse
	cardEffect(cutpurse, -1, -1, -1, G, handPos[activePlayer, &coin_bonus);
	
	//Check to make sure G->handCount[0] has been incremented by the
	//appropriate amount (i.e. number of treasures gained). Cutpurse should 
	//gain 2 treasure cards to hand (so long as two are present in the deck).
	//If only 1 treasure is in the deck, only 1 treasure should be gained
	//and handCount should only increment once, and if no treasures are in 
	//the deck, no treasures should be gained and handCount should remain
	//unchanged.
	if(((treasureCardCountSpecifier == 2 && 
		G->handCount[0] != handCountBeforeCutpurse + 2) ||
		(treasureCardCountSpecifier == 1 && 
		G->handCount[0] != handCountBeforeCutpurse + 1) ||
		(treasureCardCountSpecifier == 0 && 
		G->handCount[0] != handCountBeforeCutpurse)) &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Hand count not updated properly after Cutpurse play\n"
		"  Expected: %d ; Observed %d %s\n", 
		treasureCardCountSpecifier == 2 ? handCountBeforeCutpurse + 2 : 
		treasureCardCountSpecifier == 1 ? handCountBeforeCutpurse + 1 : 
		handCountBeforeCutpurse,
		G->handCount[0],
		isBoundary ? "(Boundary)" : "(Non-Boundary)"); 
	}
	
	//Determine how many of each card type have been removed from (or 
	//erroneously added to) the deck
	int deckCardCountByTypeAfterCutpurse[27] = {0};
	for(i = 0; i < G->deckCount[0]; i++){
		deckCardCountByTypeAfterCutpurse[G->deck[0][i]]++;
	}
	int deckDiffsAfterCutpurse[27] = {0};
	for(i = 0; i < 27; i++){
		deckDiffsAfterCutpurse[i] =
			deckCardCountByTypeBeforeCutpurse[i] -
			deckCardCountByTypeAfterCutpurse[i];
	}
	
	//Determine how many of each card type have been added to (or erroneously
	//removed from) the deck
	int handCardCountByTypeAfterCutpurse[27] = {0};
	for(i = 0; i < G->handCount[0]; i++){
		handCardCountByTypeAfterCutpurse[G->hand[0][i]]++;
	}
	int handDiffsAfterCutpurse[27] = {0};
	for(i = 0; i < 27; i++){
		handDiffsAfterCutpurse[i] =
			handCardCountByTypeAfterCutpurse[i] -
			handCardCountByTypeBeforeCutpurse[i];
	}
	
	//Make sure correct number of treasure cards were removed from deck
	if(((treasureCardCountSpecifier == 2 &&
		deckDiffsAfterCutpurse[copper] + 
		deckDiffsAfterCutpurse[silver] + 
		deckDiffsAfterCutpurse[gold] != 2) ||
	   (treasureCardCountSpecifier == 1 &&
		deckDiffsAfterCutpurse[copper] + 
		deckDiffsAfterCutpurse[silver] + 
		deckDiffsAfterCutpurse[gold] != 1) ||
		(treasureCardCountSpecifier == 0 &&
		deckDiffsAfterCutpurse[copper] + 
		deckDiffsAfterCutpurse[silver] + 
		deckDiffsAfterCutpurse[gold] != 0)) &&
		++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Incorrect number of treasure cards removed from deck after Cutpurse play\n"
			"  Expected: %d Removed ; Observed %d %s\n", 
			treasureCardCountSpecifier == 2 ? 2 : 
			treasureCardCountSpecifier == 1 ? 1 : 0, 
			deckDiffsAfterCutpurse[copper] + 
			deckDiffsAfterCutpurse[silver] + 
			deckDiffsAfterCutpurse[gold],
			isBoundary ? "(Boundary)" : "(Non-Boundary)"); 
	}
	
	//Make sure correct number of treasure cards were added to hand
	if(((treasureCardCountSpecifier == 2 &&
		handDiffsAfterCutpurse[copper] + 
		handDiffsAfterCutpurse[silver] + 
		handDiffsAfterCutpurse[gold] != 2) ||
	   (treasureCardCountSpecifier == 1 &&
		handDiffsAfterCutpurse[copper] + 
		handDiffsAfterCutpurse[silver] + 
		handDiffsAfterCutpurse[gold] != 1) ||
		(treasureCardCountSpecifier == 0 &&
		handDiffsAfterCutpurse[copper] + 
		handDiffsAfterCutpurse[silver] + 
		handDiffsAfterCutpurse[gold] != 0)) &&
		++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Incorrect number of treasure cards added to hand\n"
			"  Expected: %d ; Observed %d %s\n", 
			treasureCardCountSpecifier == 2 ? 2 : 
			treasureCardCountSpecifier == 1 ? 1 : 0, 
			handDiffsAfterCutpurse[copper] + 
			handDiffsAfterCutpurse[silver] + 
			handDiffsAfterCutpurse[gold],
			isBoundary ? "(Boundary)" : "(Non-Boundary)"); 
	}
	
	//Make sure no non-treasure cards have been added to the hand
	for(i = 0; i < 27; i++){
		if((i < copper || i > gold) && handDiffsAfterCutpurse[i] != 0 &&
			++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Non treasure card added to hand pile\n"
				" %d %d(s) added to hand %s\n", 
				handDiffsAfterCutpurse[i], i,
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				break;
		}
	}
	
	//Make sure hand count is updated correctly
	if(((treasureCardCountSpecifier == 2 && 
	     G->handCount[0] != handCountBeforeCutpurse + 2) || 
	    (treasureCardCountSpecifier == 1 && 
	     G->handCount[0] != handCountBeforeCutpurse + 1) || 
	    (treasureCardCountSpecifier == 0 && 
	   G->handCount[0] != handCountBeforeCutpurse)) &&
	   ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Hand count not as expected\n"
			"  Expected: %d Added ; Observed %d %s\n", 
			treasureCardCountSpecifier == 2 ? handCountBeforeCutpurse + 2 : 
			treasureCardCountSpecifier == 1 ? handCountBeforeCutpurse + 1 : 
			handCountBeforeCutpurse, 
			G->handCount[0],
			isBoundary ? "(Boundary)" : "(Non-Boundary)"); 
	}
	
	//Make sure the same treasure cards that were removed from the
	//deck were those that were added to the hand
	if((handDiffsAfterCutpurse[copper] != deckDiffsAfterCutpurse[copper] ||
	    handDiffsAfterCutpurse[silver] != deckDiffsAfterCutpurse[silver] ||
	    handDiffsAfterCutpurse[gold] != deckDiffsAfterCutpurse[gold]) &&
	   ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Different treasure cards added to hand than removed from deck\n"
			"    Cards added to hand   : Copper - %d Silver - %d Gold - %d \n"
			"  Cards removed from deck : Copper - %d Silver - %d Gold - %d %s\n", 
			handDiffsAfterCutpurse[copper], handDiffsAfterCutpurse[silver], handDiffsAfterCutpurse[gold],
			deckDiffsAfterCutpurse[copper], deckDiffsAfterCutpurse[silver], deckDiffsAfterCutpurse[gold],
			isBoundary ? "(Boundary)" : "(Non-Boundary)"); 
	}
	
	//Make sure the discard pile has only copies of non-treasure cards 
	//that have been removed from deck
	int discardCardCountByTypeAfterCutpurse[27] = {0};
	for(i = 0; i < G->discardCount[0]; i++){
		discardCardCountByTypeAfterCutpurse[G->discard[0][i]]++;
	}
	for(i = 0; i < 27; i++){
		if(i != copper && i != silver && i != gold && 
		   discardCardCountByTypeAfterCutpurse[i] !=
		   deckDiffsAfterCutpurse[i] &&
		    ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Different cards or card quantities added to discard pile\n"
				"\t\t\t\t" "than removed from deck\n"
				"    %d '%d' cards added to discard : %d of these removed from deck %s\n", 
				discardCardCountByTypeAfterCutpurse[i], i, deckDiffsAfterCutpurse[i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				break;
		}
		else if((i >= copper && i <= gold) && 
				deckDiffsAfterCutpurse[i] != 
				handDiffsAfterCutpurse[i] && 
				++(*failCt) <= MAX_FAILS){
					failures[*failCt-1].lineNumber = __LINE__;
					sprintf(failures[*failCt-1].description,
					"Treasure card added to discard pile\n"
					" %d %d(s) observed in discard pile %s\n", 
					discardCardCountByTypeAfterCutpurse[i], i,
					isBoundary ? "(Boundary)" : "(Non-Boundary)");
					break;
		}
	}
	
	//Make sure deck count has been updated in accordance with the number
	//of cards removed.
	int totalNumCardsRemovedFromDeck = 0;
	for(i = 0; i < 27; i++){
		totalNumCardsRemovedFromDeck += deckDiffsAfterCutpurse[i];
	}
	if(deckCountBeforeCutpurse - totalNumCardsRemovedFromDeck !=
	   G->deckCount[0] && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Deck count after Cutpurse does not correspond to cards removed\n"
			" %d cards removed, but resulting deck count is %d %s\n", 
			totalNumCardsRemovedFromDeck, G->deckCount[0],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
	}
	
	//Make sure discard count is updated correctly
	if(((treasureCardCountSpecifier >= 2 && 
		G->discardCount[0] != totalNumCardsRemovedFromDeck - 2) ||
	   (treasureCardCountSpecifier == 1 &&
		G->discardCount[0] != totalNumCardsRemovedFromDeck - 1) ||
		(treasureCardCountSpecifier <= 0 &&
		G->discardCount[0] != totalNumCardsRemovedFromDeck)) &&
		++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile count updated incorrectly\n"
			"Expected %d cards in discard pile ; Observed % d %s\n", 
			treasureCardCountSpecifier >= 2 ? totalNumCardsRemovedFromDeck - 2 :
			totalNumCardsRemovedFromDeck == 1 ? totalNumCardsRemovedFromDeck - 1 :
			totalNumCardsRemovedFromDeck, G->discardCount[0],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
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
			break;
		}
	}
	
	//Make sure other game state values haven't changed
	
	//Check numActions ...
	if(G->numActions != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of actions not updated correctly\n"
		"  Expected 0 ; Observed %d\n %s", 
		G->numActions,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
	}
	
	//Check whoseTurn...
	if(G->whoseTurn != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Whose turn changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->whoseTurn,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
	}
	
	//Check coins...
	if(G->coins != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"coins value changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->coins,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
	}
	
	//Check numBuys...
	if(G->numBuys != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of buys changed unexpectedly\n"
		"  Expected 1 ; Observed %d %s\n", 
		G->numBuys,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
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
	}
	
	//Check outpost turn...
	if(G->outpostTurn != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Outpost turn changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->outpostTurn,
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
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
	}
	
	//Check player 1 deck count...
	if(G->deckCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Player 1 deck count changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->deckCount[1],
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
	}
	
	//Check player 1 hand count...
	if(G->handCount[1] != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Player 1 hand count changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->handCount[1],
		isBoundary ? "(Boundary)" : "(Non-Boundary)");
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
		}
		break;
	}
	
	return 0;
}
	
	
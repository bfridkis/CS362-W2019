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
 
int _cardtest3helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int noCopper, int testNumber){
		
	//Test value variables	   
	int i, j, m;
	
	//Ensure discardCount, deckCount, and handCount are all set to 0
	//and discard, deck, and hand are cleared for all players
	for(i = 0; i < NUM_PLAYERS; i++){
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
	for(i = 0; i < 27; i++){
		G->supplyCount[i] = 10;
	}
	
	//Re-select random stream 2 (since initializeGame will have selected
	//stream 1 in parent function (main, see cardtest3.c)
	SelectStream(2);
	
	//Assign hand size, either randomly if indicated, or according
	//to test number
	int handSize[NUM_PLAYERS];
	if(RANDOMIZE){
		//Determine random hand size for each player in 
		//range 1 - MAX_HAND
		for(i = 0; i < NUM_PLAYERS; i++){
			handSize[i] = 1 + (Random() * (MAX_HAND - 1));
		}
	}
	else{
		for(i = 0; i < NUM_PLAYERS; i++){
			handSize[i] = testNumber * 5;
			if(handSize[i] >= MAX_HAND){
				handSize[i] = (handSize[i] % MAX_HAND) + 1;
			}
		}
	}
	
	//Load each player's hand with an equal number of each card,
	//plus an extra starting at curse for each remainder after 
	//final multiple of 17, for handSize as determined above 
	//(e.g. a 20 card deck will have 1 of each card plus 1 extra 
	//curse, estate, and duchy.)
	for(m = 0; m < NUM_PLAYERS; m++){
		for(i = 0, j = 0; i < handSize[m]; i++){
			if(j < 7){
				G->hand[m][i] = j++;
			}
			else{
				G->hand[m][i] = k[j++ - 7];
			}
			if(j == 17){
				j = 0;
			}
		}
		G->handCount[m] = handSize[m];
	}
	
	//Determine active player, randomly if indicated
	int activePlayer;
	if(RANDOMIZE){
		//Determine random player to play cutpurse
		activePlayer = Random() * (NUM_PLAYERS - 1);
		G->whoseTurn = activePlayer;
	}
	else{
		activePlayer = 0;
	}
	
	//Assign a hand position for Cutpurse for active player
	int handPos;
	if(RANDOMIZE){
		handPos = Random() * (G->handCount[activePlayer] - 1);
		if(handPos == -1){
			handPos = 0;
		}
	}
	else{
		handPos = 0;
	}
	G->hand[activePlayer][handPos] = cutpurse;
	
	//If this is not the boundary test in which players
	//have no copper, add a copper to any player with a 
	//hand size less than 4 (any deck size greater or
	//equal to 4 is guaranteed to have at least 1 copper
	//already).
	if(!noCopper){
		for(i = 0; i < NUM_PLAYERS; i++){
			if(handSize[i] < 4){
				G->hand[i][(int)Random() * (handSize[i] - 1)] = copper;
			}
		}
	}
	//If this is the boundary test (no copper in hand) replace
	//all copper with silver
	else{
		for(i = 0; i < NUM_PLAYERS; i++){
			for(j = 4; j < handSize[i]; j += 17){
				G->hand[i][j] = silver;
			}
		}
	}
	
	//Store hand card counts prior to cutpurse call for each player
	int handCardCountByTypeBeforeCutpurse[MAX_PLAYERS][27] = {{0}};
	for(i = 0; i < NUM_PLAYERS; i++){
		for(j = 0; j < G->handCount[i]; j++){
			handCardCountByTypeBeforeCutpurse[i][G->hand[i][j]]++;
		}
	}
	
	//Determine expected coin count prior to cutpurse call
	int expectedCoins = 0;
	for(i = 0; i < G->handCount[activePlayer]; i++){
		if(G->hand[activePlayer][i] == copper){
			expectedCoins += 1;
		}
		else if(G->hand[activePlayer][i] == silver){
			expectedCoins += 2;
		}
		if(G->hand[activePlayer][i] == gold){
			expectedCoins += 3;
		}
	}
	
	//int for coin bonus (unused by Cutpurse call)
	int coin_bonus = 0;
	
	/*************************  SET UP ENDS HERE  **************************/
	
	/************************* CHECKS BEGIN HERE  **************************/
	
	//Call Cutpurse
	cardEffect(cutpurse, -1, -1, -1, G, handPos, &coin_bonus);
	
	//Store hand card counts after cutpurse call for each player
	int handCardCountByTypeAfterCutpurse[MAX_PLAYERS][27] = {{0}};
	for(i = 0; i < NUM_PLAYERS; i++){
		for(j = 0; j < G->handCount[i]; j++){
			handCardCountByTypeAfterCutpurse[i][G->hand[i][j]]++;
		}
	}
	
	//If this is not the no coppers boundary test,
	//check to make sure each player's hand contains the same
	//cards as before the cutpurse play minus one (and only one)
	//copper for all non-active players. For active player, make
	//sure hand is the same as before the cutpurse play minus 
	//one cutpurse only.
	for(i = 0; i < NUM_PLAYERS && !noCopper; i++){
		for(j = 0; j < 27; j++){
			if( i != activePlayer && 
			  ((j == copper && handCardCountByTypeBeforeCutpurse[i][j] - 1 != 
			    handCardCountByTypeAfterCutpurse[i][j]) ||
			   ((j < copper || j > copper) && 
			    handCardCountByTypeBeforeCutpurse[i][j] !=
			    handCardCountByTypeAfterCutpurse[i][j])) &&
				++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Hand cards not updated properly after Cutpurse play\n"
				"  Incorrect number of %d cards for player %d\n"
				"   (player who did not play cutpurse)\n"
				"  Expected: %d ; Observed %d %s\n", 
				j, i, 
				j == copper ? handCardCountByTypeBeforeCutpurse[i][j] - 1 :
				handCardCountByTypeBeforeCutpurse[i][j],
				handCardCountByTypeAfterCutpurse[i][j],
				noCopper ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			else if( i == activePlayer && 
				  ((j == cutpurse && handCardCountByTypeBeforeCutpurse[i][j] - 1 != 
					handCardCountByTypeAfterCutpurse[i][j]) ||
				   ((j < cutpurse || j > cutpurse) && 
					handCardCountByTypeBeforeCutpurse[i][j] !=
					handCardCountByTypeAfterCutpurse[i][j])) &&
					++(*failCt) <= MAX_FAILS){
					failures[*failCt-1].lineNumber = __LINE__;
					sprintf(failures[*failCt-1].description,
					"Hand cards not updated properly after Cutpurse play\n"
					"  Incorrect number of %d cards for player %d\n"
					"    (player who played cutpurse)\n"
					"  Expected: %d ; Observed %d %s\n", 
					j, i, 
					j == cutpurse ? handCardCountByTypeBeforeCutpurse[i][j] - 1 :
					handCardCountByTypeBeforeCutpurse[i][j],
					handCardCountByTypeAfterCutpurse[i][j],
					noCopper ? "(Boundary)" : "(Non-Boundary)");
					failures[*failCt-1].testNumber = testNumber;
			}
		}
	}
	
	//If this is not the no coppers boundary test,
	//Check to make sure all hand counts are decremented by one.
	//(Non-active players discard 1 copper, active player discards
	// 1 cutpurse.)
	for(i = 0; i < NUM_PLAYERS && !noCopper; i++){
		if(handSize[i] - 1 != G->handCount[i] &&
			++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Hand counts not updated properly after Cutpurse play\n"
			"  Expected: %d for player %d; Observed %d %s\n"
			"    (player who %s cutpurse)\n",
			handSize[i] - 1, i, G->handCount[i],
			noCopper ? "(Boundary)" : "(Non-Boundary)",
			i == activePlayer ? "played" : "did not play");
			failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//If this is the no coppers boundary test,
	//check to make sure each player's hand contains the same
	//cards as before the cutpurse play. For active player, make
	//sure hand is the same as before the cutpurse play minus one 
	//cutpurse only.
	for(i = 0; i < NUM_PLAYERS && noCopper; i++){
		for(j = 0; j < 27; j++){
			if(i != activePlayer && 
			   handCardCountByTypeBeforeCutpurse[i][j] != 
			   handCardCountByTypeAfterCutpurse[i][j] && 
			   ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Hand cards not updated properly after Cutpurse play\n"
				"  Incorrect number of %d cards for player %d\n"
				"   (player who did not play cutpurse)\n"
				"  Expected: %d ; Observed %d %s\n", 
				j, i, handCardCountByTypeBeforeCutpurse[i][j],
				handCardCountByTypeAfterCutpurse[i][j],
				noCopper ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			else if( i == activePlayer && 
				   ((j == cutpurse && handCardCountByTypeBeforeCutpurse[i][j] - 1 != 
				     handCardCountByTypeAfterCutpurse[i][j]) ||
				   ((j < cutpurse || j > cutpurse) && handCardCountByTypeBeforeCutpurse[i][j] !=
					handCardCountByTypeAfterCutpurse[i][j])) &&
					++(*failCt) <= MAX_FAILS){
					failures[*failCt-1].lineNumber = __LINE__;
					sprintf(failures[*failCt-1].description,
					"Hand cards not updated properly after Cutpurse play\n"
					"  Incorrect number of %d cards for player %d\n"
					"    (player who played cutpurse)\n"
					"  Expected: %d ; Observed %d %s\n", 
					j, i, 
					j == cutpurse ? handCardCountByTypeBeforeCutpurse[i][j] - 1 :
					handCardCountByTypeBeforeCutpurse[i][j],
					handCardCountByTypeAfterCutpurse[i][j],
					noCopper ? "(Boundary)" : "(Non-Boundary)");
					failures[*failCt-1].testNumber = testNumber;
			}
		}
	}
	
	//Make sure no deck counts have changed
	for(i = 0; i < NUM_PLAYERS; i++){
		if(G->deckCount[i] != 0 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Deck Count for player %d unexpectedly changed\n"
			"    (player who %s cutpurse)\n"
			"  Expected: 0 ; Observed %d %s\n", 
			i, 
			i == activePlayer ? "played" : "did not play",
			G->deckCount[i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//Make sure no decks have changed
	for(i = 0; i < NUM_PLAYERS; i++){
		for(j = 0; j < MAX_DECK; j++){
			if(G->deck[i][j] != -1 && ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Deck for player %d unexpectedly changed\n"
				"    (player who %s cutpurse)\n"
				"  Expected: -1 at idx %d ; Observed %d %s\n", 
				i, 
				i == activePlayer ? "played" : "did not play",
				j, G->deck[i][j],
				noCopper ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
				break;
			}
		}
	}
	
	//Check coins...
	if(G->coins != expectedCoins + 2 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Coins value not updated correctly\n"
		"  Expected %d ; Observed %d %s\n"
		"    (Active player started with %d coin%s)\n",
		expectedCoins + 2, G->coins,
		noCopper ? "(Boundary)" : "(Non-Boundary)",
		expectedCoins,
		expectedCoins == 1 ? "" : "s");
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
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
			break;
		}
	}
	
	//If this is not the no copper boundary check, 
	//check that discard count for each player is 1, as active player
	//discards cutpurse and inactive players discard copper...
	for(i = 0; i < NUM_PLAYERS && !noCopper; i++){
		if(G->discardCount[i] != 1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard card count not updated correctly for player %d\n"
			"    (player who %s cutpurse)\n"
			"  Expected 1 ; Observed %d %s\n", 
			i, 
			i == activePlayer ? "played" : "did not play",
			G->discardCount[i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//If this is the no copper boundary check, 
	//check that discard count for active player is 1, and 0
	//for all other players, as the active player discards cutpurse
	//and no other players discard
	for(i = 0; i < NUM_PLAYERS && noCopper; i++){
		if(i == activePlayer && G->discardCount[i] != 1 && 
			++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard card count not updated correctly\n"
			"    (player who played cutpurse)\n"
			"  Expected 1 ; Observed %d %s\n", 
			G->discardCount[i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		else if(i != activePlayer && G->discardCount[i] != 0 && 
			++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard card count not updated correctly\n"
			"    (player who did not play cutpurse)\n"
			"  Expected 0 ; Observed %d %s\n", 
			G->discardCount[i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//Check that cutpurse is placed in the active player's discard pile,
	//and that no other discard pile index value (for active player) has changed
	for(i = 0; i < MAX_DECK; i++){
		if(i == 0 && G->discard[activePlayer][i] != cutpurse 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile not updated as expected at idx %d for player %d\n"
			"    (player who played cutpurse)\n"
			"  Expected cutpurse ; Observed %d %s\n", 
			i, activePlayer, G->discard[activePlayer][i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		else if(i != 0 && G->discard[activePlayer][i] != -1 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile not updated as expected at idx %d for player %d\n"
			"    (player who played cutpurse)\n"
			"  Expected -1 ; Observed %d %s\n", 
			i, activePlayer, G->discard[activePlayer][i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	//If this is not the no copper boundary test,
	//make sure a copper is placed into each non-active player's
	//discard pile
	for(m = 0; m < NUM_PLAYERS && !noCopper; m++){
		if(m == activePlayer){
			continue;
		}
		for(i = 0; i < MAX_DECK; i++){
			if(i == 0 && G->discard[m][i] != copper 
				&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Discard pile not updated as expected at idx %d for player %d\n"
				"    (player who did not play cutpurse)\n"
				"  Expected copper ; Observed %d %s\n", 
				i, m, G->discard[m][i],
				noCopper ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			else if(i != 0 && G->discard[m][i] != -1 
				&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Discard pile not updated as expected at idx %d for player %d\n"
				"    (player who did not play cutpurse)\n"
				"  Expected -1 ; Observed %d %s\n", 
				i, m, G->discard[m][i],
				noCopper ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			break;
		}
	}
	
	//If this is the no copper boundary test,
	//make sure each non-active player's discard pile does not change
	for(m = 0; m < NUM_PLAYERS && noCopper; m++){
		if(m == activePlayer){
			continue;
		}
		for(i = 0; i < MAX_DECK; i++){
			if(G->discard[m][i] != -1 
				&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Discard pile not updated as expected at idx %d for player %d\n"
				"    (player who did not play cutpurse)\n"
				"  Expected -1 ; Observed %d %s\n", 
				i, m, G->discard[m][i],
				noCopper ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			break;
		}
	}
	
	//Make sure other game state values haven't changed
	
	//Check numActions (numActions is updated by parent function
	//playCard, not cutpurseEffect or any function called by it)...
	if(G->numActions != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of actions not updated correctly\n"
		"  Expected 0 ; Observed %d\n %s", 
		G->numActions,
		noCopper ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check whoseTurn...
	if(G->whoseTurn != activePlayer && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Whose turn changed unexpectedly\n"
		"  Expected %d ; Observed %d %s\n", 
		activePlayer, G->whoseTurn,
		noCopper ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check numBuys...
	if(G->numBuys != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of buys changed unexpectedly\n"
		"  Expected 1 ; Observed %d %s\n", 
		G->numBuys,
		noCopper ? "(Boundary)" : "(Non-Boundary)");
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
			noCopper ? "(Boundary)" : "(Non-Boundary)");
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
		noCopper ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check outpost turn...
	if(G->outpostTurn != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Outpost turn changed unexpectedly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->outpostTurn,
		noCopper ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//NOTE: I initially wrote this test assuming the discardCard
	//		function was functioning properly (i.e. the Cutpurse
	//		was supposed to end up in playedCard after its use.
	//		Upon further investigation and consideration, I 
	//		realize that the test here should be to make sure
	//		Cutpurse gets to the proper discard pile, as this is
	//		its final destination per the game specifications.
	//		There appears to be a bug in discardCard accordingly,
	//		which I discuss in more detail in the assignment writeup.
	//		We still run this test to allow for the possibility that
	//		playedCards receives the card being played, even though
	//		the use of playedCards appears to be irrelevant.
	
	//Check playedCardCount (should be 1)
	if(G->playedCardCount != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Played card count not updated correctly\n"
		"  Expected 1 ; Observed %d %s\n", 
		G->playedCardCount,
		noCopper ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	//Check playedCards (should have Cutpurse at index 0, -1 all other indexes)
	for(i = 0; i < MAX_DECK; i++){
		if(i == 0 && G->playedCards[i] != cutpurse 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected cutpurse ; Observed %d %s\n", 
			i, G->playedCards[0],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		else if(i != 0 && G->playedCards[i] != -1 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected -1 ; Observed %d %s\n", 
			i, G->playedCards[i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	return 0;
}
	
	
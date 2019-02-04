/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *                       _cardtest4helper.c
 *
 *		            Helper function for cardtest4.c
 *
 *	      (Test for "council_room" card effect - See cardEffects.c line 5)
 *
 *            Include the following lines in your makefile:
 *
 * _cardtest4helper.o: _cardtest4helper.c _cardtest4helper.h dominion.o
 *		gcc -c _cardtest4helper.c -g  $(CFLAGS)
 *
 * ---------------------------------------------------------------------------
 */
 
#include "_cardtest4helper.h"
 
int _cardtest4helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int emptyDecks, int testNumber){
		
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
	//outpostTurn, playedCard, and playedCardCount are known values before Council_Room play
	G->whoseTurn = 0;
	G->coins = 0;
	G->numBuys = 1;
	G->numActions = 1;
	memset(G->embargoTokens, 0, sizeof(int) * treasure_map + 1);
	G->outpostPlayed = 0;
	G->outpostTurn = 0;
	memset(G->playedCards, -1, sizeof(int) * MAX_DECK);
	G->playedCardCount = 0;
	
	//Ensure supply pile counts are of known value before Council_Room play
	//Set each to 10
	for(i = 0, j = 0; i < 27; i++){
		G->supplyCount[i] = 10;
	}
	
	//Re-select random stream 2 (since initializeGame will have selected
	//stream 1 in parent function (main, see cardtest4.c)
	SelectStream(2);
	
	
	//Determine random deck size for each player in 
	//range 1 - MAX_DECK
	int deckSize[NUM_PLAYERS];
	for(i = 0; i < NUM_PLAYERS; i++){
		deckSize[i] = 1 + (Random() * (MAX_DECK - 1));
	}
	
	//Determine random hand size for each player in 
	//range 1 - MAX_HAND
	int handSize[NUM_PLAYERS];
	for(i = 0; i < NUM_PLAYERS; i++){
		handSize[i] = 1 + (Random() * (MAX_HAND - 1));
	}
	
	//Load each player's deck with an equal number of each card,
	//plus an extra starting at curse for each remainder after 
	//final multiple of 17, for deckSize as determined above 
	//(e.g. a 20 card deck will have 1 of each card plus 1 extra 
	//curse, estate, and duchy.)
	for(m = 0; m < NUM_PLAYERS; m++){
		for(i = 0, j = 0; i < deckSize[m]; i++){
			if(j < 7){
				G->deck[m][i] = j++;
			}
			else{
				G->deck[m][i] = k[j++ - 7];
			}
			if(j == 17){
				j = 0;
			}
		}
		G->deckCount[m] = deckSize[m];
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
	
	//Determine random player to play council_room
	int activePlayer = Random() * (NUM_PLAYERS - 1);
	G->whoseTurn = activePlayer;
	
	//Assign a random hand position for Council_Room for active player
	int handPos = Random() * (G->handCount[activePlayer] - 1);
	if(handPos == -1){
		handPos = 0;
	}
	G->hand[activePlayer][handPos] = council_room;
	
	//If this is the boundary test (no cards in decks),
	//clear all decks
	else{
		for(i = 0; i < NUM_PLAYERS; i++){
			memset(G->deck[i], -1, sizeof(int) * MAX_DECK);
			G->deckCount[i] = 0;
		}
	}
	
	//Shuffle each player's deck 
	//--(shuffle has been tested via unittest3.
	//-- see unittest3.c and _unittest3helper.c
	//-- for additional details.)
	for(i = 0; i < NUM_PLAYERS; i++){
		shuffle(i, G);
	}
	
	//Store deck and hand info prior to adventurer call
	
	//For deck...
	int deckCardCountByTypeBeforeAdventurer[NUM_PLAYERS][27] = {{0}};
	int deckCountBeforeAdventurer[NUM_PLAYERS];						//Just to use different variable name, could use deckSize
	int deckBeforeAdventurer[NUM_PLAYERS][MAX_DECK];
	for(i = 0; i < NUM_PLAYERS; i++){
		for(j = 0; j < G->deckCount[j]; i++){
			deckBeforeAdventurer[i] = G->deck[i][j];
			deckCardCountByTypeBeforeAdventurer[G->deck[i][j]]++;
		}
		deckCountBeforeAdventurer[i] = G->deckCount[i];
	}
	
	//For hand...
	int handCardCountByTypeBeforeAdventurer[NUM_PLAYERS][27] = {{0}};
	int handCountBeforeAdventurer[NUM_PLAYERS];						//Just to use different variable name, could use handSize
	//int handBeforeAdventurer[MAX_DECK];							//Not needed since order of hand doesn't matter
	for(i = 0; i < NUM_PLAYERS; i++){
		for(j = 0; j < G->handCount[j]; i++){
			//handBeforeAdventurer[i] = G->hand[i][j];
			handCardCountByTypeBeforeAdventurer[G->hand[i][j]]++;
		}
		handCountBeforeAdventurer[i] = G->handCount[i];
	}
	
	//int for coin bonus (unused by Council_Room call)
	int coin_bonus = 0;
	
	/*************************  SET UP ENDS HERE  **************************/
	
	/************************* CHECKS BEGIN HERE  **************************/
	
	// $$$$$$ LEFT OFF HERE  $$$$$$$
	
	//Call Council_Room
	cardEffect(council_room, -1, -1, -1, G, handPos, &coin_bonus);
	
	//Store hand card counts after council_room call for each player
	int handCardCountByTypeAfterCouncil_Room[MAX_PLAYERS][27] = {{0}};
	for(i = 0; i < NUM_PLAYERS; i++){
		for(j = 0; j < G->handCount[i]; j++){
			handCardCountByTypeAfterCouncil_Room[i][G->hand[i][j]]++;
		}
	}
	
	//Determine how many of each card type have been removed from (or 
	//erroneously added to) the deck
	int deckCardCountByTypeAfterAdventurer[NUM_PLAYERS][27] = {0};
	int deckDiffsAfterAdventurer[NUM_PLAYERS][27] = {0};
	for(m = 0; m < NUM_PLAYERS; m++){
		for(i = 0; i < G->deckCount[m]; i++){
			deckCardCountByTypeAfterAdventurer[m][G->deck[m][i]]++;
		}
		for(i = 0; i < 27; i++){
			deckDiffsAfterAdventurer[m][i] =
				deckCardCountByTypeBeforeAdventurer[m][i] -
				deckCardCountByTypeAfterAdventurer[m][i];
		}
	}
	
	//Determine how many of each card type have been removed from (or 
	//erroneously added to) the hand
	int handCardCountByTypeAfterAdventurer[NUM_PLAYERS][27] = {0};
	int handDiffsAfterAdventurer[NUM_PLAYERS][27] = {0};
	for(m = 0; m < NUM_PLAYERS; m++){
		for(i = 0; i < G->handCount[m]; i++){
			handCardCountByTypeAfterAdventurer[m][G->hand[m][i]]++;
		}
		for(i = 0; i < 27; i++){
			handDiffsAfterAdventurer[m][i] =
				handCardCountByTypeBeforeAdventurer[m][i] -
				handCardCountByTypeAfterAdventurer[m][i];
		}
	}
	
	//If this is not the empty decks boundary test,
	//check to make sure each player's hand contains the same
	//cards as before the council_room play plus one (and only one)
	//card for all non-active players. For active player, make
	//sure hand is the same as before the council_room play plus 4
	//new cards.
	for(int i; i < NUM_PLAYERS && !emptyDecks; i++){
		for(j = 0; j < 27; j++){
			if( i != activePlayer && 
			  ((j == copper && handCardCountByTypeBeforeCouncil_Room[i][j] - 1 != 
			    handCardCountByTypeAfterCouncil_Room[i][j]) ||
			   ((j < copper || j > copper) && 
			    handCardCountByTypeBeforeCouncil_Room[i][j] !=
			    handCardCountByTypeAfterCouncil_Room[i][j])) &&
				++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Hand cards not updated properly after Council_Room play\n"
				"  Incorrect number of %d cards for player %d\n"
				"   (player who did not play council_room)\n"
				"  Expected: %d ; Observed %d %s\n", 
				j, i, 
				j == copper ? handCardCountByTypeBeforeCouncil_Room[i][j] - 1 :
				handCardCountByTypeBeforeCouncil_Room[i][j],
				handCardCountByTypeAfterCouncil_Room[i][j],
				noCopper ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			else if( i == activePlayer && 
				  ((j == council_room && handCardCountByTypeBeforeCouncil_Room[i][j] - 1 != 
					handCardCountByTypeAfterCouncil_Room[i][j]) ||
				   ((j < council_room || j > council_room) && 
					handCardCountByTypeBeforeCouncil_Room[i][j] !=
					handCardCountByTypeAfterCouncil_Room[i][j])) &&
					++(*failCt) <= MAX_FAILS){
					failures[*failCt-1].lineNumber = __LINE__;
					sprintf(failures[*failCt-1].description,
					"Hand cards not updated properly after Council_Room play\n"
					"  Incorrect number of %d cards for player %d\n"
					"    (player who played council_room)\n"
					"  Expected: %d ; Observed %d %s\n", 
					j, i, 
					j == council_room ? handCardCountByTypeBeforeCouncil_Room[i][j] - 1 :
					handCardCountByTypeBeforeCouncil_Room[i][j],
					handCardCountByTypeAfterCouncil_Room[i][j],
					noCopper ? "(Boundary)" : "(Non-Boundary)");
					failures[*failCt-1].testNumber = testNumber;
			}
		}
	}
	
	//If this is not the no coppers boundary test,
	//Check to make sure all hand counts are decremented by one.
	//(Non-active players discard 1 copper, active player discards
	// 1 council_room.)
	for(int i; i < NUM_PLAYERS && !noCopper; i++){
		if(handSize[i] - 1 != G->handCount[i] &&
			++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Hand counts not updated properly after Council_Room play\n"
			"  Incorrect number of %d cards for player %d\n"
			"    (player who %s council_room)\n"
			"  Expected: %d ; Observed %d %s\n", 
			j, i,
			i == activePlayer ? "played" : "did not play",
			handSize[i] - 1, G->handCount[i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//If this is the no coppers boundary test,
	//check to make sure each player's hand contains the same
	//cards as before the council_room play. For active player, make
	//sure hand is the same as before the council_room play minus one 
	//council_room only.
	for(int i; i < NUM_PLAYERS && noCopper; i++){
		for(j = 0; j < 27; j++){
			if(i != activePlayer && 
			   handCardCountByTypeBeforeCouncil_Room[i][j] != 
			   handCardCountByTypeAfterCouncil_Room[i][j] && 
			   ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Hand cards not updated properly after Council_Room play\n"
				"  Incorrect number of %d cards for player %d\n"
				"   (player who did not play council_room)\n"
				"  Expected: %d ; Observed %d %s\n", 
				j, i, handCardCountByTypeBeforeCouncil_Room[i][j],
				handCardCountByTypeAfterCouncil_Room[i][j],
				noCopper ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			else if( i == activePlayer && 
				   ((j == council_room && handCardCountByTypeBeforeCouncil_Room[i][j] - 1 != 
				     handCardCountByTypeAfterCouncil_Room[i][j]) ||
				   ((j < council_room || j > council_room) && handCardCountByTypeBeforeCouncil_Room[i][j] !=
					handCardCountByTypeAfterCouncil_Room[i][j])) &&
					++(*failCt) <= MAX_FAILS){
					failures[*failCt-1].lineNumber = __LINE__;
					sprintf(failures[*failCt-1].description,
					"Hand cards not updated properly after Council_Room play\n"
					"  Incorrect number of %d cards for player %d\n"
					"    (player who played council_room)\n"
					"  Expected: %d ; Observed %d %s\n", 
					j, i, 
					j == council_room ? handCardCountByTypeBeforeCouncil_Room[i][j] - 1 :
					handCardCountByTypeBeforeCouncil_Room[i][j],
					handCardCountByTypeAfterCouncil_Room[i][j],
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
			"    (player who %s council_room)\n"
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
				"    (player who %s council_room)\n"
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
	
	//Check coins (expectedCoins + 2)...
	if(G->coins != expectedCoins + 2 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"coins value not updated correctly\n"
		"  Expected 2 ; Observed %d %s\n", 
		G->coins,
		noCopper ? "(Boundary)" : "(Non-Boundary)");
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
	//discards council_room and inactive players discard copper...
	for(i = 0; i < NUM_PLAYERS && !noCopper; i++){
		if(G->discardCount[i] != 1 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard card count not updated correctly for player %d\n"
			"    (player who %s council_room)\n"
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
	//for all other players, as the active player discards council_room
	//and no other players discard
	for(i = 0; i < NUM_PLAYERS && noCopper; i++){
		if(i == activePlayer && G->discardCount[i] != 1 && 
			++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard card count not updated correctly\n"
			"    (player who played council_room)\n"
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
			"    (player who did not play council_room)\n"
			"  Expected 0 ; Observed %d %s\n", 
			G->discardCount[i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//Check that council_room is placed in the active player's discard pile,
	//and that no other discard pile index value (for active player) has changed
	for(i = 0; i < MAX_DECK; i++){
		if(i == 0 && G->discard[activePlayer][i] != council_room 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile not updated as expected at idx %d for player %d\n"
			"    (player who played council_room)\n"
			"  Expected council_room ; Observed %d %s\n", 
			i, activePlayer, G->discard[activePlayer][i],
			noCopper ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		else if(i != 0 && G->discard[activePlayer][i] != -1 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile not updated as expected at idx %d for player %d\n"
			"    (player who played council_room)\n"
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
		for(i = 0; i < MAX_DECK; i++){
			if(i == 0 && G->discard[m][i] != copper 
				&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Discard pile not updated as expected at idx %d for player %d\n"
				"    (player who did not play council_room)\n"
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
				"    (player who did not play council_room)\n"
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
		for(i = 0; i < MAX_DECK; i++){
			if(G->discard[m][i] != -1 
				&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Discard pile not updated as expected at idx %d for player %d\n"
				"    (player who did not play council_room)\n"
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
	//playCard, not council_roomEffect or any function called by it)...
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
	//		function was functioning properly (i.e. the Smithy
	//		was supposed to end up in playedCard after its use.
	//		Upon further investigation and consideration, I 
	//		realize that the test here should be to make sure
	//		Smithy gets to the proper discard pile, as this is
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
		"  Expected 1 ; Observed %d\n", 
		G->playedCardCount);
		failures[*failCt-1].testNumber = testNumber;
	}
	//Check playedCards (should have Smithy at index 0, -1 all other indexes)
	for(i = 0; i < MAX_DECK; i++){
		if(i == 0 && G->playedCards[i] != council_room 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected council_room ; Observed %d\n", 
			i, G->playedCards[0]);
			failures[*failCt-1].testNumber = testNumber;
		}
		else if(i != 0 && G->playedCards[i] != -1 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected -1 ; Observed %d\n", 
			i, G->playedCards[i]);
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	return 0;
}
	
	
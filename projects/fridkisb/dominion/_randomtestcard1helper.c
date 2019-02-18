/* ------------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 4
 *
 *                       _randomtestcard1helper.c
 *
 *		            Helper function for randomtestcard1.c
 *
 *	      (Test for "council_room" card effect - See cardEffects.c line 5)
 *
 *            Include the following lines in your makefile:
 *
 * _randomtestcard1helper.o: _randomtestcard1helper.c _randomtestcard1helper.h \
 *							 dominion.o
 *		gcc -c _randomtestcard1helper.c -g  $(CFLAGS)
 *
 * ------------------------------------------------------------------------------
 */
 
#include "_randomtestcard1helper.h"
 
int _randomtestcard1helper(int numPlayers, int k[], struct gameState* G, 
	failedTest failures[], int* failCt, int emptyDecks, int testNumber){
		
	//Test value variables	   
	int i, j, m;
	
	//Re-select random stream 2 (since initializeGame will have selected
	//stream 1 in parent function (main, see randomtestcard1.c)
	SelectStream(2);
	
	//Randomize gamestate struct
	for(i = 0; i < sizeof(struct gameState); i++){
		((char*)G)[i] = floor(Random() * 256);
	}
	
	//Declare hand/deck count holders for each player
	int handCountBeforeCouncil_Room[numPlayers],
		deckCountBeforeCouncil_Room[numPlayers];
	
	//Ensure discardCount, deckCount, and handCount are all set to 0
	//and discard, deck, and hand are cleared for all players
	for(i = 0; i < numPlayers; i++){
		G->discardCount[i] = 0;
		G->deckCount[i] = 0;
		deckCountBeforeCouncil_Room[i] = 0;
		G->handCount[i] = 0;
		handCountBeforeCouncil_Room[i] = 0;
		memset(G->discard[i], -1, sizeof(int) * MAX_DECK);
		memset(G->deck[i], -1, sizeof(int) * MAX_DECK);
		memset(G->hand[i], -1, sizeof(int) * MAX_HAND);
	}
	
	//Ensure whoseTurn, coins, numBuys, numActions,
	//playedCard, and playedCardCount are known values 
	//before Council_Room play
	G->whoseTurn = 0;
	G->coins = 0;
	G->numBuys = 1;
	G->numActions = 1;
	G->numPlayers = numPlayers;
	memset(G->playedCards, -1, sizeof(int) * MAX_DECK);
	G->playedCardCount = 0;
	//If not randomizing, ensure known values of embargoTokens[],
	//outpostPlayed, and outpostTurn as well.
	if(!RANDOMIZE){
		memset(G->embargoTokens, 0, sizeof(int) * treasure_map + 1);
		G->outpostPlayed = 0;
		G->outpostTurn = 0;
	}
	
	//Store embargo token array, outpostPlayed, and outpostTurn
	//values prior to council_room play
	int embargoTokensBeforeCouncil_Room[27];
	for(i = 0; i < 27; i++){
		embargoTokensBeforeCouncil_Room[i] = G->embargoTokens[i];
	}
	int outpostPlayedBeforeCouncil_Room = G->outpostPlayed;
	int outpostTurnBeforeCouncil_Room = G->outpostTurn;
	
	//Ensure supply pile counts are of known value before Council_Room play
	//Set each to 10
	for(i = 0; i < 27; i++){
		G->supplyCount[i] = 10;
	}
	
	//Determine active player, randomly if indicated
	int activePlayer = 0;
	if(RANDOMIZE){
		//Determine random player to play council_room
		activePlayer = floor(Random() * numPlayers);
		G->whoseTurn = activePlayer;
	}
	else{
		G->whoseTurn = 0;
	}
	
	//If this is not the emptyDecks boundary test
	if(!emptyDecks){	
		if(RANDOMIZE){
			//Determine random deck size for each player in 
			//range 1 - MAX_DECK
			for(i = 0; i < numPlayers; i++){
				deckCountBeforeCouncil_Room[i] = 1 + floor(Random() * MAX_DECK);
			}
		
			//Ensure active player has at least 4 cards to draw,
			//if this is not the emptyDecks boundary test
			if(deckCountBeforeCouncil_Room[activePlayer] < 4){
				deckCountBeforeCouncil_Room[activePlayer] = 4;
			}
		}
		else{
			for(i = 0; i < numPlayers; i++){
				deckCountBeforeCouncil_Room[i] = testNumber * 5;
				if(deckCountBeforeCouncil_Room[i] >= MAX_DECK){
					deckCountBeforeCouncil_Room[i] = 
						(deckCountBeforeCouncil_Room[i] % MAX_DECK) + 1;
					if(i == activePlayer && deckCountBeforeCouncil_Room[i] < 4){
						deckCountBeforeCouncil_Room[i] = 4;
					}
				}
			}
		}
	}
	
	if(RANDOMIZE){
		//Determine random hand size for each player in 
		//range 1 - MAX_HAND
		for(i = 0; i < numPlayers; i++){
			handCountBeforeCouncil_Room[i] = 1 + floor(Random() * MAX_HAND);
			
			//Without the following if statement, hand can potentially 
			//exceed MAX_HAND, which will result in hand count and position
			//failures after call to council_roomEffect! See assignment4
			//writeup for additional discussion on this matter.
			
			/* if(handCountBeforeCouncil_Room[i] > MAX_HAND - 4){
				handCountBeforeCouncil_Room[i] = MAX_HAND - 4;
			} */
		}
	}
	else{
		for(i = 0; i < numPlayers; i++){
			handCountBeforeCouncil_Room[i] = (testNumber * 5) + 1;
			if(handCountBeforeCouncil_Room[i] >= MAX_HAND - 4){
				handCountBeforeCouncil_Room[i] = 
				(handCountBeforeCouncil_Room[i] % MAX_HAND) + 1;
			}
		}
	}
	
	if(RANDOMIZE){
		//Randomly load each player's deck according to randomly
		//chosen deck size
		for(m = 0; !emptyDecks && m < numPlayers; m++){
			for(i = 0; i < deckCountBeforeCouncil_Room[m]; i++){
				int randomCard = floor(Random() * 27);
				//If the random card choice is a kingdom card,
				//randomly assign a kingdom card that is in play
				//(Note this value may or may not be the same as
				// as the randomly determined card number above.
				// This is done to ensure only cards in play are 
				// used.)
				if(randomCard >= adventurer){
					G->deck[m][i] = k[(int)floor(Random() * 10)];
				}
				else{
					G->deck[m][i] = randomCard;
				}
			}
			G->deckCount[m] = deckCountBeforeCouncil_Room[m];
		}
	}
	else{
		//Load each player's deck with an equal number of each card,
		//plus an extra starting at curse for each remainder after 
		//final multiple of 17, for deckCountBeforeCouncil_Room as determined above 
		//(e.g. a 20 card deck will have 1 of each card plus 1 extra 
		//curse, estate, and duchy.)
		for(m = 0; !emptyDecks && m < numPlayers; m++){
			for(i = 0, j = 0; i < deckCountBeforeCouncil_Room[m]; i++){
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
			G->deckCount[m] = deckCountBeforeCouncil_Room[m];
		}
	}
	
	if(RANDOMIZE){
		//Randomly load each player's hand according to randomly
		//chosen hand size
		for(m = 0; m < numPlayers; m++){
			for(i = 0; i < handCountBeforeCouncil_Room[m]; i++){
				int randomCard = floor(Random() * 27);
				//If the random card choice is a kingdom card,
				//randomly assign a kingdom card that is in play
				//(Note this value may or may not be the same as
				// as the randomly determined card number above.
				// This is done to ensure only cards in play are 
				// used.)
				if(randomCard >= adventurer){
					G->hand[m][i] = k[(int)floor(Random() * 10)];
				}
				else{
					G->hand[m][i] = randomCard;
				}
			}
			G->handCount[m] = handCountBeforeCouncil_Room[m];
		}
	}
	else{
		//Load each player's hand with an equal number of each card,
		//plus an extra starting at curse for each remainder after 
		//final multiple of 17, for handCountBeforeCouncil_Room as 
		//determined above (e.g. a 20 card deck will have 1 of each 
		//card plus 1 extra curse, estate, and duchy.)
		for(m = 0; m < numPlayers; m++){
			for(i = 0, j = 0; i < handCountBeforeCouncil_Room[m]; i++){
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
			G->handCount[m] = handCountBeforeCouncil_Room[m];
		}
	}
	int handPos;
	if(RANDOMIZE){
		//Assign a random hand position for Council_Room for active player
		handPos = floor(Random() * G->handCount[activePlayer]);
		if(handPos == -1){
			handPos = 0;
		}
	}
	else{
		handPos = 0;
	}
	G->hand[activePlayer][handPos] = council_room;
	//Shuffle each player's deck 
	//--(shuffle has been tested via unittest3. 
	//-- see unittest3.c and _unittest3helper.c (of Assignment 3)
	//-- for additional details.)
	//-- (Deterministic if RANDOMIZE is disabled
	//-- as seed for initializeGame will remain constant
	//-- with each run.)
	for(i = 0; i < numPlayers; i++){
		shuffle(i, G);
	}
	
	//Store deck and hand info prior to adventurer call
	
	//For deck...
	int deckCardCountByTypeBeforeCouncil_Room[numPlayers][27];
	memset(deckCardCountByTypeBeforeCouncil_Room, 
				0, sizeof(int) * numPlayers * 27);
	int deckBeforeCouncil_Room[numPlayers][MAX_DECK];
	for(i = 0; i < numPlayers; i++){
		for(j = 0; j < G->deckCount[i]; j++){
			deckBeforeCouncil_Room[i][j] = G->deck[i][j];
			deckCardCountByTypeBeforeCouncil_Room[i][G->deck[i][j]]++;
		}
	}
	
	//For hand...
	int handCardCountByTypeBeforeCouncil_Room[numPlayers][27];
	memset(handCardCountByTypeBeforeCouncil_Room, 
				0, sizeof(int) * numPlayers * 27);
	//int handBeforeCouncil_Room[numPlayers][MAX_DECK];				//Not needed since order of hand doesn't matter
	for(i = 0; i < numPlayers; i++){
		for(j = 0; j < G->handCount[i]; j++){
			//handBeforeCouncil_Room[i][j] = G->hand[i][j];
			handCardCountByTypeBeforeCouncil_Room[i][G->hand[i][j]]++;
		}
	}
		
	//int for coin bonus (unused by Council_Room call)
	int coin_bonus = 0;
	
	/*************************  SET UP ENDS HERE  **************************/
	
	/************************* CHECKS BEGIN HERE  **************************/
	
	// $$$$$$ LEFT OFF HERE  $$$$$$$
	
	//Call Council_Room
	cardEffect(council_room, -1, -1, -1, G, handPos, &coin_bonus);
	
	//Determine how many of each card type have been removed from (or 
	//erroneously added to) the deck
	int deckCardCountByTypeAfterCouncil_Room[numPlayers][27];
	memset(deckCardCountByTypeAfterCouncil_Room, 
				0, sizeof(int) * numPlayers * 27);
	//int deckDiffsAfterCouncil_Room[numPlayers][27] = {{0}};			//Not used, but may come in handy later...
	for(m = 0; m < numPlayers; m++){
		for(i = 0; i < G->deckCount[m]; i++){
			deckCardCountByTypeAfterCouncil_Room[m][G->deck[m][i]]++;
		}
		/* for(i = 0; i < 27; i++){
			deckDiffsAfterCouncil_Room[m][i] =
				deckCardCountByTypeBeforeCouncil_Room[m][i] -
				deckCardCountByTypeAfterCouncil_Room[m][i];
		} */
	}
	
	//Determine how many of each card type have been added to (or 
	//erroneously removed from) the hand
	int handCardCountByTypeAfterCouncil_Room[numPlayers][27];
	memset(handCardCountByTypeAfterCouncil_Room, 
				0, sizeof(int) * numPlayers * 27);
	int handDiffsAfterCouncil_Room[numPlayers][27];
	memset(handDiffsAfterCouncil_Room, 0, sizeof(int) * numPlayers * 27);
	for(m = 0; m < numPlayers; m++){
		for(i = 0; i < G->handCount[m]; i++){
			handCardCountByTypeAfterCouncil_Room[m][G->hand[m][i]]++;
		}
		for(i = 0; i < 27; i++){
			handDiffsAfterCouncil_Room[m][i] =
				handCardCountByTypeAfterCouncil_Room[m][i] -
				handCardCountByTypeBeforeCouncil_Room[m][i];
		}
	}
	
	//If this is not the empty decks boundary test,
	//check to make sure each player's hand contains the same
	//cards as before the council_room play plus one (and only one)
	//card for all non-active players. For active player, make
	//sure hand is the same as before the council_room play plus 3
	//new cards (4 are gained from deck but 1 council_room should be
	//discarded from hand, net gain of 3).
	for(i = 0; i < numPlayers && !emptyDecks; i++){
		//Check deck counts...
		if(((i != activePlayer && 
		   deckCountBeforeCouncil_Room[i] - 1 != G->deckCount[i]) ||
			(i == activePlayer && deckCountBeforeCouncil_Room[i] - 4 !=
			 G->deckCount[i])) && ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Deck count not updated properly after Council_Room play\n"
				"  Expected %d ; Observed %d %s\n"
				"   (player %d, who %s council_room)\n"
				" Aborting remaining deck vs. deck comparison checks for player %d...\n",
				i != activePlayer ? deckCountBeforeCouncil_Room[i] - 1 :
				deckCountBeforeCouncil_Room[i] - 4, 
				G->deckCount[i],
				emptyDecks ? "(Boundary)" : "(Non-Boundary)",
				i, i != activePlayer ? "did not play" : "played",
				i);
				failures[*failCt-1].testNumber = testNumber;
				continue;
		}
		//Check hand counts 
		//(Active player should have a net gain of only 3 cards to the hand,
		// since 4 cards are gained from the deck but 1 council_room is discarded
		// from the hand.)
		if(((i != activePlayer && 
		   handCountBeforeCouncil_Room[i] + 1 != G->handCount[i]) ||
			(i == activePlayer && handCountBeforeCouncil_Room[i] + 3 !=
			 G->handCount[i])) && ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Hand count not updated properly after Council_Room play\n"
				"  Expected %d ; Observed %d %s\n"
				"   (player %d, who %s council_room)\n"
				" Aborting remaining deck vs. hand comparison checks for player %d...\n",
				i != activePlayer ? handCountBeforeCouncil_Room[i] + 1 :
				handCountBeforeCouncil_Room[i] + 3, 
				G->handCount[i],
				emptyDecks ? "(Boundary)" : "(Non-Boundary)",
				i, i != activePlayer ? "did not play" : "played",
				i);
				failures[*failCt-1].testNumber = testNumber;
				continue;
		}
		
		//Make sure cards added to hand came from top of deck
		if(i == activePlayer){
			for(j = deckCountBeforeCouncil_Room[i] - 1, m = 0; 
				j > deckCountBeforeCouncil_Room[i] - 5; j--, m++){
				if(((j != deckCountBeforeCouncil_Room[i] - 4 &&
					deckBeforeCouncil_Room[i][j] != 
					G->hand[i][handCountBeforeCouncil_Room[i] + m]) ||
				   (j == deckCountBeforeCouncil_Room[i] - 4 && 
				    deckBeforeCouncil_Room[i][j] != G->hand[i][handPos])) &&
					++(*failCt) <= MAX_FAILS){
						failures[*failCt-1].lineNumber = __LINE__;
						sprintf(failures[*failCt-1].description,
						"Cards not gained from top of deck to top of hand %s\n"
						"   (player %d, who played council_room)\n",
						emptyDecks ? "(Boundary)" : "(Non-Boundary)", i);
						failures[*failCt-1].testNumber = testNumber;
						break;
				}
			}
		}
		else{
			if(deckBeforeCouncil_Room[i][deckCountBeforeCouncil_Room[i] - 1] != 
				G->hand[i][handCountBeforeCouncil_Room[i]] &&
				++(*failCt) <= MAX_FAILS){
					failures[*failCt-1].lineNumber = __LINE__;
					sprintf(failures[*failCt-1].description,
					"Cards not gained from top of deck to top of hand %s\n"
					"   (player %d, who did not play council_room)\n",
					emptyDecks ? "(Boundary)" : "(Non-Boundary)", i);
					failures[*failCt-1].testNumber = testNumber;
					break;
			}
		}
		
		//Check to make sure that the deck and hand are unchanged
		//for indexes not affected by council_room play (i.e. below
		//cards drawn from the deck and below those gained to the hand
		if(i == activePlayer){
			for(j = 0; j < deckCountBeforeCouncil_Room[i] - 4; j++){
				if(deckBeforeCouncil_Room[i][j] != G->deck[i][j] &&
				++(*failCt) <= MAX_FAILS){
						failures[*failCt-1].lineNumber = __LINE__;
						sprintf(failures[*failCt-1].description,
						"Deck below drawn cards changed unexpectedly\n"
						"   (player %d, who played council_room)\n"
						"  Expected %d at idx %d ; Observed %d %s\n",
						i, deckBeforeCouncil_Room[i][j], j, G->deck[i][j],
						emptyDecks ? "(Boundary)" : "(Non-Boundary)");
						failures[*failCt-1].testNumber = testNumber;
						break;
				}
			}
		}
		else{
			for(j = 0; j < deckCountBeforeCouncil_Room[i] - 1; j++){
				if(deckBeforeCouncil_Room[i][j] != G->deck[i][j] &&
				++(*failCt) <= MAX_FAILS){
						failures[*failCt-1].lineNumber = __LINE__;
						sprintf(failures[*failCt-1].description,
						"Deck below drawn cards changed unexpectedly\n"
						"   (player %d, who did not play council_room)\n"
						"  Expected %d at idx %d ; Observed %d %s\n",
						i, deckBeforeCouncil_Room[i][j], j, G->deck[i][j],
						emptyDecks ? "(Boundary)" : "(Non-Boundary)");
						failures[*failCt-1].testNumber = testNumber;
						break;
				}
			}
		}
		
		/* Redundant, in that everything above effectively covers the same check */
		
		/*
		for(j = 0; j < 27; j++){
			int deckVsHandDiffs = handDiffsAfterCouncil_Room[i][j] - 
				deckDiffsAfterCouncil_Room[i][j];
			//If active player gains a council_room, the card type diff total
		    //is only expected to be 1, (council_room removed from deck and
			//gained to hand but also discard from hand) but only in this case...
			if((i == activePlayer && deckVsHandDiffs == 1 &&
			   j != council_room) || deckVsHandDiffs != 0 &&
				++(*failCt) <= MAX_FAILS){
					failures[*failCt-1].lineNumber = __LINE__;
					sprintf(failures[*failCt-1].description,
					"Cards gained to hand are not the same as\n"
					"  those removed from deck after Council_Room play\n"
					"  Gained %d %ds to hand, but removed %d %ds from deck\n"
					"   (player %d, who %s council_room)\n",
					 :
					handCountBeforeCouncil_Room[i] + 3, 
					i, G->handCount[i],
					emptyDecks ? "(Boundary)" : "(Non-Boundary)"),
					i != activePlayer ? "did not play" : "played";
					failures[*failCt-1].testNumber = testNumber;
			}
		}
		*/
	}
	
	//Make sure numBuys is incremented by 1
	if(G->numBuys != 2 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of buys not updated correctly\n"
		"  Expected 2 ; Observed %d %s\n", 
		G->numBuys,
		emptyDecks ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	// EMPTY DECKS BOUNDARY CHECKS ...
	for(i = 0; i < numPlayers && emptyDecks; i++){
		
		//For emptyDecks boundary test, make sure no decks change
		if(G->deckCount[i] != 0 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Deck count changed unexpectedly (empty deck check)\n"
			"  Expected 0 : Observed %d %s\n"
			"   (player %d, who %s council_room)\n",
			G->deckCount[i],
			emptyDecks ? "(Boundary)" : "(Non-Boundary)",
			i, i != activePlayer ? "did not play" : "played");
			failures[*failCt-1].testNumber = testNumber;
		}
		
		//Make sure deck and hand haven't changed, except for the
		//active player's hand, which should be less 1 council_room
		//(order matters for deck, but not for hand)
		for(j = 0; j < MAX_DECK; j++){
			if(G->deck[i][j] != -1 && ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Deck changed unexpectedly (empty deck check)\n"
				"  Expected -1 at idx %d : Observed %d %s\n"
				"   (player %d, who %s council_room)\n",
				j, G->deck[i][j],
				emptyDecks ? "(Boundary)" : "(Non-Boundary)",
				i, i != activePlayer ? "did not play" : "played");
				failures[*failCt-1].testNumber = testNumber;
			}
		}
		for(j = 0; j < 27; j++){
			if(((i == activePlayer && j == council_room &&
			   handDiffsAfterCouncil_Room[i][council_room] != -1) ||
			  (i == activePlayer && j != council_room &&
			   handDiffsAfterCouncil_Room[i][j] != 0) ||
			  (i != activePlayer && 
			   handDiffsAfterCouncil_Room[i][j] != 0)) &&
			   ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Hand changed unexpectedly (empty deck check)\n"
				"  Gained %d %ds : Expected gain is %d %s\n"
				"   (player %d, who %s council_room)\n"
				"   (negative value indicates removal from hand)\n",
				handDiffsAfterCouncil_Room[i][j], j,
				i == activePlayer && j == council_room ? -1 : 0,
				emptyDecks ? "(Boundary)" : "(Non-Boundary)",
				i, i != activePlayer ? "did not play" : "played");
				failures[*failCt-1].testNumber = testNumber;
			}
		}
	}
		
	//Make sure supply piles haven't changed
	for(i = 0; i < 27; i++){
		if(G->supplyCount[i] != 10 && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Supply pile count changed unexpectedly at pile %d\n"
			"  Expected 10 ; Observed %d %s\n", 
			i, G->supplyCount[i],
			emptyDecks ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
			break;
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
			emptyDecks ? "(Boundary)" : "(Non-Boundary)");
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
			emptyDecks ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	//Make sure each non-active player's discard pile does not change
	for(m = 0; m < numPlayers && emptyDecks; m++){
		if(m == activePlayer){
			continue;
		}
		for(i = 0; i < MAX_DECK; i++){
			if(G->discard[m][i] != -1 
				&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Discard pile not updated as expected at idx %d for player %d\n"
				"    (player who did not play council_room)\n"
				"  Expected -1 ; Observed %d %s\n", 
				i, m, G->discard[m][i],
				emptyDecks ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			break;
		}
	}
	
	//Make sure other game state values haven't changed
	
	//Make sure coins hasn't changed
	if(G->coins != 0 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"coins value not updated correctly\n"
		"  Expected 0 ; Observed %d %s\n", 
		G->coins,
		emptyDecks ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check numActions (numActions is updated by parent function
	//playCard, not council_roomEffect or any function called by it)...
	if(G->numActions != 1 && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Number of actions not updated correctly\n"
		"  Expected 0 ; Observed %d\n %s", 
		G->numActions,
		emptyDecks ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check whoseTurn...
	if(G->whoseTurn != activePlayer && ++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Whose turn changed unexpectedly\n"
		"  Expected %d ; Observed %d %s\n", 
		activePlayer, G->whoseTurn,
		emptyDecks ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check embargo tokens...
	for(i = 0; i < treasure_map + 1; i++){
		if(G->embargoTokens[i] != embargoTokensBeforeCouncil_Room[i]
			&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Number of embargo tokens changed unexpectedly for card %d\n"
				"  Expected 0 ; Observed %d %s\n", 
				i, G->embargoTokens[i],
				emptyDecks ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//Check outpost played...
	if(G->outpostPlayed != outpostPlayedBeforeCouncil_Room
		&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Outpost played changed unexpectedly\n"
			"  Expected 0 ; Observed %d %s\n", 
			G->outpostPlayed,
			emptyDecks ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check outpost turn...
	if(G->outpostTurn != outpostTurnBeforeCouncil_Room
		&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Outpost turn changed unexpectedly\n"
			"  Expected 0 ; Observed %d %s\n", 
			G->outpostTurn,
			emptyDecks ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//NOTE: I initially wrote this test assuming the discardCard
	//		function was functioning properly (i.e. the Council_Room
	//		was supposed to end up in playedCard after its use.
	//		Upon further investigation and consideration, I 
	//		realize that the test here should be to make sure
	//		Council_Room gets to the proper discard pile, as this is
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
		emptyDecks ? "(Boundary)" : "(Non-Boundary)");
		failures[*failCt-1].testNumber = testNumber;
	}
	//Check playedCards (should have Council_Room at index 0, -1 all other indexes)
	for(i = 0; i < MAX_DECK; i++){
		if(i == 0 && G->playedCards[i] != council_room 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected council_room ; Observed %d %s\n", 
			i, G->playedCards[0],
			emptyDecks ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		else if(i != 0 && G->playedCards[i] != -1 
			&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Played cards not updated as expected at idx %d\n"
			"  Expected -1 ; Observed %d %s\n", 
			i, G->playedCards[i],
			emptyDecks ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
		}
		break;
	}
	
	return 0;
}
	
	
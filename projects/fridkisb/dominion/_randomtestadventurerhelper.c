/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 4
 *
 *                       _randomtestadventurerhelper.c
 *
 *		            Helper function for randomtestadventurer.c
 *
 *	      (Test for "adventurer" card effect - See cardEffects.c line 5)
 *
 *            Include the following lines in your makefile:
 *
 * _randomtestadventurerhelper.o: _randomtestadventurerhelper.c \
 *								  _randomtestadventurerhelper.h dominion.o
 *		gcc -c _randomtestadventurerhelper.c -g  $(CFLAGS)
 * ---------------------------------------------------------------------------
 */
 
#include "_randomtestadventurerhelper.h"
 
int _randomtestadventurerhelper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int treasureCardCountSpecifier, int isBoundary, int testNumber){
		
	//Test value variables	   
	int i, j;
	
	//Randomize gamestate struct
	for(i = 0; i < sizeof(struct gameState); i++){
		((char*)G)[i] = floor(Random() * 256);
	}
	
	//Determine random number of players
	int numPlayers = floor(Random() * NUM_PLAYERS) + 1;
	
	//Deck count and hand count storage
	int deckCountBeforeAdventurer[numPlayers], 
		handCountBeforeAdventurer[numPlayers];
	
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
	
	//Ensure discardCount, deckCount, and handCount are all set to 0
	//and discard, deck, and hand are cleared for all players
	for(i = 0; i < numPlayers; i++){
		G->discardCount[i] = 0;
		G->deckCount[i] = 0;
		deckCountBeforeAdventurer[i] = 0;
		G->handCount[i] = 0;
		handCountBeforeAdventurer[i] = 0;
		memset(G->discard[i], -1, sizeof(int) * MAX_DECK);
		memset(G->deck[i], -1, sizeof(int) * MAX_DECK);
		memset(G->hand[i], -1, sizeof(int) * MAX_HAND);
	}
	
	//Ensure coins, numBuys, numActions, playedCard, 
	//and playedCardCount are known values before 
	//Adventurer play
	G->coins = 0;
	G->numBuys = 1;
	G->numActions = 1;
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
	int embargoTokensBeforeAdventurer[27];
	for(i = 0; i < 27; i++){
		embargoTokensBeforeAdventurer[i] = G->embargoTokens[i];
	}
	int outpostPlayedBeforeAdventurer = G->outpostPlayed;
	int outpostTurnBeforeAdventurer = G->outpostTurn;
	
	//Ensure supply pile counts are of known value before Adventurer play
	//Set each to 10
	for(i = 0; i < 27; i++){
		G->supplyCount[i] = 10;
	}
	
	//Re-select random stream 2 (since initializeGame will have selected
	//stream 1 in parent function (main, see randomtestadventurer.c)
	SelectStream(2);
	
	//Determine Deck Sizes
	if(RANDOMIZE && treasureCardCountSpecifier >= 2){
		//Determine random deck sizes, based on treasureCardCountSpecifier
		//(treasureCardCountSpecifier == 2 guarantees a deck size in range
		//6 - MAX_DECK, which will have at minimum one copper and one silver. 
		for(i = 0; i < numPlayers; i++){
			deckCountBeforeAdventurer[i] = 6 + floor(Random() * (MAX_DECK - 5));
		}
	}
	else if(treasureCardCountSpecifier >= 2){
		for(i = 0; i < numPlayers; i++){
			deckCountBeforeAdventurer[i] = (testNumber * 5) + 1;
			if(deckCountBeforeAdventurer[i] >= MAX_DECK){
				deckCountBeforeAdventurer[i] = (deckCountBeforeAdventurer[i] % MAX_DECK) + 1;
			}
		}
	}
	//A deck size of 5 will contain only one treasure card (copper), and 
	//a deck size of 4 or less will contain no treasure cards. (This is so 
	//"boundary" conditions of decks with less than 2 treasure cards can be 
	//tested.)
	else if(treasureCardCountSpecifier >= 0) {
		for(i = 0; i < numPlayers; i++){
			deckCountBeforeAdventurer = treasureCardCountSpecifier + 4;
		}
	}
	else{
		for(i = 0; i < numPlayers; i++){
			deckCountBeforeAdventurer = 0;
		}
	}
	
	//Determine Hand Size
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
		for(i = 0; i < numPlayers; i++){
			handCountBeforeAdventurer[i] = 6 + floor(Random() * MAX_HAND - 5));
		}
	}
	else{
		for(i = 0; i < numPlayers; i++){
			handCountBeforeAdventurer[i] = (testNumber * 5) + 1;
			if(handCountBeforeAdventurer[i] >= MAX_HAND){
				handCountBeforeAdventurer[i] = (handCountBeforeAdventurer[i] % MAX_HAND) + 1;
				if(handCountBeforeAdventurer[i] < 6){
					handCountBeforeAdventurer[i] = 6;
				}
			}
		}
	}
	
	
	//handCountBeforeAdventurer[activePlayer] = 0; <- Testing with handCountBeforeAdventurer 0 fails with Seg Fault or Bus Error!
	//								   			   <-- -- At latest this occurs when the BOUNDARY test for a deck 
	//								   			   <-- -- with no treasure cards is executed.
	
	//For deck...
	if(RANDOMIZE){
		//Randomly load each player's deck according to randomly
		//chosen deck size
		for(m = 0; !emptyDecks && m < numPlayers; m++){
			for(i = 0; i < deckCountBeforeAdventurer[m]; i++){
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
			G->deckCount[m] = deckCountBeforeAdventurer[m];
		}
	}
	else{
		//Load each player's deck with an equal number of each card,
		//plus an extra starting at curse for each remainder after 
		//final multiple of 17, for deckCountBeforeAdventurer as determined above 
		//(e.g. a 20 card deck will have 1 of each card plus 1 extra 
		//curse, estate, and duchy.)
		for(m = 0; !emptyDecks && m < numPlayers; m++){
			for(i = 0, j = 0; i < deckCountBeforeAdventurer[m]; i++){
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
			G->deckCount[m] = deckCountBeforeAdventurer[m];
		}
	}
	
	if(RANDOMIZE){
		//Randomly load each player's hand according to randomly
		//chosen hand size
		for(m = 0; m < numPlayers; m++){
			for(i = 0; i < handCountBeforeAdventurer[m]; i++){
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
		}
	}
	else{
		//Load each player's hand with an equal number of each card,
		//plus an extra starting at curse for each remainder after 
		//final multiple of 17, for handCountBeforeAdventurer as 
		//determined above (e.g. a 20 card deck will have 1 of each 
		//card plus 1 extra curse, estate, and duchy.)
		for(m = 0; m < numPlayers; m++){
			for(i = 0, j = 0; i < handCountBeforeAdventurer[m]; i++){
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
			G->handCount[m] = handCountBeforeAdventurer[m];
		}
	}
	
	//Shuffle each player's hand 
	//--(shuffle has been tested via unittest3.
	//-- see unittest3.c and _unittest3helper.c
	//-- for additional details.)
	//-- (This is deterministic if RANDOMIZE is not enabled,
	//-- since the same seed value is used with each run.)
	for(i = 0; i < numPlayers; i++){
		shuffle(i, G);
	}
	
	//Assign active player hand position for adventurer, 
	//either randomly or according to test number
	int handPos;
	if(RANDOMIZE){
		//Assign a random hand position for Adventurer
		handPos = floor(Random() * (G->handCount[activePlayer]);
		if(handPos == -1){
			handPos = 0;
		}
	}
	else{
		handPos = 0;
		
		//For test #1, replace province with gold so there is
		//a third treasure card in the deck.
		/* if(testNumber == 1){
			G->deck[activePlayer][province] = gold;
		} */
	}
	G->hand[activePlayer][handPos] = adventurer;
	
	//Store additional deck and hand info prior to adventurer call
	//for active player
	
	//For deck...
	int deckCardCountByTypeBeforeAdventurer[27] = {0};
	int deckBeforeAdventurer[MAX_DECK];
	for(i = 0; j < G->deckCount[activePlayer]; i++){
		deckBeforeAdventurer[activePlayer] = G->deck[activePlayer][i];
		deckCardCountByTypeBeforeAdventurer[G->deck[activePlayer][i]]++;
	}
	
	//For hand...
	int handCardCountByTypeBeforeAdventurer[27] = {0};
	//int handBeforeAdventurer[numPlayers][MAX_DECK];				//Not needed since order of hand doesn't matter
	for(i = 0; i < G->handCount[activePlayer]; i++){
		//handBeforeAdventurer[i][j] = G->hand[activePlayer][i];
		handCardCountByTypeBeforeAdventurer[G->hand[activePlayer][i]]++;
	}
	
	//Find total number of treasure cards in activePlayer's hand.
	//(Used in next steps if RANDOMIZE is enabled to ensure active
	//player's deck contains the appropriate number of treasure cards...)
	int treasureCardsInAPDeck = deckCardCountByTypeBeforeAdventurer[copper] +
									deckCardCountByTypeBeforeAdventurer[silver] +
									deckCardCountByTypeBeforeAdventurer[gold];
	
	//In case incorrect number of treasure cards are assigned to active
	//player's deck, reassign indicated number of random treasure cards in 
	//random indexes (this is only possibly when RANDOMIZE is enabled).
	if(RANDOMIZE && treasureCardCountSpecifier >= 2	&& treasureCardsInAPDeck < 2){
		//Determine two random indexes
		int randomTreasureIndex1 = floor(Random() * G->deckCount[activePlayer]),
			randomTreasureIndex2 = floor(Random() * G->deckCount[activePlayer]);
		while(randomTreasureIndex1 == randomTreasureIndex2){
			randomTreasureIndex2 = floor(Random() * G->deckCount[activePlayer]);
		}
		//Assign random treasure cards to random indexes and update card counts
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureIndex1]--;
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureIndex2]--;
		
		int randomTreasureCard1 = copper + floor(Random() * 3),
			randomTreasureCard2 = copper + floor(Random() * 3);
			
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureIndex1] =
			randomTreasureCard1;
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureIndex2] =
			randomTreasureCard2;
			
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureCard1]++;
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureCard2]++;
	}
	else if(RANDOMIZE && treasureCardCountSpecifier < 2 && 
			(treasureCardsInAPDeck != treasureCardCountSpecifier ||
			(treasureCardsInAPDeck != 0 && treasureCardCountSpecifier == -1))){
		//Strip out all treasure cards and replace with random non-treasure card...
		for(i = 0; i < deckCountBeforeAdventurer[activePlayer]; i++){
			if(G->deck[activePlayer][i] == copper || 
			   G->deck[activePlayer][i] == silver || 
			   G->deck[activePlayer][i] == gold){
				   int randomCard = floor(Random() * 27);
				   while(randomCard == copper || 
						 randomCard == silver ||
						 randomCard == gold){
							 randomCard = floor(Random() * 27);
					}
					deckCardCountByTypeBeforeAdventurer[activePlayer][G->deck[activePlayer][i]]--;
				    G->deck[activePlayer][i] = randomCard;
				    deckBeforeAdventurer[activePlayer][i] = randomCard;
				    deckCardCountByTypeBeforeAdventurer[activePlayer][randomCard]++;
			}
		}
		//Assign one random treasure card to a random position if specified.
		//(Note if treasure card specifier is not 1 at this juncture, it 
		// means the deck is to have no treasure cards or is to be empty.
		// i.e. treasureCardCountSpecifier is either 0 or -1)
		if(treasureCardCountSpecifier == 1){
		//Assign random treasure card to random index and update card counts
		int randomTreasureIndex = floor(Random() * G->deckCount[activePlayer]);
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureIndex]--;
		
		int randomTreasureCard = copper + floor(Random() * 3);
			
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureIndex] =
			randomTreasureCard;
			
		deckCardCountByTypeBeforeAdventurer[activePlayer][randomTreasureCard]++;
		}
	}
		
	//int for coin bonus (unused by Adventurer call)
	int coin_bonus = 0;
	
	/*************************  SET UP ENDS HERE  **************************/
	
	/************************* CHECKS BEGIN HERE  **************************/
	
	//Call Adventurer
	cardEffect(adventurer, -1, -1, -1, G, handPos, &coin_bonus);
	
	//Check to make sure G->handCount[activePlayer] has been incremented by the
	//appropriate amount (i.e. number of treasures gained). Adventurer should 
	//gain 2 treasure cards to hand (so long as two are present in the deck).
	//If only 1 treasure is in the deck, only 1 treasure should be gained
	//and handCount should only increment once, and if no treasures are in 
	//the deck, no treasures should be gained and handCount should remain
	//unchanged.
	if(((treasureCardCountSpecifier == 2 && 
		G->handCount[activePlayer] != handCountBeforeAdventurer + 2) ||
		(treasureCardCountSpecifier == 1 && 
		G->handCount[activePlayer] != handCountBeforeAdventurer + 1) ||
		(treasureCardCountSpecifier == 0 && 
		G->handCount[activePlayer] != handCountBeforeAdventurer)) &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Hand count not updated properly after Adventurer play\n"
		"  Expected: %d ; Observed %d %s\n", 
		treasureCardCountSpecifier == 2 ? handCountBeforeAdventurer + 2 : 
		treasureCardCountSpecifier == 1 ? handCountBeforeAdventurer + 1 : 
		handCountBeforeAdventurer,
		G->handCount[activePlayer],
		isBoundary ? "(Boundary)" : "(Non-Boundary)"); 
		failures[*failCt-1].testNumber = testNumber;
	}
	
	//Determine how many of each card type have been removed from (or 
	//erroneously added to) active player's deck
	int deckCardCountByTypeAfterAdventurer[27] = {0};
	for(i = 0; i < G->deckCount[activePlayer]; i++){
		deckCardCountByTypeAfterAdventurer[G->deck[activePlayer][i]]++;
	}
	int deckDiffsAfterAdventurer[27] = {0};
	for(i = 0; i < 27; i++){
		deckDiffsAfterAdventurer[i] =
			deckCardCountByTypeBeforeAdventurer[i] -
			deckCardCountByTypeAfterAdventurer[i];
	}
	
	//Determine how many of each card type have been added to (or erroneously
	//removed from) the active player's hand
	int handCardCountByTypeAfterAdventurer[27] = {0};
	for(i = 0; i < G->handCount[activePlayer]; i++){
		handCardCountByTypeAfterAdventurer[G->hand[activePlayer][i]]++;
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
	
	//Make sure active player's hand count is updated correctly
	if(((treasureCardCountSpecifier == 2 && 
	     G->handCount[activePlayer] != handCountBeforeAdventurer + 2) || 
	    (treasureCardCountSpecifier == 1 && 
	     G->handCount[activePlayer] != handCountBeforeAdventurer + 1) || 
	    (treasureCardCountSpecifier == 0 && 
	   G->handCount[activePlayer] != handCountBeforeAdventurer)) &&
	   ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Hand count not as expected\n"
			"  Expected: %d ; Observed %d %s\n", 
			treasureCardCountSpecifier == 2 ? handCountBeforeAdventurer + 2 : 
			treasureCardCountSpecifier == 1 ? handCountBeforeAdventurer + 1 : 
			handCountBeforeAdventurer, 
			G->handCount[activePlayer],
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
	//tested below) for active player
	int discardCardCountByTypeAfterAdventurer[27] = {0};
	for(i = 0; i < G->discardCount[activePlayer]; i++){
		discardCardCountByTypeAfterAdventurer[G->discard[activePlayer][i]]++;
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
	//of cards removed for active player.
	int totalNumCardsRemovedFromDeck = 0;
	for(i = 0; i < 27; i++){
		totalNumCardsRemovedFromDeck += deckDiffsAfterAdventurer[i];
	}
	if(deckCountBeforeAdventurer - totalNumCardsRemovedFromDeck !=
	   G->deckCount[activePlayer] && ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Deck count after Adventurer does not correspond to cards removed\n"
			" %d cards removed, but resulting deck count is %d %s\n", 
			totalNumCardsRemovedFromDeck, G->deckCount[activePlayer],
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Make sure the deck order is not changed (for cards remaining in deck)
	//for active player
	for(i = 0; i < deckCountBeforeAdventurer - totalNumCardsRemovedFromDeck &&
			i < G->deckCount[activePlayer]; i++){
		if(deckBeforeAdventurer[i] != G->deck[activePlayer][i]
			&& ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Deck after Adventurer re-ordered unexpectedly\n"
				" Expected %d at idx %d ; Observed %d %s\n", 
				deckBeforeAdventurer[i], i, G->deck[activePlayer][i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
				break;
		}
	}
	
	//Make sure discard count is updated correctly for active player
	//(Also account for the adventurer itself which should be discarded,
	// such that if two treasures are gained to hand, all other
	// cards removed from the deck - 2 treasures + 1 adventurer should
	// equal the current deck count, and so forth.)
	if(((treasureCardCountSpecifier >= 2 && 
		G->discardCount[activePlayer] != totalNumCardsRemovedFromDeck - 1) ||
	   (treasureCardCountSpecifier == 1 &&
		G->discardCount[activePlayer] != totalNumCardsRemovedFromDeck) ||
		(treasureCardCountSpecifier <= 0 &&
		G->discardCount[activePlayer] != totalNumCardsRemovedFromDeck + 1)) &&
		++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Discard pile count updated incorrectly\n"
			"  Expected %d cards in discard pile ; Observed % d %s\n", 
			treasureCardCountSpecifier >= 2 ? totalNumCardsRemovedFromDeck - 1 :
			totalNumCardsRemovedFromDeck == 1 ? totalNumCardsRemovedFromDeck :
			totalNumCardsRemovedFromDeck + 1, G->discardCount[activePlayer],
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
		if(G->embargoTokens[i] != embargoTokensBeforeAdventurer[i]
			&& ++(*failCt) <= MAX_FAILS){
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
	if(G->outpostPlayed != outpostPlayedBeforeAdventurer
		&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Outpost played changed unexpectedly\n"
			"  Expected 0 ; Observed %d %s\n", 
			G->outpostPlayed,
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check outpost turn...
	if(G->outpostTurn != outpostTurnBeforeAdventurer
		&& ++(*failCt) <= MAX_FAILS){
			failures[*failCt-1].lineNumber = __LINE__;
			sprintf(failures[*failCt-1].description,
			"Outpost turn changed unexpectedly\n"
			"  Expected 0 ; Observed %d %s\n", 
			G->outpostTurn,
			isBoundary ? "(Boundary)" : "(Non-Boundary)");
			failures[*failCt-1].testNumber = testNumber;
	}
	
	//Check other players' values (all should be unchanged)
	
	//Check discard counts...
	for(i = 0; i < numPlayers; i++){
		if(i != activePlayer && G->discardCount[i] != 0 && 
		   ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Player %d discard count changed unexpectedly\n"
				"  Expected 0 ; Observed %d %s\n", 
				i, G->discardCount[i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//Check deck counts...
	for(i = 0; i < numPlayers; i++){
		if(i != activePlayer && G->deckCount[i] != 0 && 
		   ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Player %d deck count changed unexpectedly\n"
				"  Expected 0 ; Observed %d %s\n", 
				i, G->deckCount[i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//Check hand counts...
	for(i = 0; i < numPlayers; i++){
		if(i != activePlayer && G->handCount[i] != 0 && 
		   ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Player %d hand count changed unexpectedly\n"
				"  Expected 0 ; Observed %d %s\n", 
				i, G->handCount[i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
		}
	}
	
	//Check discard piles...
	for(m = 0; m < numPlayers; m++){
		for(i = 0; i < MAX_DECK; i++){
			if(G->discard[m][i] != -1 && ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Player %d deck changed unexpectedly at idx %d\n"
				"  Expected -1 ; Observed %d %s\n", 
				m, i, G->discard[m][i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			break;
		}
	}
	
	//Check decks...
	for(m = 0; m < numPlayers; m++){
		for(i = 0; i < MAX_DECK; i++){
			if(G->deck[1][m] != -1 && ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Player %d deck changed unexpectedly at idx %d\n"
				"  Expected -1 ; Observed %d %s\n", 
				m, i, G->deck[1][m],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			break;
		}
	}
	
	//Check hands...
	for(m = 0; m < numPlayers; m++){	
		for(i = 0; i < MAX_HAND; i++){
			if(G->hand[m][i] != -1 && ++(*failCt) <= MAX_FAILS){
				failures[*failCt-1].lineNumber = __LINE__;
				sprintf(failures[*failCt-1].description,
				"Player %d hand changed unexpectedly at idx %d\n"
				"  Expected -1 ; Observed %d %s\n", 
				m, i, G->hand[m][i],
				isBoundary ? "(Boundary)" : "(Non-Boundary)");
				failures[*failCt-1].testNumber = testNumber;
			}
			break;
		}
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
	
	
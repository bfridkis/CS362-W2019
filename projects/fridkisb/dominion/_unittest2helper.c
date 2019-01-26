/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *               _unittest2helper.c
 *
 *		   Helper function for unittest2.c
 *
 *			(Tests "gainCard" function)
 *
 * Include the following lines in your makefile:
 *
 * _unittest2helper.o: _unittest2helper.c _unittest2helper.h dominion.o
 *		gcc -c _unittest2helper.c -g  $(CFLAGS)
 * ---------------------------------------------------------------------------
 */
 
#include "_unittest2helper.h"
 
#define GAINCARDCALL1
#define GAINCARDCALL2

//Set this in unittest2.c also!
#define MAX_FAILS 400
 
int _unittest2helper(int testCase, int k[], struct gameState* G,
	failedTest failures[]){
	 
	//Test value variables	   
	int i, j, returnValue, failCt = 0;
	
	//Used to track cards gained to discard, deck, and hand piles.
	//-- int addedTo[0] == cards added to discount
	//-- int addedTo[1] == cards added to deck
	//-- int addedTo[2] == cards added to hand
	//Index number corresponds to card type.
	int addedTo[3][17];
	memset(addedTo, 0, sizeof(int) * 3 * 17);
	
	//Ensure player 0's deck, discard, and hand are cleared
		memset(G->deck[0], -1, sizeof(int) * MAX_DECK);
		memset(G->discard[0], -1, sizeof(int) * MAX_DECK);
		memset(G->hand[0], -1, sizeof(int) * MAX_HAND);
		G->deckCount[0] = 0;
		G->discardCount[0] = 0;
		G->handCount[0] = 0;
	
	for(i = 0; i < 17; i++){
		
		//Treasure_map
		case(i == 0):
			returnValue = gainCard(26, G, 0, 0);
			
void _unittest2helper2(int rv, int expectedRV, struct gameState* G,
	int expectedPile, int expectedIdx, int cardValue, int k[]){

	//Check to make sure return value is as expected (0)
	if(rv != expectedRV){
		if(++(*failedTestCount) <= MAX_FAILS){
		  failures[(*failedTestCount)-1].lineNumber = BUYCARDCALL1;
		  sprintf(failures[(*failedTestCount)-1].description,
			"Return value not as expected.\n"
			"Expected: %d ; Observed %d when buying kingdom card at idx: %d",
			rv, expectedRV, expectedIdx);
		}
	  failCt++;	
	}
	
	//Check to make sure card was gained to expected pile
	if(expectedPile = 0 && G->discard[0][expectedIdx] != cardValue){
		if(++(*failedTestCount) <= MAX_FAILS){
		  failures[(*failedTestCount)-1].lineNumber = BUYCARDCALL1;
		  sprintf(failures[(*failedTestCount)-1].description,
			"Card not gained to proper location.\n"
			"Expected: %d at idx %d of discard pile ; Observed %d when gaining %d",
			cardValue, expectedIdx, G->discard[0][expectedIdx], cardValue);
		}
	  failCt++;	
	}
	else if(expectedPile = 1 && G->deck[0][expectedIdx] != cardValue){
		if(++(*failedTestCount) <= MAX_FAILS){
		  failures[(*failedTestCount)-1].lineNumber = BUYCARDCALL1;
		  sprintf(failures[(*failedTestCount)-1].description,
			"Card not gained to proper location.\n"
			"Expected: %d at idx %d of deck ; Observed %d when gaining %d",
			cardValue, expectedIdx, G->discard[0][expectedIdx], cardValue);
		}
	  failCt++;	
	}
	else if(expectedPile = 2 && G->hand[0][expectedIdx] != cardValue){
		if(++(*failedTestCount) <= MAX_FAILS){
		  failures[(*failedTestCount)-1].lineNumber = BUYCARDCALL1;
		  sprintf(failures[(*failedTestCount)-1].description,
			"Card not gained to proper location.\n"
			"Expected: %d at idx %d of hand ; Observed %d when gaining %d",
			cardValue, expectedIdx, G->discard[0][expectedIdx], cardValue);
		}
	  failCt++;	
	}
	
	//Check to make sure no cards are in unexpected locations
	for(j = 0; j < MAX_DECK; j++){			
		if(j < expectedIdx && G->discount[0][j] != k[j] ||
		   j > expectedIdx && G->discount[0][j] != -1){
			
				
	
	
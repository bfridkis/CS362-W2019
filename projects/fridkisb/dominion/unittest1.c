/* -----------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *               unittest1.c
 *
 * Include the following lines in your makefile:
 *
 * testBuyCard: testBuyCard.c dominion.o rngs.o
 *      gcc -o testBuyCard -g  testBuyCard.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define BUYCARDCALL1 66
#define BUYCARDCALL2 166
#define BUYCARDCALL2 238


struct failedTest {
	int lineNumber;
	char[100] description;
}

typedef struct failedTest failedTest;

int main (int argc, char** argv) {
	struct gameState G;

	//Structure array to note each failure
	failedTest failures[500];
	int failedTestCount = 0;

	//Kingdom card types for test game
	int k[10] = {adventurer = 6, ambassador = 3, embargo = 2, village = 3, minion = 5, mine = 5, cutpurse = 4,
		   sea_hag = 4, tribute = 5, smithy = 4};
		   
	//Test value variables	   
	int i, j, returnValue;

		   
	printf("Starting unittest1 - Testing 'buyCard' function\n");

	
	
	//Initializes game for two players, with seed value of 5000.
	initializeGame(2, k, 5000, &G);
	
	//This is just in case initializeGame is not functioning properly
	G.whoseTurn = 0; 
	
	//Test all buyCard functionality under normal circumstances.
	//For each of the 10 kingdom cards listed in k[], exhaust
	//the supply pile.
	for(i = 0; i < 10; i++){
		G.discardCount[0] = 0;
		for(j = 1; j <= 10; j++){
			G.numBuys = 1;
			G.coins = 6;
			returnValue = buyCard(k[i]);
			
			//Check to make sure G.whoseTurn has not been changed
			if(G.whoseTurn != 0){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"whoseTurn unexpectedly changed.\n"
				"Expected: 0 ; Observed %d", G.whoseTurn);
			}
			
			//Check to make sure return value is as expected (0)
			if(returnValue != 0){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"Return value not as expected.\n"
				"Expected: 0 ; Observed %d", returnValue);
			}
			
			//Check to make sure purchased card is placed onto discard pile
			if(G->discard[0][j-1] != k[i]){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  strcpy(failures[failedTestCount++].description,
				"Purchased card not in expected discount pile location.");
			}
			
			//Check to make sure discardCount is updated as expected
			if(G.discardCount[0] != j){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"Discard (card) count not updated as expected.\n"
				"Expected: %d ; Observed %d", j, G.discardCount[0]);
			}
			
			//Check to make sure coin count is updated as expected,
			//depending on card type.
			
			//For cards costing 6 coins (adventurer)...
			if(i == 0 && G->coins != 0){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"Coin value not updated as expected.\n"
				"Expected: 0; Observed %d", G.coins);
			}
			
			//For cards costing 5 coins (minion, mine, & tribute)...
			else if((i == 4 || i == 5 || i = 8) && G->coins != 1){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"Coin value not updated as expected.\n"
				"Expected: 1; Observed %d", G.coins);
			}
			
			//For cards costing 4 coins (cutpurse, sea hag, & smithy)...
			else if((i == 6 || i == 7 || i = 9) && G->coins != 2){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"Coin value not updated as expected.\n"
				"Expected: 2; Observed %d", G.coins);
			}
			
			//For cards costing 3 coins (ambassador & village)...
			else if((i == 0 || i == 2) && G->coins != 3){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"Coin value not updated as expected.\n"
				"Expected: 3; Observed %d", G.coins);
			}
			
			//For cards costing 2 coins (embargo)...
			else if(G->coins != 4){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"Coin value not updated as expected.\n"
				"Expected: 4; Observed %d", G.coins);
			}
			
			//Check to make sure numBuys is updated correctly
			if(G.numBuys != 0){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"numBuys not updated as expected.\n"
				"Expected: 0; Observed %d", G.numBuys);
			}
			
			//Check to make sure the supplyCount is updated correctly
			if(G.supplyCount[k[i]] != 10 - j){
			  failures[failedTestCount].lineNumber = BUYCARDCALL1;
			  sprintf(failures[failedTestCount++].description,
				"Supply pile count not updated as expected.\n"
				"Expected: %d; Observed %d", 10 - j, G.supplyCount[k[i]]);
			}
		}
	}
	
	//Test buyCard response when coins are insufficient for each kingdom card.
	for(i = 0; i < 10; i++){
		G.discardCount[0] = 0;
		memset(G.discard[0], -1, sizeof(int) * MAX_DECK);
		G.numBuys = 1;
		G.coins = 0;
		returnValue = buyCard(k[i]);
		
		//Check to make sure G.whoseTurn has not been changed
		if(G.whoseTurn != 0){
		  failures[failedTestCount].lineNumber = BUYCARDCALL1;
		  sprintf(failures[failedTestCount++].description,
			"whoseTurn unexpectedly changed.\n"
			"Expected: 0 ; Observed %d", G.whoseTurn);
		}
		
		//Check to make sure return value is as expected (-1)
		if(returnValue != -1){
		  failures[failedTestCount].lineNumber = BUYCARDCALL2;
		  sprintf(failures[failedTestCount++].description,
			"Return value not as expected.\n"
			"Expected: 0 ; Observed %d", returnValue);
		}
		
		//Check to make sure no card is added to discard pile
		for(i = 0; i < MAX_DECK; i++){
			if(G->discount[0][i] != -1){
			  failures[failedTestCount].lineNumber = BUYCARDCALL2;
			  sprintf(failures[failedTestCount++].description,
				"Card erroneously added to discard pile at position %d,\n"
				"\tdespite insufficient coins", i);
			  break;
			}
		}
		
		//Check to make sure discardCount is not changed
		if(G.discardCount[0] != 0){
		  failures[failedTestCount].lineNumber = BUYCARDCALL2;
		  sprintf(failures[failedTestCount++].description,
			"Discard count erroneously changed.\n"
			"Should remain 0 when coins are insufficient for purchase.\n"
			"Expected: 0 ; Observed %d", G.discardCount[0]);
		}
		
		//Check to make sure coin count is not changed
		if(G->coins != 0){
		  failures[failedTestCount].lineNumber = BUYCARDCALL2;
		 sprintf(failures[failedTestCount++].description,
			"Coin value erroneously changed.\n"
			"Should remain 0 when coins are insufficient for purchase.\n"
			"Expected: 0 ; Observed %d", G.coins);
		}
		
		//Check to make sure numBuys is not changed
		if(G.numBuys != 1){
		  failures[failedTestCount].lineNumber = BUYCARDCALL2;
		  sprintf(failures[failedTestCount++].description,
			"numBuys erroneously changed.\n"
			"Should remain 0 when coins are insufficient for purchase.\n"
			"Expected: 0; Observed %d", G.numBuys);
		}
		
		//Check to make sure the supplyCount is not changed for each kingdom card
		for(i = 0; i < 10; i++){
			if(G.supplyCount[k[i]] != 10){
				failures[failedTestCount].lineNumber = BUYCARDCALL2;
				sprintf(failures[failedTestCount++].description,
				"Supply pile count erroneously changed.\n"
				"Pile %d Expected : 10 ; Observed %d", i, G.supplyCount[k[i]]);
			}
		}
	}
	
	//Test buyCard response when numBuys are insufficient for purchase,
	//for each kingdom card.
	for(i = 0; i < 10; i++){
		G.discardCount[0] = 0;
		memset(G.discard[0], -1, sizeof(int) * MAX_DECK);
		G.numBuys = 0;
		G.coins = 6;
		returnValue = buyCard(k[i]);
		
		//Check to make sure G.whoseTurn has not been changed
		if(G.whoseTurn != 0){
		  failures[failedTestCount].lineNumber = BUYCARDCALL1;
		  sprintf(failures[failedTestCount++].description,
			"whoseTurn unexpectedly changed.\n"
			"Expected: 0 ; Observed %d", G.whoseTurn);
		}
		
		//Check to make sure return value is as expected (-1)
			if(returnValue != -1){
			  failures[failedTestCount].lineNumber = BUYCARDCALL2;
			  sprintf(failures[failedTestCount++].description,
				"Return value not as expected.\n"
				"Expected: 0 ; Observed %d", returnValue);
			}
		
		//Check to make sure no card is added to discard pile
		for(i = 0; i < MAX_DECK; i++){
			if(G->discount[0][i] != -1){
			  failures[failedTestCount].lineNumber = BUYCARDCALL2;
			  sprintf(failures[failedTestCount++].description,
				"Card erroneously added to discard pile at position %d,\n"
				"\tdespite insufficient numBuys", i);
			  break;
			}
		}
		
		//Check to make sure discardCount is not changed
		if(G.discardCount[0] != 0){
		  failures[failedTestCount].lineNumber = BUYCARDCALL2;
		  sprintf(failures[failedTestCount++].description,
			"Discard count erroneously changed.\n"
			"Should remain 0 when numBuys = 0.\n"
			"Expected: 0 ; Observed %d", G.discardCount[0]);
		}
		
		//Check to make sure coin count is not changed
		if(G->coins != 6){
		  failures[failedTestCount].lineNumber = BUYCARDCALL2;
		 sprintf(failures[failedTestCount++].description,
			"Coin value erroneously changed.\n"
			"Should remain 6 when numBuys = 0.\n"
			"Expected: 0 ; Observed %d", G.coins);
		}
		
		//Check to make sure numBuys is not changed
		if(G.numBuys != 0){
		  failures[failedTestCount].lineNumber = BUYCARDCALL2;
		  sprintf(failures[failedTestCount++].description,
			"numBuys erroneously changed.\n"
			"Should remain 0 when numBuys = 0.\n"
			"Expected: 0; Observed %d", G.numBuys);
		}
		
		//Check to make sure the supplyCount is not changed for each kingdom card
		for(i = 0; i < 10; i++){
			if(G.supplyCount[k[i]] != 10){
				failures[failedTestCount].lineNumber = BUYCARDCALL2;
				sprintf(failures[failedTestCount++].description,
				"Supply pile count erroneously changed.\n"
				"Pile %d Expected : 10 ; Observed %d", i, G.supplyCount[k[i]]);
			}
		}
	}
	
	if(failedTestCount == 0){
		printf("**ALL TESTS PASSED!**");
	}
	else{
		printf("\t%d tests failed, as follows:\n\n", failedTestCount);
		for(i = 0; i < failedTestCount; i++){
			printf("%d - LINE %d: %s", 
				i, failures[i].lineNumber, failures[i].description);
		}
	}
}
		
		  
		  
		  
		  
		  
		  
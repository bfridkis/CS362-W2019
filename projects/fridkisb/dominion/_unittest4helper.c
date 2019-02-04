/* ---------------------------------------------------------------------------
 * Benjamin Fridkis - CS362 
 * Assignment 3
 *
 *               _unittest4helper.c
 *
 *		   Helper function for unittest4.c
 *
 *	 (Tests "updateCoins" function - See dominion.c line 1137)
 *
 * Include the following lines in your makefile:
 *
 * _unittest4helper.o: _unittest4helper.c _unittest4helper.h dominion.o
 *		gcc -c _unittest4helper.c -g  $(CFLAGS)
 *
 * ---------------------------------------------------------------------------
 */
 
#include "_unittest4helper.h"
 
#define NUM_PLAYERS 2
#define MAX_BONUS 100
 
int _unittest4helper(int k[], struct gameState* G, failedTest failures[], 
	int* failCt, int isNoTreasureTest, int isNoBonusTest, int testNumber){
	 
	//Ensure hand and handCount are cleared for player 0
	memset(G->hand[0], -1, sizeof(int) * MAX_HAND);
	G->handCount[0] = 0;
	
	//Re-select random number stream 2 (since initializeGame call
	//in unittest4.c will select stream 1)
	SelectStream(2);
	
	//Test value variables	   
	int i, treasureCardCount = 0, bonusValue = 0,
		numCopper, numSilver, numGold, expectedCoinValue = 0;
		
	if(!isNoTreasureTest){
		//Determine a random number of treasure cards in range 0 to MAX_HAND, 
		//and load player 0's hand with a random assortment of treasure cards
		//in total quantity equal to this value.
		treasureCardCount = Random() * MAX_HAND;

		//Determine random assortment of treasure cards.
		numCopper = Random() * treasureCardCount;
		numSilver = Random() * (treasureCardCount - numCopper);
		numGold = treasureCardCount - numCopper - numSilver;
	}	
	if(!isNoBonusTest){
		//Determine a random bonus in range 0 to MAX_BONUS, and assign
		//to bonusValue
		bonusValue = Random() * MAX_BONUS;
	}
	
	//Load hand with random assortment of treasure cards, as determined above
	for(i = 0; i < treasureCardCount; i++){
		if((int)(Random() * INT_MAX) % 3 == 0){
			if(numCopper-- > 0){
				G->hand[0][i] = copper;
				expectedCoinValue++;
			}
			else if(numSilver-- > 0){
				G->hand[0][i] = silver;
				expectedCoinValue += 2;
			}
			else{
				numGold--;
				G->hand[0][i] = gold;
				expectedCoinValue += 3;
			}
		}
		else if((int)(Random() * INT_MAX) % 3 == 1){
			if(numSilver-- > 0){
				G->hand[0][i] = silver;
				expectedCoinValue += 2;
			}
			else if(numGold-- > 0){
				G->hand[0][i] = gold;
				expectedCoinValue += 3;
			}
			else{
				numCopper--;
				G->hand[0][i] = copper;
				expectedCoinValue++;
			}
		}
		else{
			if(numGold-- > 0){
				G->hand[0][i] = gold;
				expectedCoinValue += 3;
			}
			else if(numCopper-- > 0){
				G->hand[0][i] = copper;
				expectedCoinValue++;
			}
			else{
				numSilver--;
				G->hand[0][i] = silver;
				expectedCoinValue += 2;
			}
		}
		G->handCount[0]++;
	}
	
	//Add random bonus to expectedCoinValue (or 0 for no bonus test)
	expectedCoinValue += bonusValue;
	
	/*************************  SET UP ENDS HERE  **************************/
	
	/************************* CHECKS BEGIN HERE  **************************/
	
	//Update Coins
	updateCoins(0, G, bonusValue);
	
	
	//No treasure AND no bonus test
	if(isNoTreasureTest && isNoBonusTest && G->coins != 0 &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"No treasure cards & no bonus test failed\n"
		"  Expected: 0 ; Observed %d\n", G->coins);
		failures[*failCt-1].testNumber = testNumber;
		return -1;
	}
	
	//No treasure test
	else if(isNoTreasureTest && !isNoBonusTest && G->coins != bonusValue &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"No treasure cards test failed\n"
		"  Expected: %d (bonus value only); Observed %d\n", bonusValue, G->coins);
		failures[*failCt-1].testNumber = testNumber;
		return -1;
	}
	
	//No bonus test
	else if(isNoBonusTest && !isNoTreasureTest && G->coins != expectedCoinValue &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"No bonus test failed\n"
		"  Expected: %d (treasure value only); Observed %d\n",
		expectedCoinValue, G->coins);
		failures[*failCt-1].testNumber = testNumber;
		return -1;
	}
	
	//Check for proper coin value (all other tests)
	else if(!isNoBonusTest && !isNoTreasureTest && G->coins != expectedCoinValue &&
		++(*failCt) <= MAX_FAILS){
		failures[*failCt-1].lineNumber = __LINE__;
		sprintf(failures[*failCt-1].description,
		"Coin value not updated correctly\n"
		"  Expected: %d ; Observed %d\n", expectedCoinValue, G->coins);
		failures[*failCt-1].testNumber = testNumber;
	}
	
	return 0;
}
	
	
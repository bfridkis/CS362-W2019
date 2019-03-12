#include "cardEffects.h"
#include "dominion_helpers.h"
#include <stdio.h>

//  Note:
//                        Main bug in adventurer function :
//			(Before the introduction of additional bugs for assignment 2) 
//		
//		If there are not at least 2 treasure cards in the set of cards making up the 
//		deck and discard piles, the function will start "consuming" the hand itself in 
//		search of treasures. These treasures found in the existing hand will be used 
//		erroneously if found. If there are not at least 2 treasures in the
//		set of cards making up the deck, discard, and hand (i.e. the set of all
//		cards for the given player), the function will eventually fault the program
//		by attempting to access the player's hand array using an invalid index (-1)
// 		(state->hand[player][-1]) in continuous search of treasures. The faulty mechanism
//		lies in the fact that drawCard will simply return a -1 once it has placed
//		all cards in the temp hand after depleting both the deck and discard piles,
//		whereafter playAdventurer erroneously assumes drawCard is still adding 
//		to the hand. The hand itself is then added to the temp hand until no cards are
//		remaining (i.e. handCount has reached 0. Finally it (handCount) will reach -1,
//		and the attempt on line 33 to access state->hand[currentPlayer][-1] will crash
//		the program.

int playAdventurer(int currentPlayer, struct gameState *state){
	int temphand[MAX_HAND];
	int drawntreasure = 0, z = 0;
	while(drawntreasure<2){
	if (state->deckCount[currentPlayer] <1){//if the deck is empty we need to shuffle discard and add to deck
	  shuffle(currentPlayer, state);
	}
	drawCard(currentPlayer, state);
	int cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
	if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
	  drawntreasure++;
	else{
	  temphand[z]=cardDrawn;
	  state->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
	  z++;
	}
      }
      while(z-1>=0){
	state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
	z=z-1;
      }
      return 0;
}

int playSmithy(int currentPlayer, struct gameState *state, int handPos){
	int i;
	//+3 Cards
      for (i = 0; i < 3; i++)
	{
	  drawCard(currentPlayer, state);
	}
			
      //discard card from hand
      discardCard(handPos, currentPlayer, state, 0);
      return 0;
}

int cutpurseEffect(int currentPlayer, struct gameState *state, int handPos){
	updateCoins(currentPlayer, state, 2);
      int i, j, k;
	  for (i = 0; i < state->numPlayers; i++)
	{
	  if (i != currentPlayer)
	    {
	      for (j = 0; j < state->handCount[i]; j++)
		{
		  if (state->hand[i][j] == copper)
		    {
		      discardCard(j, i, state, 0);
		      break;
		    }
			//The if statement below can never be true...
		  if (j == state->handCount[i])
		    {
		      for (k = 0; k < state->handCount[i]; k++)
			{
			  if (DEBUG)
			    printf("Player %d reveals card number %d\n", i, state->hand[i][k]);
			}	
		      break;
		    }		
		}
					
	    }
				
	}				

      //discard played card from hand
      discardCard(handPos, currentPlayer, state, 0);			

      return 0;
}

int minionEffect(int currentPlayer, struct gameState *state, int choice1,
	int choice2, int handPos){
	  //+1 action
      state->numActions++;
			
      //discard card from hand
      discardCard(handPos, currentPlayer, state, 0);
			
      if (choice1)		//+2 coins
	{
	  state->coins = state->coins + 2;
	}
			
      else if (choice2)		//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
	{
	  //discard hand
	  while(numHandCards(state) > 0)
	    {
	      discardCard(handPos, currentPlayer, state, 0);
	    }
				
	  //draw 4
	  int i, j;
	  for (i = 0; i < 4; i++)
	    {
	      drawCard(currentPlayer, state);
	    }
				
	  //other players discard hand and redraw if hand size > 4
	  for (i = 0; i < state->numPlayers; i++)
	    {
	      if (i != currentPlayer)
		{
		  if ( state->handCount[i] >= 4 )
		    {
		      //discard hand
		      while( state->handCount[i] > 1 )
			{
			  discardCard(handPos, i, state, 0);
			}
							
		      //draw 4
		      for (j = 0; j < 3; j++)
			{
			  drawCard(i, state);
			}
		    }
		}
	    }
				
	}
      return 0; 
}

int ambassadorEffect(int currentPlayer, struct gameState *state, int choice1,
	int choice2, int handPos){
	int j = 0;		//used to check if player has enough cards to discard

      if (choice2 > 2 || choice2 < 0)
	{
	  return -1;				
	}

      if (choice1 == handPos)
	{
	  return -1;
	}

	  int i;
      for (i = 0; i < state->handCount[currentPlayer]; i++)
	{
	  if (i != handPos && i == state->hand[currentPlayer][choice1] && i != choice1)
	    {
	      j++;
	    }
	}
      if (j < choice2)
	{
	  return -1;				
	}

      if (DEBUG) 
	printf("Player %d reveals card number: %d\n", currentPlayer, state->hand[currentPlayer][choice1]);

      //increase supply count for choosen card by amount being discarded
      state->supplyCount[state->hand[currentPlayer][choice1]] += choice2;
			
      //each other player gains a copy of revealed card
      for (i = 0; i < state->numPlayers; i++)
	{
	  if (i != currentPlayer)
	    {
	      gainCard(state->hand[currentPlayer][choice1], state, 0, i);
	    }
	}

      //discard played card from hand
      discardCard(handPos, currentPlayer, state, 0);			

      //trash copies of cards returned to supply
      for (j = 0; j < choice2; j++)
	{
	  for (i = 0; i < state->handCount[currentPlayer]; i++)
	    {
	      if (state->hand[currentPlayer][i] == state->hand[currentPlayer][choice1])
		{
		  discardCard(i, currentPlayer, state, 1);
		  break;
		}
	    }
	}			

      return 0;
}

int playCouncilRoom(int currentPlayer, struct gameState *state, int handPos){
	int i;
	//+4 Cards
	  for (i = 0; i < 4; i++)
	{
	  drawCard(currentPlayer, state);
	}
			
	  //+1 Buy
	  state->numBuys++;
			
	  //Each other player draws a card
	  for (i = 0; i < state->numPlayers; i++)
	{
	  if ( i != currentPlayer )
		{
		  drawCard(i, state);
		}
	}
	
	  //put played card in played card pile
      discardCard(handPos, currentPlayer, state, 0);
	
	return 0;
}
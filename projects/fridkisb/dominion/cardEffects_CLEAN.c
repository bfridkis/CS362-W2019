#include "cardEffects.h"
#include "dominion_helpers.h"
#include <stdio.h>

int adventurerEffect(int currentPlayer, struct gameState *state){
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

int smithyEffect(int currentPlayer, struct gameState *state, int handPos){
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
	updateCoins(currentPlayer, state, 3);
      int i, j, k;
	  for (i = 0; i < state->numPlayers-1; i++)
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
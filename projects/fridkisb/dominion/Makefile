CFLAGS= -Wall -Wfatal-errors -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)
	
cardEffects.o: cardEffects.h cardEffects.c dominion.h
	gcc -c cardEffects.c -g  $(CFLAGS)	

dominion.o: dominion.h dominion.c rngs.o cardEffects.o
	gcc -c dominion.c -g  $(CFLAGS)
	
_unittest1helper.o: _unittest1helper.c _unittest1helper.h dominion.o
	gcc -c _unittest1helper.c -g  $(CFLAGS)
	
_unittest2helper.o: _unittest2helper.c _unittest2helper.h dominion.o
	gcc -c _unittest2helper.c -g  $(CFLAGS)
	
_unittest3helper.o: _unittest3helper.c _unittest3helper.h dominion.o
	gcc -c _unittest3helper.c -g  $(CFLAGS)

_unittest4helper.o: _unittest4helper.c _unittest4helper.h dominion.o
	gcc -c _unittest4helper.c -g  $(CFLAGS)
	
_cardtest1helper.o: _cardtest1helper.c _cardtest1helper.h dominion.o
	gcc -c _cardtest1helper.c -g  $(CFLAGS)
	
_cardtest2helper.o: _cardtest2helper.c _cardtest2helper.h dominion.o
	gcc -c _cardtest2helper.c -g  $(CFLAGS)
	
_cardtest3helper.o: _cardtest3helper.c _cardtest3helper.h dominion.o
	gcc -c _cardtest3helper.c -g  $(CFLAGS)
	
playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o cardEffects.o rngs.o $(CFLAGS)
#To run playdom you need to enter: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

unittest1: unittest1.c _unittest1helper.o dominion.o cardEffects.o rngs.o
	gcc -o unittest1 unittest1.c -g dominion.o cardEffects.o _unittest1helper.o rngs.o $(CFLAGS)
	
unittest2: unittest2.c _unittest2helper.o dominion.o cardEffects.o rngs.o
	gcc -o unittest2 unittest2.c -g dominion.o cardEffects.o _unittest2helper.o rngs.o $(CFLAGS)	
	
unittest3: unittest3.c _unittest3helper.o dominion.o cardEffects.o rngs.o
	gcc -o unittest3 unittest3.c -g dominion.o cardEffects.o _unittest3helper.o rngs.o $(CFLAGS)
	
unittest4: unittest4.c _unittest4helper.o dominion.o cardEffects.o rngs.o
	gcc -o unittest4 unittest4.c -g dominion.o cardEffects.o _unittest4helper.o rngs.o $(CFLAGS)
	
cardtest1: cardtest1.c _cardtest1helper.o dominion.o cardEffects.o rngs.o
	gcc -o cardtest1 cardtest1.c -g dominion.o cardEffects.o _cardtest1helper.o rngs.o $(CFLAGS)
	
cardtest2: cardtest2.c _cardtest2helper.o dominion.o cardEffects.o rngs.o
	gcc -o cardtest2 cardtest2.c -g dominion.o cardEffects.o _cardtest2helper.o rngs.o $(CFLAGS)
	
cardtest3: cardtest3.c _cardtest3helper.o dominion.o cardEffects.o rngs.o
	gcc -o cardtest3 cardtest3.c -g dominion.o cardEffects.o _cardtest3helper.o rngs.o $(CFLAGS)
	
testAll: dominion.o testSuite.c 
	gcc -o testSuite testSuite.c -g  dominion.o cardEffects.o rngs.o $(CFLAGS)
	
unittestresults: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3
	@chmod +x unittestmakehelper
	@./unittestmakehelper
	@chmod +x cardtestmakehelper
	@./cardtestmakehelper

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o cardEffects.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno \
	*.so *.out testDrawCard testDrawCard.exe unittest1 unittest2 unittest3 unittest4 \
	cardtest1 cardtest2 cardtest3 cardtest4
	
# References
# https://stackoverflow.com/questions/3824500/compiling-c-on-remote-linux-machine-clock-skew-detected-warning
# https://stackoverflow.com/questions/10520623/how-to-split-one-string-into-multiple-variables-in-bash-shell
# https://stackoverflow.com/questions/4678967/makefile-command-substitution-problem/4679166#4679166

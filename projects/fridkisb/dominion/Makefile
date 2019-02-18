CFLAGS= -Wall -Werror -Wfatal-errors -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)
	
cardEffects.o: cardEffects.h cardEffects.c dominion.h
	gcc -c cardEffects.c -g  $(CFLAGS)	

dominion.o: dominion.h dominion.c rngs.o cardEffects.o
	gcc -c dominion.c -g  $(CFLAGS)
	
_randomtestcard1helper.o: _randomtestcard1helper.c _randomtestcard1helper.h dominion.o
	gcc -c _randomtestcard1helper.c -g  $(CFLAGS)
	
_randomtestcard2helper.o: _randomtestcard2helper.c _randomtestcard2helper.h dominion.o
	gcc -c _randomtestcard2helper.c -g  $(CFLAGS)
	
_randomtestadventurerhelper.o: _randomtestadventurerhelper.c _randomtestadventurerhelper.h dominion.o
	gcc -c _randomtestadventurerhelper.c -g  $(CFLAGS)
	
playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o cardEffects.o rngs.o $(CFLAGS)
#To run playdom you need to enter: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)
	
randomtestcard1: randomtestcard1.c _randomtestcard1helper.o dominion.o cardEffects.o rngs.o
	gcc -o randomtestcard1 randomtestcard1.c -g dominion.o cardEffects.o \
	_randomtestcard1helper.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c _randomtestcard2helper.o dominion.o cardEffects.o rngs.o
	gcc -o randomtestcard2 randomtestcard2.c -g dominion.o cardEffects.o \
	_randomtestcard2helper.o rngs.o $(CFLAGS)

randomtestadventurer: randomtestadventurer.c _randomtestadventurerhelper.o dominion.o cardEffects.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c -g dominion.o cardEffects.o \
	_randomtestadventurerhelper.o rngs.o $(CFLAGS)		
	
testAll: dominion.o testSuite.c 
	gcc -o testSuite testSuite.c -g  dominion.o cardEffects.o rngs.o $(CFLAGS)
	
randomtestresults: randomtestcard1 randomtestcard2 randomtestadventurer		 
	@chmod +x randomtestmakehelper
	@./randomtestmakehelper randomtestcard1
	@./randomtestmakehelper randomtestcard2
	@./randomtestmakehelper randomtestcardadventurer

randomtestcard1.out: randomtestcard1
	@chmod +x randomtestmakehelper
	@./randomtestmakehelper randomtestcard1
	
randomtestcard2.out: randomtestcard2
	@chmod +x randomtestmakehelper
	@./randomtestmakehelper randomtestcard2
	
randomtestadventurer.out: randomtestadventurer
	@chmod +x randomtestmakehelper
	@./randomtestmakehelper randomtestadventurer

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
	*.so *.out testDrawCard testDrawCard.exe randomtestcard1 randomtestcard2 \
	randomtestadventurer
	
# References
# https://stackoverflow.com/questions/3824500/compiling-c-on-remote-linux-machine-clock-skew-detected-warning
# https://stackoverflow.com/questions/10520623/how-to-split-one-string-into-multiple-variables-in-bash-shell
# https://stackoverflow.com/questions/4678967/makefile-command-substitution-problem/4679166#4679166

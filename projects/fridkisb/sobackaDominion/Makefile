CFLAGS= -Wall -Werror -Wfatal-errors -fpic -coverage -lm -g -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c $(CFLAGS)	

dominion.o: dominion.h dominion.c rngs.o 
	gcc -c dominion.c $(CFLAGS)
	
_cardtesthelper_smithy.o: _cardtesthelper_smithy.c \
						  _cardtesthelper_smithy.h dominion.o 
	gcc -c _cardtesthelper_smithy.c $(CFLAGS)
	
_cardtesthelper_adventurer.o: _cardtesthelper_adventurer.c \
							  _cardtesthelper_adventurer.h dominion.o 
	gcc -c _cardtesthelper_adventurer.c $(CFLAGS)
	
_cardtesthelper_councilroom.o: _cardtesthelper_councilroom.c \
							   _cardtesthelper_councilroom.h dominion.o 
	gcc -c _cardtesthelper_councilroom.c $(CFLAGS)
	
_randomtesthelper_councilroom.o: _randomtesthelper_councilroom.c \
								 _randomtesthelper_councilroom.h \
								 dominion.o
	gcc -c _randomtesthelper_councilroom.c $(CFLAGS)
	
_randomtesthelper_adventurer.o: _randomtesthelper_adventurer.c \
								_randomtesthelper_adventurer.h \
								dominion.o
	gcc -c _randomtesthelper_adventurer.c $(CFLAGS)
	
playdom: dominion.o playdom.c
	gcc -o playdom playdom.c dominion.o rngs.o $(CFLAGS)
#To run playdom you need to enter: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard testDrawCard.c dominion.o rngs.o $(CFLAGS)
	
cardtest_smithy: cardtest_smithy.c _cardtesthelper_smithy.o \
				 dominion.o rngs.o
	gcc -o $@ $^ $(CFLAGS)
	
cardtest_adventurer: cardtest_adventurer.c _cardtesthelper_adventurer.o \
					 dominion.o rngs.o
	gcc -o $@ $^ $(CFLAGS)
	
	
cardtest_councilroom: cardtest_councilroom.c _cardtesthelper_councilroom.o \
					  dominion.o rngs.o
	gcc -o $@ $^ $(CFLAGS)
	
randomtest_councilroom: randomtest_councilroom.c \
						_randomtesthelper_councilroom.o \
						dominion.o rngs.o
	gcc -o $@ $^ $(CFLAGS)
	
randomtest_adventurer: randomtest_adventurer.c \
					  _randomtesthelper_adventurer.o \
					  dominion.o rngs.o
	gcc -o $@ $^ $(CFLAGS)
	
testAll: dominion.o testSuite.c 
	gcc -o testSuite testSuite.c dominion.o rngs.o $(CFLAGS)
	
testresults: cardtest_smithy cardtest_adventurer cardtest_councilroom \
			 randomtest_councilroom randomtest_adventurer	
	@chmod +x testmakehelper
	@./testmakehelper
	@chmod +x randomtestmakehelper
	@./randomtestmakehelper randomtest_councilroom 1 
	@./randomtestmakehelper randomtest_adventurer 2

interface.o: interface.h interface.c
	gcc -c interface.c $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno \
	*.so *.out testDrawCard testDrawCard.exe \
	cardtest_smithy cardtest_adventurer cardtest3 cardtest_councilroom \
	randomtest_councilroom randomtest_adventurer
	
# References
# https://stackoverflow.com/questions/3824500/compiling-c-on-remote-linux-machine-clock-skew-detected-warning
# https://stackoverflow.com/questions/10520623/how-to-split-one-string-into-multiple-variables-in-bash-shell
# https://stackoverflow.com/questions/4678967/makefile-command-substitution-problem/4679166#4679166

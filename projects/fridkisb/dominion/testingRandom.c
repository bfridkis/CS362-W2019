#include "rngs.h"
#include <math.h>
#include <stdio.h>

int main(){
	int random = floor(Random() * 27);
	while(1){printf("\n%d", random); random = floor(Random() * 27);}
	return 0;
}
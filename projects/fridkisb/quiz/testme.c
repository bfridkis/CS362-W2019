#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	//Randomly return any ascii character in range 32-127
    return (char)((rand() % 96) + 32);
}

char *inputString()
{
	//Randomly return any string in the set of all
	//possible 5-character strings containing the 
	//characters 'r', 'e', 's', 'e', 't' (in any
	//frequency and in any of the possible 5 character 
	//positions.)
	static char randStr[6];
    int randChar, i = 0;
	for(i = 0; i < 5; i++){
		randChar = rand() % 5;
		if(randChar == 0)
			randStr[i] = 'r';
		else if(randChar == 1 || randChar == 2)
			randStr[i] = 'e';
		else if(randChar == 3)
			randStr[i] = 's';
		else
			randStr[i] = 't';
	}
	randStr[6] = '\0';
	return randStr;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' ' && state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}

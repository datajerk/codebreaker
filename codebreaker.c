/*

Codebreaker, Version 1.0, Sun May 13 13:49:18 GMT 2012

Copyright (c) 2012 All Rights Reserved, Egan Ford (egan@sense.net)

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

License:
	*  Public Domain.
	*  Do what you like, remember to credit all sources when using.

*/

#include <conio.h>

#define TIME ((count*59)/959790)
#define TENTHS ((count*59)/95979)
#define DEBUG 0
#define TOP 0
#define MIDDLE 1
#define BOTTOM 2
#define NOVICE 0
#define PRO 1
#define MASTER 2
#define TIMER 600
#define srand(x) next = (x)

extern void getkey();
extern void __fastcall__ putchar(char);
extern unsigned long count;
extern unsigned char key;

void drawboard(unsigned char);
void clearscreen();
void printnum(unsigned long, unsigned char, unsigned char);
void instructions();
int rand();
//void srand(unsigned int);

const char *title = "CODEBREAKER!\r\r\"CRACKED\" BY EGAN FORD\r\r\r\r";
const char *novicepro = "WOULD YOU LIKE TO PLAY AS A\r\r[N]OVICE, [P]RO, OR [M]ASTER? ";
const char *insprompt = "DO YOU NEED INSTRUCTIONS? ";

const char *instruct1 = "\
GOAL:  CRACK THE CODE.  FAILURE IS NOT\r\
       AN OPTION!\r\
\r\
CODE:  THE CODE IS COMPOSED OF ";

const char *instruct2 = " COLORS\r\
       OUT OF A SET OF ";

const char *instruct3 = ".  DUPLICATES\r\
       ALLOWED.\r\
\r\
RULES: YOU HAVE ";

const char *instruct4 = " ATTEMPTS.  AFTER EACH\
       ATTEMPT A CLUE WILL BE GIVEN.\r\
\r\
CLUES: AN * INDICATES THAT YOU HAVE A\r\
       CORRECT COLOR IN THE CORRECT\r\
       PLACE.  A + INDICATES THAT YOU\r\
       HAVE A CORRECT COLOR.  YOU\r\
       NEED ";

const char *instruct5 = " *'S TO WIN.  THE\r\
       POSITION OF THE CLUES IS NOT\r\
       RELEVANT.\r\
\r\
SCORE: TIME IN SECONDS.  GOOD LUCK!\r\
\r\
\r\
";
const char *instructg =
	"\rA PRO GAME WITH A TWIST...\r\r\r\r       YOU ONLY HAVE 60 SECONDS!\r\r\r\r";
const char *anykey = "PRESS ANY KEY TO BEGIN CRACKING ";
const char *header = "\
              CODEBREAKER!\r\
\r\
";
const char *won = "     YOU'VE CRACKED IT!\r";
const char *fail = "       YOU'VE FAILED\r";
const char *playagain = "WOULD YOU LIKE TO PLAY AGAIN? ";

unsigned long int next = 1;
char numcolors, slots, attempts, tt;

int main(void)
{
	char code[5], claimed[5], guess[5];
	char colors[] = { 'R', 'G', 'B', 'Y', 'O', 'P', 'S', 'C' };
	char *colorkey[] =
		{ " [R]ED    ", " [G]REEN  ", " [B]LUE   ", " [Y]ELLOW ",
		" [O]RANGE ", " [P]URPLE ", " [S]ILVER ", " [C]OPPER "
	};
	char *gametypes[] = { "NOVICE", "PRO   ", "MASTER" };
	char i, j, k, right, close, gametype;
	signed int timer;
	unsigned int win[3], loss[3], avgtime[3], besttime[3];

	for(i = 0; i < 3; ++i)
		win[i] = loss[i] = avgtime[i] = besttime[i] = 0;

	clearscreen();
	cputs(title);

	while (1) {
		tt = 0;
		numcolors = 8;
		slots = 5;
		attempts = 12;
		gametype = PRO;
		cputs(novicepro);
		key = 0;
		while (key != 'N' && key != 'P' && key != 'M')
			getkey();
		putchar(key);
		cputs("\r\r");
		if (key == 'N') {
			numcolors = 6;
			slots = 4;
			attempts = 10;
			gametype = NOVICE;
		}
		if (key == 'M') {
			tt = 1;
			gametype = MASTER;
		}
		instructions();
		clearscreen();

		timer = TIMER;
		srand(count & 0xFFFF);
		for (i = 0; i < slots; ++i) {
			code[i] = rand() % numcolors;
		}

		clearscreen();
		cputs(header);
		if (slots == 5) {
			for (i = 0; i < numcolors; ++i)
				cputs(colorkey[i]);
			cputs("\r");
		} else {
			cputs("     ");
			for (i = 0; i < numcolors / 2; ++i)
				cputs(colorkey[i]);
			cputs("\r     ");
			for (i = numcolors / 2; i < numcolors; ++i)
				cputs(colorkey[i]);
			cputs("\r\r");
		}

		drawboard(TOP);
		cputs("   ! ");
		for (i = 0; i < slots; i++) {
#if DEBUG==1
			putchar(colors[code[i]]);
#else
			putchar('X');
#endif
			putchar(' ');
		}
		cputs("! CLUE");
		if (slots == 5)
			cputs("S");
		if (tt)
			cputs("  TIME: -60.0");
		cputs("\r");
		drawboard(MIDDLE);

		for (i = 1; i <= attempts; ++i) {
			for (j = 0; j < slots; ++j)
				claimed[j] = 0;
			right = close = 0;
			printnum(i, 2, ' ');
			cputs(" ! ");
			if(i == 1)
				count = 0;
			for (j = 0; j < slots; ++j) {
				getkey();
				if (tt) {
					timer -= TENTHS;
					count = 0;
					if (timer < 0) {
						timer = 0;
						break;
					}
				}
				for (k = 0; k < numcolors; ++k)
					if (key == colors[k])
						break;
				if (k == numcolors) {
					j--;
					continue;
				}
				putchar(colors[k]);
				putchar(' ');
				if (k == code[j]) {
					right++;
					claimed[j] = 1;
				}
				guess[j] = k;
			}
			for (; j < slots; ++j) {
				cputs("- ");
				guess[j] = 9;
			}
			cputs("! ");
			for (j = 0; j < slots; ++j) {
				if (guess[j] == code[j])
					continue;
				for (k = 0; k < slots; ++k)
					if (guess[j] == code[k] && !claimed[k]) {
						close++;
						claimed[k] = 1;
						break;
					}
			}

			for (j = 0; j < right; ++j)
				putchar('*');
			for (j = 0; j < slots - right - close; ++j)
				putchar(' ');
			for (j = 0; j < close; ++j)
				putchar('+');

			if (tt) {
				cputs("  TIME: -");
				printnum(timer / 10, 2, '0');
				putchar('.');
				printnum(timer % 10, 1, '0');
				putchar(0xd);
			} else {
				if (slots == 4)
					cputs("  ");
				cputs("  TIME: ");
				printnum(TIME / 60, 2, '0');
				putchar(':');
				printnum(TIME % 60, 2, '0');
				putchar(0xd);
			}

			if (right == slots || timer == 0)
				break;
		}

		drawboard(MIDDLE);
		cputs("   ! ");
		for (i = 0; i < slots; i++) {
			putchar(colors[code[i]]);
			putchar(' ');
		}
		putchar('!');
		if (slots == 4)
			cputs(" ");
		if (right == slots) {
			cputs(won);
			if(gametype == MASTER)
				timer = TIMER - timer;
			else
				timer = TIME;
			win[gametype] += 1;
			if(win[gametype] == 1)
				avgtime[gametype] = besttime[gametype] = timer;
			else {
				avgtime[gametype] = (avgtime[gametype]*(win[gametype] - 1)+timer)/win[gametype];
				if(timer < besttime[gametype])
					besttime[gametype] = timer;
			}
		}
		else {
			cputs(fail);
			loss[gametype] += 1;
		}
		drawboard(BOTTOM);
		putchar(0xd);

		cputs(playagain);
		key = 0;
		while (key != 'N' && key != 'Y')
			getkey();
		putchar(key);
		clearscreen();
		if (key == 'Y')
			continue;

		cputs("STATS\r\r");
		cputs("                         -----TIMES-----");
		cputs("GAME    CRACKED  FAILED  AVERAGE    BEST\r");
		for(i = 0; i < 3; ++i) {
			if(win[i] + loss[i] == 0)
				continue;
			cputs(gametypes[i]);
			cputs("     ");
			printnum(win[i],4,' ');
			cputs("    ");
			printnum(loss[i],4,' ');
			cputs("    ");
			if(avgtime[i] == 0) {
				cputs("-----   -----");
				continue;
			}
			if(i < 2) {
				printnum(avgtime[i] / 60, 2, '0');
				putchar(':');
				printnum(avgtime[i] % 60, 2, '0');
				cputs("   ");
				printnum(besttime[i] / 60, 2, '0');
				putchar(':');
				printnum(besttime[i] % 60, 2, '0');
			}
			else {
				putchar(' ');
				printnum(avgtime[i] / 10, 2, ' ');
				putchar('.');
				printnum(avgtime[i] % 10, 1, '0');
				cputs("    ");
				printnum(besttime[i] / 10, 2, ' ');
				putchar('.');
				printnum(besttime[i] % 10, 1, '0');
			}
		}
		cputs("\r\r\r\rTO PLAY AGAIN TYPE:  280R\r\r");
		break;
	}

	return 0;
}

#if CODESEG == 1
#pragma codeseg ("CODE1")
#endif

void drawboard(unsigned char p)
{
	char i, ends[] = { '.', ':', '''};

	cputs("   ");
	putchar(ends[p]);
	for(i=0;i<slots*2+1;++i)
		putchar('-');
	putchar(ends[p]);
	putchar(0xd);
}

void clearscreen()
{
	char i=25;

	while(--i)
		putchar(0xd);
}

void printnum(unsigned long n, char padding, char pad)
{
	char i=8, c[8];

	while(n > 0) {
		c[--i]=(n%10 + 0x30);
		n/=10;
	}
	if(padding > 0) {
		while(i > 0)
			c[--i]=pad;
		for(i=8-padding;i<8;++i)
			putchar(c[i]);
		return;
	}
	for(;i<8;++i)
		putchar(c[i]);
}

void instructions()
{
	cputs(insprompt);
	key = 0;
	while (key != 'N' && key != 'Y')
		getkey();
	putchar(key);
	if(key == 'N')
		return;
	clearscreen();
	if(slots == 5)
		if(tt == 0)
			cputs("PRO");
		else
			cputs("MASTER");
	else
		cputs("NOVICE");
	cputs(" INSTRUCTIONS\r\r");
	if(tt == 0) {
		cputs(instruct1);
		printnum(slots,0,'0');
		cputs(instruct2);
		printnum(numcolors,0,'0');
		cputs(instruct3);
		printnum(attempts,0,'0');
		cputs(instruct4);
		printnum(slots,0,'0');
		cputs(instruct5);
	}
	else
		cputs(instructg);
	cputs(anykey);
	getkey();
}

int rand()
{
	next = next * 1103515245 + 12345;
	return (unsigned int)(next/65536UL) % 32768UL;
}

/*
void srand(unsigned int seed)
{
	next = seed;
}
*/



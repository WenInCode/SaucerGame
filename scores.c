#include	<curses.h>
#include 	<stdio.h>

#include	"locks.h"
#include 	"scores.h"

#define	BUFFLEN 256

void displayInfo () {
	char info[BUFFLEN];
	sprintf(info, "Press 'Q' to quit | ',' & '.' to move"
		" left & right | ESCAPED: %d | ROCKETS: %d", 
		noEscaped, rocketsLeft);

	pthread_mutex_lock(&mx);
	move(LINES - 1, 0);
	addstr(info);
	move(LINES-1, COLS-1);
	refresh();
	pthread_mutex_unlock(&mx);
}

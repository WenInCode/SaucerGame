#include <curses.h>
#include <stdio.h>
#include <string.h>

#include "locks.h"
#include "rockets.h"


static void 	initRocket(struct rocket *, int *);
static void 	animateRocket(struct rocket *);

void setRocketsToDead(struct rocket **) {
	int i;
	for (i = 0; i < MAX_ROCKETS; i++)
		rockets[i].isAlive = 0;
}

void *setupRocket(void *arg) {
	int i;
	int * col = arg;	/* the current column of the cannon */
	
	/*
	 * Select the first available rocket
	 */
	for (i = 0; i < MAX_ROCKETS; i++) {
		if (rockets[i].isAlive == 0) {
			initRocket(&rockets[i], col);
			animateRocket(&rockets[i]);
			break;
		}
	}
}

void initRocket(struct rocket *r, int *col) {
	strncpy(r->message, ROCKET, ROCKET_LEN);
	r->length = ROCKET_LEN;
	r->row = LINES - 3;
	r->col = *col;
	r->delay = 5;
	r->dir = -1; 		/* moving up! */
	r->isAlive = 1;
}

void animateRocket(struct rocket *r) {
	int len = r->length;

	/*
	 * Draw the rocket initially
	 */
	pthread_mutex_lock(&mx);
	move(r->row, r->col);
	addstr(r->message);
	move(LINES-1, COLS-1);
	refresh();
	pthread_mutex_unlock(&mx);
	
	while (1) {
		/*
		 * Now erase the previous and draw the next
		 */
		usleep(r->delay*TUNIT);

		pthread_mutex_lock(&mx);
		move(r->row, r->col);
		addch(' ');		/* replace the last rocket */
		r->row += r->dir;
		move(r->row, r->col);
		addstr(r->message);
		move(LINES-1, COLS-1);
		refresh();
		pthread_mutex_unlock(&mx);

		if (r->row < 0) {
			break;
		}
	}
	r->isAlive = 0;
	pthread_exit(NULL);
}

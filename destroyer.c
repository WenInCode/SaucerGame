#include <string.h>
#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <time.h>
#include "locks.h"
#include "destroyer.h"
#include "destroyerrocket.h"
#include "scores.h"

#define TUNIT 20000		/* time units in microseconds */

pthread_t destRocketThread;
extern struct destroyer destShip;

static void 	initDestroyer(int, int);
static void 	animateDestroyer();
/*
 * Only send the destroyer if it is not already
 * present.
 */
void *sendDestroyer() {
	srand(getpid());

	while(1) {
		if (!destShip.isAlive) {
			sleep(10 + (rand()%20));
		
			// initialize the destroyer
			initDestroyer(rand()%15, 1+(rand()%10));
			pthread_create(&destRocketThread, NULL, shootDestRocket, &destShip);
			animateDestroyer();
		}	
	}
}

void initDestroyer(int row, int delay) {
	strncpy(destShip.topMessage, DESTROYER_TOP, DESTROYER_LEN);
	strncpy(destShip.botMessage, DESTROYER_BOT, DESTROYER_LEN);
	destShip.length = DESTROYER_LEN;
	destShip.row = row;
	destShip.col = 0;
	destShip.dir = 1;
	destShip.delay = delay;
	destShip.hit = 0;
	destShip.isAlive = 1;
}

void animateDestroyer() {
	int i;

	while (1) {
		usleep(destShip.delay*TUNIT);
		
		pthread_mutex_lock(&mx);
		move(destShip.row, destShip.col);
		addch(' ');
		addnstr(destShip.topMessage, DESTROYER_LEN);
		addch(' ');
		move(LINES-1, COLS-1);
		refresh();

		move(destShip.row + 1, destShip.col);
		addch(' ');
		addnstr(destShip.botMessage, DESTROYER_LEN);
		addch(' ');
		move(LINES-1, COLS-1);
		refresh();

		pthread_mutex_unlock(&mx);
		
		/*
		 * perform checks;
		 */
		if ((destShip.col + destShip.length + 2) >= COLS && destShip.dir == 1)
			destShip.dir = -1;

		if (destShip.col <= 0 && destShip.dir == -1) 
			destShip.dir = 1;
		
		if (destShip.hit > 100) {
			pthread_mutex_lock(&mx);
			move(destShip.row, destShip.col);
			for (i=0; i <= destShip.length; i++)
				addch(' ');
			move(destShip.row + 1, destShip.col);
			for (i=0; i <= destShip.length; i++)
				addch(' ');
			move(LINES-1, COLS-1);
			refresh();
			pthread_mutex_unlock(&mx);
			break;
		}
		destShip.col += destShip.dir;
	}

	destShip.isAlive = 0;
	
	pthread_cancel(destRocketThread);
	pthread_exit(NULL);
}

#include <curses.h>
#include <stdio.h>

#include "locks.h"
#include "rocket.h"

static struct rocket rockets[MAX_ROCKETS];

void setRocketsToDead() {
	int i;
	for (i = 0; i < MAX_ROCKETS; i++)
		rockets.isAlive = 0;
}

void setupRocket(void *arg) {
	int * col = arg;	/* the current column of the cannon */
	
	/*
	 * Select the first available rocket
	 */
	for (i = 0; i < MAX_ROCKETS; i++) {
	}
}
}

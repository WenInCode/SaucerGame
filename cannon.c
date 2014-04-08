#include 	<curses.h>
#include 	<stdio.h>
#include	<string.h>

#include 	"cannon.h"
#include	"locks.h"

#define TUNIT 20000

static struct cannon	cn;

void setupCannon() {
	int col = ((0 + COLS)/2);

	strncpy(cn.message, CANNON, CANNON_LEN);
	cn.length = CANNON_LEN;
	cn.col = col;
	cn.hit = 0;
	cn.delay = 1;
}

int getCannonCol() {
	return cn.col;
}

int getCannonHit() {
	return cn.hit;
}

void setCannonHit() {
	cn.hit = 1;
}

void moveCannon(int dir) {
	pthread_mutex_lock(&mx);
	move(LINES-2, cn.col);
	delch();
	move(LINES-1, COLS-1);
	pthread_mutex_unlock(&mx);

	cn.col += dir;
	if (cn.col >= COLS || cn.col <= 0)
		cn.col -= dir;
	displayCannon();
}

void displayCannon() {
	usleep(cn.delay*TUNIT);
	
	pthread_mutex_lock(&mx);
	move(LINES-2, cn.col);
	//delch();
	addstr(cn.message);
	move(LINES-1, COLS-1);
	refresh();
	pthread_mutex_unlock(&mx);
}

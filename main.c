/*
 * tanimate.c: animate several strings using threads, curses, usleep()
 *
 *	bigidea one thread for each animated string
 *		one thread for keyboard control
 *		shared variables for communication
 *	compile	cc tanimate.c -lcurses -lpthread -o tanimate
 *	to do   needs locks for shared variables
 *	        nice to put screen handling in its own thread
 */

#include	<stdio.h>
#include	<curses.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>

#include	"cannon.h"
#include 	"locks.h"
#include	"rockets.h"
#include 	"saucer.h"
#include 	"scores.h"

#define	MAXMSG	10		/* limit to number of strings	*/
#define MAX_ROCKETS 200		
#define	TUNIT   20000		/* timeunits in microseconds */

static void 	setup();

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

int 	noEscaped = 0;
int	rocketsLeft = MAX_ROCKETS;

int main(int ac, char *av[])
{
	int	       	c;		/* user input		*/
	pthread_t	saucerSetup;
	pthread_t	rocketThreads[MAX_ROCKETS];
	struct rocket 	rockets[MAX_ROCKETS];
	void	      	*animate();	/* the function		*/
	int	       	num_msg ;	/* number of strings	*/
	int	     	i;
	int		col;

	setup();
	setupCannon();
	setRocketsToDead(rockets);
	displayCannon();
	displayInfo();
	
	if (pthread_create(&saucerSetup, NULL, setupSaucer, NULL)) {
		endwin();
		exit(0);
	} 	

	/* process user input */
	while(1) {
		c = getch();

		if ( c == 'Q' ){
			 break;
		} else if (c == ',') {
			moveCannon(-1);
		} else if (c == '.') {
			moveCannon(1);
		} else if (c == ' ') {
			col = getCannonCol();
		
			if (rocketsLeft > 0) {	
				rocketsLeft -= 1;
				/*	
				pthread_mutex_lock(&mx);
				pthread_mutex_unlock(&mx);
				*/	
				displayInfo();
				for (i = 0; i < MAX_ROCKETS; i++) {
					if (rockets[i].isAlive == 0) {
						rockets[i].col = col;
						pthread_create(&rocketThreads[i], NULL, setupRocket, &rockets[i]);	
						break;
					}
				}
			}
		}	
	}

	/* cancel all the threads */
	pthread_mutex_lock(&mx);
	//for (i=0; i<num_msg; i++ )
	//	pthread_cancel(thrds[i]);
	endwin();
	return 0;
}

void setup()
{
	//int num_msg = ( nstrings > MAXMSG ? MAXMSG : nstrings );
	int num_msg = 1;
	int i;

	/* set up curses */
	initscr();
	crmode();
	noecho();
	clear();
	mvprintw(LINES-1,0,"'Q' to quit");
}

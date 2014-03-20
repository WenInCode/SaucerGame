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
#include 	"saucer.h"
#include 	"scores.h"

#define	MAXMSG	10		/* limit to number of strings	*/
#define	TUNIT   20000		/* timeunits in microseconds */

static void 	setup();

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

int 	noEscaped = 0;
int	rocketsLeft = STARTING_ROCKETS;

int main(int ac, char *av[])
{
	int	       c;		/* user input		*/
	pthread_t	saucerSetup;
	void	       *animate();	/* the function		*/
	int	       num_msg ;	/* number of strings	*/
	int	     i;

	setup();
	setupCannon();
	displayCannon();
	displayInfo();
	
	if (pthread_create(&saucerSetup, NULL, setupSaucer, NULL)) {
		endwin();
		exit(0);
	} 	

	/* process user input */
	while(1) {
		c = getch();
		/*switch (c) {
		case 'Q':
			break;
		case ',':
			moveCannon(-1);
			break;
		case '.':
			moveCannon(1);
			break;
		default:
			break;
		}*/
		if ( c == 'Q' ){
			 break;
		} else if (c == ',') {
			moveCannon(-1);
		} else if (c == '.') {
			moveCannon(1);
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

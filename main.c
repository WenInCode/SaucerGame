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
#define ENDGAME 15		/* end game condition */
#define	TUNIT   20000		/* timeunits in microseconds */

static void 	setup();
static void	shootRocket();
static void 	*collisionDetection();
static void 	compareCoords(int i, int j); 

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

pthread_t	rocketThreads[MAX_ROCKETS];
struct rocket 	rockets[MAX_ROCKETS];

pthread_t	sThread[MAX_SAUCERS];
struct saucer saucers[MAX_SAUCERS];

int 	noEscaped = 0;
int	rocketsLeft = MAX_ROCKETS;

int main(int ac, char *av[])
{
	int	       	c, endGame;		/* user input		*/
	pthread_t	saucerSetup;
	pthread_t	collisionThread;
	void	      	*animate();	/* the function		*/
	int	       	num_msg;	/* number of strings	*/
	
	endGame = 0;

	setup();
	setupCannon();
	setRocketsToDead(rockets);
	displayCannon();
	displayInfo();
	
	if (pthread_create(&saucerSetup, NULL, setupSaucer, NULL)) {
		endwin();
		exit(0);
	} 	
	pthread_create(&collisionThread, NULL, collisionDetection, NULL);

	/* process user input */
	while(1) {
		c = getch();
		if (noEscaped >= ENDGAME) {
			endGame = 1;
		}	

		if ( c == 'Q' ){
			 break;
		} else if (c == ',' && !endGame) {
			moveCannon(-1);
		} else if (c == '.' && !endGame) {
			moveCannon(1);
		} else if (c == ' ' && !endGame) {
			shootRocket();	
		}	
	}

	/* cancel all the threads */
	pthread_mutex_lock(&mx);
	//for (i=0; i<num_msg; i++ )
	//	pthread_cancel(thrds[i]);
	endwin();
	return 0;
}

void shootRocket() {
	int 	i;
	int 	col = getCannonCol();
		
	if (rocketsLeft > 0) {	
		rocketsLeft -= 1;
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

void *setupSaucer() {
	int num_msg = 1;
	int i;

	/* assign rows and velocities to each string */
	srand(getpid());

	/*
	 * initliaze the saucers
	 */
	for (i = 0; i < MAX_SAUCERS; i++) {
		saucers[i].isAlive = 0;
	}

	while (1) {

		for(i=0 ; i<MAX_SAUCERS; i++){
			if (saucers[i].isAlive == 0) {
				initSaucer(&saucers[i], rand()%15, 1+(rand()%15));
				pthread_create(&sThread[i], NULL, animateSaucer, &saucers[i]);
				break;
			} 
		}

		sleep(1+(rand()%15));
	}
}

void *collisionDetection() {
	int i, j;

	while (1) {
		for (i = 0; i < MAX_SAUCERS; i++) {
			for (j = 0; j < MAX_ROCKETS; j++) {
				compareCoords(i, j);	
			}
		} 
	}
}

void compareCoords(int i, int j) {
	/*
	 * Check here if any coords match
	 */ 
	if (saucers[i].isAlive == 1 
		&& rockets[j].isAlive == 1) {
		if ((saucers[i].row == rockets[j].row 
		    || (saucers[i].row + 1) == rockets[j].row)
		    && rockets[j].col > saucers[i].col
		    && rockets[j].col <= (saucers[i].col + saucers[i].length)) {
			/*
			 * hit
		 	 */
			saucers[i].hit = 1;
			rockets[j].hit = 1;
		}
	}
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

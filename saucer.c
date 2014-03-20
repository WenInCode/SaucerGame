#include 	"locks.h"
#include 	"saucer.h"
#include 	"scores.h"

#define TUNIT 20000		/* time units in microseconds */

pthread_mutex_t	escaped = PTHREAD_MUTEX_INITIALIZER; 

static void	initSaucer(struct saucer *, int, int);

static pthread_t	sThread[MAX_SAUCERS];
static struct saucer saucers[MAX_SAUCERS];

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

/**
 * initialize all the saucers with 
 * - message
 * - length
 * - speed
 * - isAlive = 0 (dead)
 */
void initSaucer(struct saucer *ship, int col, int delay) {
	strncpy(ship->message, SAUCER, SAUCER_LEN);
	ship->length = SAUCER_LEN;
	ship->row = col;
	ship->delay = delay;
	ship->isAlive = 1;
}

void *animateSaucer(void *arg) {
	struct saucer *ship = arg;
	int len = ship->length+2;
	int col = 0;	/* might need to review this */

	while (1) {
		usleep(ship->delay*TUNIT);
	
		pthread_mutex_lock(&mx);	
		move(ship->row, col);
		addch(' ');
		addstr(ship->message);
		addch(' ');
		move(LINES-1, COLS-1);
		refresh();	
		pthread_mutex_unlock(&mx);

		col += 1;

		/*
		 * CODE REFERENCED FROM JAMES HODGSON
		 */
		if (col + len >= COLS) {
			ship->message[len-1] = '\0';
			len = len-1;
		}

		if (col >= COLS) {
			pthread_mutex_lock(&escaped);
			noEscaped++;	
			pthread_mutex_unlock(&escaped);
			displayInfo();
			break;
		}
	}
	
	ship->isAlive = 0;
	pthread_exit(NULL);
}

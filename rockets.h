#ifndef _ROCKETS_H_
#define _ROCKETS_H_
#include <stdlib.h>

#define ROCKET "^"
#define ROCKET_LEN 1
#define MAX_ROCKETS 200
#define TUNIT 20000 

struct rocket {
	char 	message[ROCKET_LEN];	/* rocket string */
	int 	length;			/* rocket length */
	int 	row;			/* current row */
	int 	col;			/* current column */
	int	delay;
	int	dir;
	int 	isAlive;		/* is the rocket still on screen */
};

void 	setRocketsToDead();
void 	*setupRocket(void *);

#endif

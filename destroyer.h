#ifndef _DESTROYER_H_ 
#define _DESTROYER_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <time.h>

#define DESTROYER_TOP "  -------^-------  "
#define DESTROYER_BOT "<[_______________]>"
#define DESTROYER_LEN 19
#define MAX_DESTROYERS 1

struct	destroyer {
	char	message[SAUCER_LEN];	/* the message */
	int 	length;		/* the saucer length */
	int	row;			/* the row     */
	int	col;
	int	delay;  		/* delay in time units */
	int	hit;
	int	isAlive;		/* +1 or -1	*/
};

#endif

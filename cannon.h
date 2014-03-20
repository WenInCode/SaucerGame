#ifndef _CANNON_H_
#define _CANNON_H_
#include <stdlib.h>

#define CANNON "|"
#define CANNON_LEN 1

struct	cannon {
	char	message[CANNON_LEN];
	int	length;
	int	col;
	int	delay;
};

void	setupCannon();
int 	getCannonCol();
void	moveCannon(int);
void	displayCannon();

#endif

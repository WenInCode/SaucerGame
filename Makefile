CC := cc
FLAGS := -lcurses -lpthread

game:		saucer.o main.o
	$(CC) main.o saucer.o $(FLAGS) -o game

saucer.o:	locks.h saucer.h saucer.c
	$(CC) -c saucer.c $(FLAGS) -o saucer.o

main.o: 	locks.h saucer.h main.c
	$(CC) -c main.c $(FLAGS) -o main.o	

clean:
	rm -rf *.o game

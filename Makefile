CC := cc
FLAGS := -lcurses -lpthread

game:		scores.o saucer.o rockets.o cannon.o main.o
	$(CC) main.o cannon.o rockets.o saucer.o scores.o $(FLAGS) -o game

scores.o:	locks.h scores.h scores.c
	$(CC) -c scores.c $(FLAGS) -o scores.o

rockets.o:	locks.h rockets.h rockets.c
	$(CC) -c rockets.c $(FLAGS) -o rockets.o

cannon.o:	locks.h cannon.h cannon.c
	$(CC) -c cannon.c $(FLAGS) -o cannon.o

saucer.o:	locks.h saucer.h scores.h saucer.c
	$(CC) -c saucer.c $(FLAGS) -o saucer.o

main.o: 	locks.h cannon.h saucer.h scores.h main.c
	$(CC) -c main.c $(FLAGS) -o main.o	

clean:
	rm -rf *.o game

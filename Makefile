CC=gcc
CFLAGS=-lSDL2
DEPS = game.h
OBJ = main.o game.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

main: main.o game.o
	$(CC) -g -o main main.o game.o $(CFLAGS)

game: game.o
	$(CC) -g -o game game.o $(CFLAGS)

all: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf ./*.o; rm -rf ./*.out; rm ./main; rm ./game; rm ./all

CC=gcc
CFLAGS="-Wall" `pkg-config sdl2 SDL2_ttf --libs --cflags`
BIN=sdl_game1

debug:clean
	$(CC) $(CFLAGS) -g -o ${BIN} main.c 
build:clean
	$(CC) $(CFLAGS) -o ${BIN} main.c
clean:
	rm -vfr *~ ${BIN}
run:build
	./${BIN}

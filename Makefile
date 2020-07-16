CC=gcc
CFLAGS="-Wall"

debug:clean
	$(CC) $(CFLAGS) -g -o sdl_game1 main.c `pkg-config sdl2 SDL2_ttf --libs --cflags`
stable:clean
	$(CC) $(CFLAGS) -o sdl_game1 main.c `pkg-config sdl2 SDL2_ttf --libs --cflags`
clean:
	rm -vfr *~ sdl_game1
run:stable
	./sdl_game1

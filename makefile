


main: rainbowFactory.c display.c rainbowImageGen.h display.h
#	gcc -o main display.c `sdl2-config --cflags --libs`
	gcc -o main rainbowFactory.c display.c `sdl2-config --cflags --libs`
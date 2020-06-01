
RBHEADERS = $(addprefix src/,RB_AssignmentQueue.h RB_BasicTypes.h RB_ColorPool.h RB_Main.h RB_Pixel.h RB_PixelMap.h RB_Display.h) 
IMPLEMENTATIONS = $(addprefix src/,basicAssignmentQueue.c basicColorPool.c basicPixelMap.c display.c rainbowMain.c basicTypes.c)

main: $(RBHEADERS) $(IMPLEMENTATIONS) src/main.c
	gcc -o main src/main.c $(IMPLEMENTATIONS) `sdl2-config --cflags --libs`

# main: rainbowFactory.c display.c rainbowImageGen.h display.h
# #	gcc -o main display.c `sdl2-config --cflags --libs`
# 	gcc -o main rainbowFactory.c display.c `sdl2-config --cflags --libs`
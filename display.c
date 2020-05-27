#include <SDL2/SDL.h>
#include <stdio.h>
#include "rainbowImageGen.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

int initializeDisplay() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error initializing SDL!: %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("Rainbow", 0, 0, 512, 512, 0);

	if(window == NULL) {
		fprintf(stderr, "Error creating window!: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	//SDL_RenderSetScale(renderer, 8, 8);

	if(renderer == NULL) {
		fprintf(stderr, "Error creating renderer!: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 512, 512);

	if(texture == NULL) {
		fprintf(stderr, "Error creating texture!: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_Event e;
	SDL_PollEvent(&e);

	return 0;
}

void displayQuit() {
	printf("Quitting display\n");
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int handleEvents() {
	SDL_Event e;
	while(SDL_PollEvent(&e) == 1) {
		if(e.type == SDL_QUIT) {
			displayQuit();
			return 0;
		}
	}

	return 1;
}

void displayAddPixel(Color c, Coord loc) {
	if(c.r == c.g && c.g == c.b) {
		printf("Setting Coord(%d, %d) to \tColor(%u, %u, %u)\n", loc.x, loc.y, c.r, c.g, c.b);
	}
	
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawPoint(renderer, loc.x, loc.y);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	handleEvents();
}




#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <signal.h>
#include <cstdio>
#include "defer.h"
#include "server.h"

CServer gServer;

void onSignal(int) {
	gServer.Stop();
}

int main(int, char**) {
	if (SDL_Init(0) < 0) {
		printf("SDL_Init: %s\n", SDL_GetError());
		return 1;
	}
	defer(SDL_Quit);

	if (SDLNet_Init() < 0) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return 2;
	}
	defer(SDLNet_Quit);

	// Capture Ctrl+C and Terminate
	signal(SIGINT, onSignal);
	signal(SIGTERM, onSignal);

	// This function is expected to be blocking until Stop() is called.
	// This happens when you hit Ctrl+C.
	gServer.Start(9000);

	return 0;
}
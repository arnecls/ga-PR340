#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstdio>
#include <varargs.h>
#include "defer.h"
#include "storage.h"
#include <windows_sane.h>

// Use this client to test your server.
// It will print results of the different tests on the command line as well as
// on visual studio's debug output.

void println(const char* apFormat, ...) {
	char buffer[2048] = { 0 };
	va_list args;
	va_start(args, apFormat);
	vsnprintf_s(buffer, 2048, apFormat, args);
	va_end(args);

	printf("%s\n", buffer);
#ifdef _DEBUG
	OutputDebugStringA(buffer);
#endif
}

TCPsocket openConnection() {
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, "localhost", 9000) < 0) {
		println("SDLNet_ResolveHost: %s", SDLNet_GetError());
		return nullptr;
	}
	
	// Open connection
	auto socket = SDLNet_TCP_Open(&ip);
	if (!socket) {
		println("SDLNet_TCP_Open: %s", SDLNet_GetError());
		return nullptr;
	}

	return socket;
}

void sendCommand(TCPsocket& arSocket, const std::string& aCmd) {
	char buffer[2048] = { 0 };
	SDLNet_TCP_Send(arSocket, aCmd.c_str(), aCmd.size());
	SDLNet_TCP_Recv(arSocket, buffer, 2048);
	println("Set returned: %s", buffer);
}

int main(int, char**) {
	if (SDL_Init(0) < 0) {
		println("SDL_Init: %s", SDL_GetError());
		return 1;
	}
	defer(SDL_Quit);

	if (SDLNet_Init() < 0) {
		println("SDLNet_Init: %s", SDLNet_GetError());
		return 2;
	}
	defer(SDLNet_Quit);

	auto socket = openConnection();
	if (socket == nullptr) {
		return 3;
	}
	defer([socket]() { 
		SDLNet_TCP_Close(socket); 
	});

	sendCommand(socket, "SET foo bar\n");
	sendCommand(socket, "SET bar \"I love the smell of pointers in the morning\"\n");
	sendCommand(socket, "GET foo\n");
	sendCommand(socket, "GET bar\n");
	sendCommand(socket, "QUIT\n");

	println("DONE");
	return 0;
}
#pragma once
#include <SDL.h>
#include <SDL_syswm.h>
#include <Windows.h>

class Window {

private:
	SDL_Window* window = NULL;

public:

	enum Win32_Event {
		ID_ABOUT,
		ID_EXIT,
		ID_DEBUG_PAINT,
		ID_DEBUG_FILL_BLACK,
		ID_OPEN_FILE,
	};

	
	Window(const char* title, int x, int y, int w, int h, Uint32 flags);

	void ActivateMenu(HWND window_ref);
	HWND GetSDLWinHandle(SDL_Window* win);

	SDL_Window* GetSDLWindow() { return window; };

};


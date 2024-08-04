#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#if defined(_WIN32)
#include <Windows.h>
#endif

class xim_window {

private:
	SDL_Window* window = NULL;

public:
	xim_window(const char* title, int x, int y, int w, int h, Uint32 flags);

#if defined(_WIN32)
	enum Win32_Event {
		ID_ABOUT,
		ID_EXIT,
		ID_DEBUG_PAINT,
		ID_DEBUG_FILL_BLACK,
		ID_OPEN_FILE,
	};

	void ActivateMenu(HWND window_ref);
	HWND GetSDLWinHandle(SDL_Window* win);
#endif

	SDL_Window* GetSDLWindow() { return window; };

};


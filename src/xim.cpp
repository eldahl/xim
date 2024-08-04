#include <SDL2/SDL_syswm.h>
#include <stdio.h>
#include <chrono>
#include <SDL2/SDL.h>
#include "tinyfiledialogs.h"
#include "xim_window.h"

#if defined(_WIN32)
HWND windowHandle;
#endif

SDL_Surface* image = NULL;
const char* imagePath = "res/tux.bmp";

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	else
		SDL_Log("Initialized SDL2");

	SDL_Window* sdlwindow = NULL;
	SDL_Surface* screenSurface = NULL;

#if defined(_WIN32)
	//Enable WinAPI Events Processing
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
#endif // defined(_WIN32)

	//Create window
	xim_window window = xim_window("xim - cross image manipulator",
							SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
							SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
							sdlwindow = window.GetSDLWindow();
	
	if (sdlwindow) {

#if defined(_WIN32)
		windowHandle = window.GetSDLWinHandle(sdlwindow);
		window.ActivateMenu(windowHandle);
#endif
	
		screenSurface = SDL_GetWindowSurface(sdlwindow);

		//Fill the surface with white
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
		SDL_UpdateWindowSurface(sdlwindow);

		//SDL_Delay(2000);
	}
	else
		SDL_LogError(1, "SDL_Window failed to be created! SDL_Error: %s\n", SDL_GetError());

	image = SDL_LoadBMP(imagePath);
	if (!image) {
		printf("Unable to load image %s! SDL Error: %s\n", imagePath, SDL_GetError());
	}

	SDL_Event mainEvent;

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point t2;

	bool isRunning = true;
	while (isRunning) {
		// Event handling
		SDL_PollEvent(&mainEvent);
		switch (mainEvent.type) {
		case SDL_WINDOWEVENT_CLOSE:
			mainEvent.type = SDL_QUIT;
			SDL_PushEvent(&mainEvent);
			break;
		case SDL_WINDOWEVENT:
			switch (mainEvent.window.event) {

			case SDL_WINDOWEVENT_SHOWN:
				// Window SDL Surface is invalidated on resize, so we have to get the new one.
				SDL_Log("Debug: Shown Surface");

				// Redraw workspace
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
				if(image)
					SDL_BlitSurface(image, NULL, screenSurface, NULL);
				SDL_UpdateWindowSurface(sdlwindow);
				break;

			case SDL_WINDOWEVENT_RESIZED:
				// Window SDL Surface is invalidated on resize, so we have to get the new one.
				screenSurface = SDL_GetWindowSurface(sdlwindow);
				SDL_Log("Debug: Resized and got new Window SDL Surface");

				// Redraw workspace
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
				if(image)
					SDL_BlitSurface(image, NULL, screenSurface, NULL);
				SDL_UpdateWindowSurface(sdlwindow);
				break;
			}
			break;
		case SDL_SYSWMEVENT:

			switch(mainEvent.syswm.msg->subsystem) {
#if defined(_WIN32)
			case SDL_SYSWM_WINDOWS:
				switch (mainEvent.syswm.msg->msg.win.msg) {
				case WM_COMMAND:
					switch (LOWORD(mainEvent.syswm.msg->msg.win.wParam)) {
					case xim_window::ID_EXIT:
						isRunning = false;
						break;
					case xim_window::ID_OPEN_FILE:
						SDL_Log("Opening file: %s", tinyfd_openFileDialog("Select file", NULL, NULL, NULL, NULL, 0));
						break;
					case xim_window::ID_DEBUG_PAINT:
						if(image)
							SDL_BlitSurface(image, NULL, screenSurface, NULL);
						SDL_UpdateWindowSurface(sdlwindow);
						break;
					case xim_window::ID_DEBUG_FILL_BLACK:
						//Fill the surface with black
						if (screenSurface)
							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
						SDL_UpdateWindowSurface(sdlwindow);
						break;
					}
					break;
				case WM_PAINT:
					/*
					t2 = std::chrono::high_resolution_clock::now();

					std::chrono::duration<double, std::milli> ms_double = t2 - t1;
					SDL_Log("Duration between redraw: %f", ms_double.count());

					t1 = t2;
					*/
					break;
				default: break;
				}
				break;
#endif
			case SDL_SYSWM_X11:
				SDL_Log("X11");
				break;
			case SDL_SYSWM_WAYLAND:
				SDL_Log("Wayland");
				break;
			}
			break;
		case SDL_RENDER_TARGETS_RESET:
			SDL_Log("Reset render targets");
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}

	//Destroy window
	SDL_DestroyWindow(sdlwindow);
	SDL_Quit();
	return 0;
}

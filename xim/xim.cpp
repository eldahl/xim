#include "SDL.h"
#include <SDL_syswm.h>
#include <stdio.h>
#include "tinyfiledialogs.h"

//Namespace variables/Defines
#define ID_ABOUT 2
#define ID_EXIT 3
#define ID_CONTROLS 4
#define ID_TEST 5
static HMENU hHelp;
static HMENU hEdit;
static HMENU hFile;
static HMENU hMenuBar;

HWND windowHandle;

void ActivateMenu(HWND windowRef);
HWND getSDLWinHandle(SDL_Window* win);

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	else
		SDL_Log("Initialized SDL2");

	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	//Create window
	window = SDL_CreateWindow("xim - cross image manipulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	if (window != NULL)
	{
		screenSurface = SDL_GetWindowSurface(window);

		//Fill the surface with white
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
		SDL_UpdateWindowSurface(window);
		
		//SDL_Delay(2000);
	}
	else
		SDL_Log("SDL_Window failed to be created! SDL_Error: %s\n", SDL_GetError());

	//Enable WinAPI Events Processing
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	windowHandle = getSDLWinHandle(window);
	ActivateMenu(windowHandle);

	SDL_Event mainEvent;

	bool isRunning = true;
	while (isRunning) {
		SDL_PollEvent(&mainEvent);
		switch (mainEvent.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
			mainEvent.type = SDL_QUIT;
			SDL_PushEvent(&mainEvent);
			break;
		case SDL_SYSWMEVENT:
			if (mainEvent.syswm.msg->msg.win.msg == WM_COMMAND)
			{
				if (LOWORD(mainEvent.syswm.msg->msg.win.wParam) == ID_EXIT)
				{
					isRunning = false;
				}
				else if (LOWORD(mainEvent.syswm.msg->msg.win.wParam) == ID_TEST)
				{
					tinyfd_openFileDialog("Select file", NULL, NULL, NULL, NULL, 0);
				}
			}

			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		};
	}

	//Destroy window
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
	
}


//Function which retrieves the address/Handle of an SDL window
//Also retrieves the specific subsystem used by SDL to create that window which is platform specific (Windows, MAC OS x, IOS, etc...)
HWND getSDLWinHandle(SDL_Window* win)
{
	SDL_SysWMinfo infoWindow;
	SDL_VERSION(&infoWindow.version);
	if (!SDL_GetWindowWMInfo(win, &infoWindow))
	{
		return NULL;
	}
	return (infoWindow.info.win.window);
}

//Initializes the native windows drop down menu elements of the window
void ActivateMenu(HWND windowRef)
{
	hMenuBar = CreateMenu();
	hFile = CreateMenu();
	hEdit = CreateMenu();
	hHelp = CreateMenu();

	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"File");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hEdit, L"Edit");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, L"Help");

	
	AppendMenu(hFile, MF_STRING, ID_TEST, L"Open");
	AppendMenu(hFile, MF_STRING, ID_EXIT, L"Exit");
	AppendMenu(hEdit, MF_STRING, ID_CONTROLS, L"Configure Controls");
	AppendMenu(hHelp, MF_STRING, ID_ABOUT, L"About");

	SetMenu(windowRef, hMenuBar);
}
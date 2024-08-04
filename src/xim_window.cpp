#include "xim_window.h"

#if defined(_WIN32)
//Namespace variables/Defines
static HMENU hHelp;
static HMENU hEdit;
static HMENU hFile;
static HMENU hTest;
static HMENU hMenuBar;
#endif

xim_window::xim_window(const char* title, int x, int y, int w, int h, Uint32 flags) {
	window = SDL_CreateWindow(title, x, y, w, h, flags);
}

#if defined(_WIN32)
//Initializes the native windows drop down menu elements of the window
void xim_window::ActivateMenu(HWND window_ref) {
	hMenuBar = CreateMenu();
	hFile = CreateMenu();
	hEdit = CreateMenu();
	hHelp = CreateMenu();
	hTest = CreateMenu();
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"File");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hTest, L"Debug");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hEdit, L"Edit");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, L"Help");
	AppendMenu(hFile, MF_STRING, ID_OPEN_FILE, L"Open");
	AppendMenu(hFile, MF_STRING, ID_EXIT, L"Exit");
	AppendMenu(hTest, MF_STRING, ID_DEBUG_PAINT, L"Repaint");
	AppendMenu(hTest, MF_STRING, ID_DEBUG_FILL_BLACK, L"Fill black");
	AppendMenu(hHelp, MF_STRING, ID_ABOUT, L"About");
	SetMenu(window_ref, hMenuBar);
}

//Function which retrieves the address/Handle of an SDL window
//Also retrieves the specific subsystem used by SDL to create that window which is platform specific (Windows, MAC OS x, IOS, etc...)
HWND xim_window::GetSDLWinHandle(SDL_Window* win) {
	SDL_SysWMinfo infoWindow;
	SDL_VERSION(&infoWindow.version);
	if (!SDL_GetWindowWMInfo(win, &infoWindow)) {
		return NULL;
	}
	return (infoWindow.info.win.window);
}
#endif

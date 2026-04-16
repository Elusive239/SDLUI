// --- SDL skeleton application demonstrating how to use SDLUI. ---

// There are 4 functions that handle initialization, input, window management and rendering:
// SDLUI_Init
// SDLUI_EventHandler
// SDLUI_WindowHandler
// SDLUI_Render

// Control creation and usage is performed with functions of the following convention:
// Control creation: 	SDLUI_Create<control name>
// Control usage: 		SDLUI<control name>

// The rest of the code is just what one would find in a typical SDL application.
// NOTE: The in-place includes were done for clarity.


#include <iostream>
#include <string>
#include <vector>

#ifdef __APPLE__
    #include <SDL2/SDL.h>
#else
    #include "SDL.h"
#endif


#include "./../src/sdlui.h"

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("SDLUI app", 1200, 800, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	// Initializes SDLUI. Called after a window and renderer have been created.
	SDLUI_Init(renderer, window);

	// Each SDLUI_Create_xxx function returns a pointer to a control. It is used to access its properties,
	// and is later passed on to its usage function inside tha main application loop.
	#include "sdlui_demo_controls_creation.cpp"

	bool quit = false;
	SDL_Event e;

	while (!quit)
	{
		if (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_EVENT_QUIT)
			{
				quit = true;
			}
			if(e.type == SDL_EVENT_WINDOW_MINIMIZED || e.type == SDL_EVENT_WINDOW_FOCUS_LOST)
			{
				while(SDL_PollEvent(&e))
				{
					if(e.type == SDL_EVENT_WINDOW_RESTORED || e.type == SDL_EVENT_WINDOW_FOCUS_GAINED)
					{
						SDLUI_ForceReDraw();
						break;
					}
				}
			}

			// Captures mouse input and window events.
			SDLUI_EventHandler(e);
		}

		// Manages SDLUI windows and their child controls
		SDLUI_WindowHandler();

		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
		SDL_RenderClear(renderer);

		// Controls are called just like in immediate mode GUIs. Example: if(SDLUI_Button(btn1)) {// do something}
		#include "sdlui_demo_controls_usage.cpp"

		// Draws the UI. Regular SDL drawing should take place before this, for the UI to appear on top.
		SDLUI_Render();
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

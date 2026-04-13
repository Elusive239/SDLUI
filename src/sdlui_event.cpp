void SDLUI_EventHandler(SDL_Event const &ev)
{
	if(ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN || ev.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		SDLUI_MouseStateSet(ev.type, ev.button.button);
	}

	if(ev.type == SDL_EVENT_MOUSE_WHEEL)
	{
		SDLUI_Core.mouse_wheel_y = ev.wheel.y;
		SDLUI_Core.mouse_wheel_x = ev.wheel.x;
		if(ev.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
		{
    			SDLUI_Core.mouse_wheel_x *= -1;
		}
	}
	else
	{
		SDLUI_Core.mouse_wheel_y = 0;
		SDLUI_Core.mouse_wheel_x = 0;
	}

	if(ev.type == SDL_EVENT_WINDOW_RESIZED)
	{
		SDLUI_ForceReDraw();
	}

	if(ev.type > 0)
	{
		SDLUI_Core.e = ev;
	}
}

SDLUI_Control_Window *SDLUI_CreateWindow(i32 x, i32 y, i32 w, i32 h, char const *title)
{
	SDLUI_Control_Window *wnd = (SDLUI_Control_Window*)malloc(sizeof(SDLUI_Control_Window));

	wnd->base.type = SDLUI_CONTROL_TYPE_WINDOW;
	wnd->title.create(title);
	wnd->base.x = x;
	wnd->base.y = y;
	wnd->drag_x = 0;
	wnd->drag_y = 0;
	wnd->base.w = w;
	wnd->base.h = h;
	wnd->base.visible = true;
	wnd->visible_last_frame = false;
	wnd->base.enabled = true;
	wnd->enabled_last_frame = false;
	wnd->is_resized = false;
	wnd->base.do_process = false;
	wnd->has_close_button = true;
	wnd->can_be_resized = true;

	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,wnd->title.data, 0, SDLUI_Core.theme.col_white);
	wnd->tex_title = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
	SDL_DestroySurface(s);

	wnd->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	wnd->children.create();
	SDLUI_Window_Collection.push(CTRL(wnd));

	return wnd;
}

SDLUI_Control_Button *SDLUI_CreateButton(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text)
{
	SDLUI_Control_Button *btn = (SDLUI_Control_Button*)malloc(sizeof(SDLUI_Control_Button));

	btn->base.type = SDLUI_CONTROL_TYPE_BUTTON;
	btn->base.do_process = false;
	btn->text.create(text);
	btn->base.x = wnd->base.x + x;
	btn->base.y = wnd->base.y + y;
	btn->base.w = 100;
	btn->base.h = 30;
	btn->base.visible = true;
	btn->base.enabled = true;
	btn->base.owned_by_window = true;
	btn->align = SDLUI_ALIGN_CENTER;
	btn->state = SDLUI_BUTTON_STATE_NORMAL;
	btn->base.parent = CTRL(wnd);

	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text, 0, SDLUI_Core.theme.col_white);
	btn->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
	SDL_DestroySurface(s);

	wnd->children.push(CTRL(btn));
	return btn;
}

SDLUI_Control_SliderInt *SDLUI_CreateSliderInt(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 min, i32 max, i32 value, SDLUI_ORIENTATION orientation = SDLUI_ORIENTATION_HORIZONTAL)
{
	SDLUI_Control_SliderInt *si = (SDLUI_Control_SliderInt*)malloc(sizeof(SDLUI_Control_SliderInt));

	si->base.type = SDLUI_CONTROL_TYPE_SLIDER_INT;
	si->base.do_process = false;
	si->base.x = wnd->base.x + x;
	si->base.y = wnd->base.y + y;
	si->base.visible = true;
	si->base.enabled = true;
	si->base.owned_by_window = true;
	si->min = min;
	si->max = max;
	si->value = value;
	si->thumb_size = 12;
	si->is_changing = false;
	si->orientation = orientation;
	si->base.parent = CTRL(wnd);

	if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
	{
		si->base.w = 100;
		si->base.h = 16;
	}
	else
	{
		si->base.w = 16;
		si->base.h = 100;
	}

	wnd->children.push(CTRL(si));
	return si;
}

SDLUI_Control_CheckBox *SDLUI_CreateCheckBox(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text, bool checked)
{
	SDLUI_Control_CheckBox *chk = (SDLUI_Control_CheckBox*)malloc(sizeof(SDLUI_Control_CheckBox));

	chk->base.type = SDLUI_CONTROL_TYPE_CHECKBOX;
	chk->base.do_process = false;
	chk->base.x = wnd->base.x + x;
	chk->base.y = wnd->base.y + y;
	chk->base.w = 16;
	chk->base.h = 16;
	chk->base.visible = true;
	chk->base.enabled = true;
	chk->base.owned_by_window = true;
	chk->checked = checked;
	chk->base.parent = CTRL(wnd);

	if(strlen(text) > 0)
	{
		chk->text.create(text);
		SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text, 0, SDLUI_Core.theme.col_white);
		chk->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
		SDL_DestroySurface(s);
	}
	else
	{
		chk->tex_text = NULL;
	}

	wnd->children.push(CTRL(chk));
	return chk;
}

SDLUI_Control_Text *SDLUI_CreateText(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text)
{
	SDLUI_Control_Text *txt = (SDLUI_Control_Text*)malloc(sizeof(SDLUI_Control_Text));

	txt->base.type = SDLUI_CONTROL_TYPE_TEXT;
	txt->base.do_process = false;
	txt->text.create(text);
	txt->base.x = wnd->base.x + x;
	txt->base.y = wnd->base.y + y;
	txt->base.w = (txt->text.length) * SDLUI_Font.width;
	txt->base.h = SDLUI_Font.height;
	txt->base.visible = true;
	txt->base.enabled = true;
	txt->base.owned_by_window = true;
	txt->base.parent = CTRL(wnd);

	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,txt->text.data, 0, SDLUI_Core.theme.col_white);
	txt->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
	SDL_DestroySurface(s);

	wnd->children.push(CTRL(txt));
	return txt;
}

SDLUI_Control_ToggleButton *SDLUI_CreateToggleButton(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text, bool checked)
{
	SDLUI_Control_ToggleButton *tb = (SDLUI_Control_ToggleButton*)malloc(sizeof(SDLUI_Control_ToggleButton));

	tb->base.type = SDLUI_CONTROL_TYPE_TOGGLE_BUTTON;
	tb->base.do_process = false;
	tb->base.x = wnd->base.x + x;
	tb->base.y = wnd->base.y + y;
	tb->base.w = 32;
	tb->base.h = 16;
	tb->base.visible = true;
	tb->base.enabled = true;
	tb->base.owned_by_window = true;
	tb->checked = checked;
	tb->base.parent = CTRL(wnd);

	if(strlen(text) > 0)
	{
		tb->text.create(text);
		SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text, 0, SDLUI_Core.theme.col_white);
		tb->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
		SDL_DestroySurface(s);
	}
	else
	{
		tb->tex_text = NULL;
	}

	wnd->children.push(CTRL(tb));

	return tb;
}

SDLUI_ArrayOfControls SDLUI_CreateRadioButtonGroup()
{
	SDLUI_ArrayOfControls rb_group;
	rb_group.create();
	return rb_group;
}

SDLUI_Control_RadioButton *SDLUI_CreateRadioButton(SDLUI_Control_Window *wnd, SDLUI_ArrayOfControls &group, i32 x, i32 y, char const *text, bool checked)
{
	SDLUI_Control_RadioButton *rb = (SDLUI_Control_RadioButton*)malloc(sizeof(SDLUI_Control_RadioButton));

	rb->base.type = SDLUI_CONTROL_TYPE_RADIO_BUTTON;
	rb->base.do_process = false;
	rb->base.x = wnd->base.x + x;
	rb->base.y = wnd->base.y + y;
	rb->base.w = 16;
	rb->base.h = 16;
	rb->base.visible = true;
	rb->base.enabled = true;
	rb->base.owned_by_window = true;
	rb->checked = checked;
	rb->group = &group;
	rb->group->push(CTRL(rb));
	rb->base.parent = CTRL(wnd);

	if(strlen(text) > 0)
	{
		rb->text.create(text);
		SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text,0, SDLUI_Core.theme.col_white);
		rb->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
		SDL_DestroySurface(s);
	}
	else
	{
		rb->tex_text = NULL;
	}

	wnd->children.push(CTRL(rb));

	return rb;
}

SDLUI_Control_TabContainer *SDLUI_CreateTabContainer(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w, i32 h)
{
	SDLUI_Control_TabContainer *tbc = (SDLUI_Control_TabContainer*)malloc(sizeof(SDLUI_Control_TabContainer));

	tbc->base.type = SDLUI_CONTROL_TYPE_TAB_CONTAINER;
	tbc->base.do_process = false;
	tbc->tabs.create();
	tbc->base.x = wnd->base.x + x;
	tbc->base.y = wnd->base.y + y;
	tbc->base.w = w;
	tbc->base.h = h;
	tbc->base.visible = true;
	tbc->base.enabled = true;
	tbc->base.owned_by_window = true;
	tbc->bar_height = 30;
	tbc->active_tab = NULL;
	tbc->base.parent = CTRL(wnd);

	wnd->children.push(CTRL(tbc));
	return tbc;

}

SDLUI_Control_ScrollArea *SDLUI_CreateScrollArea(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w, i32 h, SDL_Texture *tex)
{
	SDLUI_Control_ScrollArea *sa = (SDLUI_Control_ScrollArea*)malloc(sizeof(SDLUI_Control_ScrollArea));

	sa->base.type = SDLUI_CONTROL_TYPE_SCROLL_AREA;
	sa->base.do_process = false;
	sa->base.x = wnd->base.x + x;
	sa->base.y = wnd->base.y + y;
	sa->base.w = w;
	sa->base.h = h;
	sa->base.visible = true;
	sa->base.enabled = true;
	sa->base.owned_by_window = true;
	sa->scrollbar_thickness = 12;
	sa->track_size_h = sa->base.w ;//- sa->scrollbar_thickness;
	sa->track_size_v = sa->base.h ;//- sa->scrollbar_thickness;
	sa->scroll_x = 0;
	sa->scroll_y = 0;
	sa->is_changing_v = false;
	sa->is_changing_h = false;
	sa->tex_rect = tex;
	if(SDL_GetTextureSize(sa->tex_rect, &sa->content_width, &sa->content_height) == 0)
	{
		if(sa->content_width > sa->base.w)
		{
			sa->client_height = sa->base.h - sa->scrollbar_thickness;
		}
		if(sa->content_height > sa->base.h)
		{
			sa->client_width = sa->base.w - sa->scrollbar_thickness;
		}
	}
	else
	{
		sa->client_width = sa->base.w;
		sa->client_height = sa->base.h;
	}

	sa->base.parent = CTRL(wnd);

	wnd->children.push(CTRL(sa));
	return sa;
}

SDLUI_Control_List *SDLUI_CreateList(SDLUI_Control_Window *wnd, SDLUI_Control_ScrollArea *sa, i32 num_items)
{
	SDLUI_Control_List *lst = (SDLUI_Control_List*)malloc(sizeof(SDLUI_Control_List));

	lst->base.type = SDLUI_CONTROL_TYPE_LIST;
	lst->base.do_process = false;
	lst->scroll_area = sa;
	lst->num_items = num_items;
	lst->selected_index = 0;
	lst->max_string_width = 0;

	float h = num_items * SDLUI_Font.height;
	if(h < sa->base.h)
	{
		h = sa->base.h;
	}
	sa->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sa->base.w, h);
	sa->content_width = sa->base.w;
	sa->content_height = h;

	sa->client_height = sa->base.h - sa->scrollbar_thickness;

	if(SDL_GetTextureSize(sa->tex_rect,  &sa->content_width, &sa->content_height) == 0)
	{
		if(sa->content_width > sa->base.w)
		{
			sa->client_height = sa->base.h - sa->scrollbar_thickness;
		}
		if(sa->content_height > sa->base.h)
		{
			sa->client_width = sa->base.w - sa->scrollbar_thickness;
		}
	}
	else
	{
		sa->client_width = sa->base.w;
		sa->client_height = sa->base.h;
	}

	return lst;
}

SDLUI_Control_TextBox *SDLUI_CreateTextBox(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w)
{
	SDLUI_Control_TextBox *tbx = (SDLUI_Control_TextBox*)malloc(sizeof(SDLUI_Control_TextBox));

	tbx->base.do_process = false;

	tbx->base.type = SDLUI_CONTROL_TYPE_TEXTBOX;
	tbx->base.do_process = false;
	tbx->base.x = wnd->base.x + x;
	tbx->base.y = wnd->base.y + y;
	tbx->base.w = w;
	tbx->base.h = 30;
	tbx->base.visible = true;
	tbx->base.enabled = true;
	tbx->base.owned_by_window = true;
	tbx->text.create("");
	tbx->select_start = 0;
	tbx->select_end = 0;
	tbx->cursor_pos = 0;
	tbx->focused = false;
	tbx->scroll = 0;
	tbx->max_chars = (w - (2 * SDLUI_MARGIN)) / SDLUI_Font.width;

	i32 tw = w - (2 * SDLUI_MARGIN);
	i32 th = SDLUI_Font.height;
	tbx->tex_text = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tw, th);

	tbx->base.parent = CTRL(wnd);
	wnd->children.push(CTRL(tbx));

	return tbx;
}

bool SDLUI_Window(SDLUI_Control_Window *wnd)
{
	if(!wnd->base.visible)
	{
		return false;
	}

	wnd->base.do_process = true;

	if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED || SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_NONE)
	{
		wnd->is_dragged = false;
	}

	if(wnd == SDLUI_Core.active_window)
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_FRect r = {wnd->base.x, wnd->base.y, wnd->base.w, wnd->base.h};

		if(SDLUI_PointInRect(r, mx, my))
		{
			wnd->is_hovered = true;
		}
		else
		{
			wnd->is_hovered = false;
		}

		if(wnd->has_close_button)
		{
			r = (SDL_FRect){wnd->base.x + wnd->base.w - 30, wnd->base.y, 30, 30};

			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED && SDLUI_PointInRect(r, mx, my))
			{
				wnd->base.visible = false;
				SDLUI_Core.active_window = NULL;
				SDL_SetCursor(SDLUI_Core.cursor_arrow);
			}
		}

		r = (SDL_FRect){wnd->base.x,wnd->base.y,wnd->base.w - (wnd->has_close_button * 30), 30};

		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED && !wnd->is_dragged)
			{
				wnd->is_dragged = true;
				wnd->drag_x = mx - wnd->base.x;
				wnd->drag_y = my - wnd->base.y;
			}
		}

		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD && wnd->is_dragged)
		{
			SDL_SetCursor(SDLUI_Core.cursor_arrow);
			i32 old_x = wnd->base.x;
			i32 old_y = wnd->base.y;
			wnd->base.x = mx - wnd->drag_x;
			wnd->base.y = my - wnd->drag_y;

			for (int i = 0; i < wnd->children.size; ++i)
			{
				wnd->children.data[i]->x += wnd->base.x - old_x;
				wnd->children.data[i]->y += wnd->base.y - old_y;
			}
		}

		return false;
	}

	return false;
}

bool SDLUI_Button(SDLUI_Control_Button *btn)
{
	btn->base.do_process = true;

	if(btn->base.visible && btn->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window->is_hovered)
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_FRect r = {btn->base.x,btn->base.y,btn->base.w,btn->base.h};
		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				btn->state = SDLUI_BUTTON_STATE_CLICK;
				return true;
			}
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
			{
				btn->state = SDLUI_BUTTON_STATE_CLICK;
			}
			else
			{
				btn->state = SDLUI_BUTTON_STATE_HOVER;
			}

			return false;
		}

		btn->state = SDLUI_BUTTON_STATE_NORMAL;
		return false;
	}

	btn->state = SDLUI_BUTTON_STATE_NORMAL;
	return false;
}

bool SDLUI_SliderInt(SDLUI_Control_SliderInt *si)
{
	si->base.do_process = true;

	if(si->base.visible && si->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window->is_hovered)
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_FRect r = {si->base.x,si->base.y,si->base.w,si->base.h};
		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				// if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
				// {
				// 	si->value = SDLUI_Map(si->base.x, si->base.x + si->base.w, si->min, si->max, mx);
				// }
				// else
				// {
				// 	si->value = SDLUI_Map(si->base.y + si->base.h, si->base.y, si->min, si->max, my);
				// }

				si->is_changing = true;
			}
		}

		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD && si->is_changing)
		{
			if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
			{
				si->value = SDLUI_Map(si->base.x, si->base.x + si->base.w, si->min, si->max, mx);
				si->value = SDLUI_Clamp(si->value, si->min, si->max);
			}
			else
			{
				si->value = SDLUI_Map(si->base.y + si->base.h, si->base.y, si->min, si->max, my);
				si->value = SDLUI_Clamp(si->value, si->min, si->max);
			}

			// NOTE: Return here or on mouse release?
			return true;
		}

		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED && si->is_changing)
		{
			si->is_changing = false;
			return true;
		}

		return false;
	}

	//si->is_changing = false;
	return false;
}

bool SDLUI_CheckBox(SDLUI_Control_CheckBox *chk)
{
	chk->base.do_process = true;

	if(chk->base.visible && chk->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window->is_hovered)
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		float tex_w, tex_h;
		SDL_GetTextureSize(chk->tex_text,&tex_w , &tex_h);

		SDL_FRect r = {chk->base.x, chk->base.y, chk->base.w, chk->base.h};
		if(chk->tex_text != NULL)
		{
			r.w += SDLUI_MARGIN + tex_w;
		}
		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				chk->checked = !chk->checked;
				return true;
			}
		}
		return false;
	}
	return false;
}

bool SDLUI_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
	tb->base.do_process = true;

	if(tb->base.visible && tb->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window->is_hovered)
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		float tex_w, tex_h;
		SDL_GetTextureSize(tb->tex_text,&tex_w , &tex_h);

		SDL_FRect r = {tb->base.x,tb->base.y,tb->base.w,tb->base.h};
		if(tb->tex_text != NULL)
		{
			r.w += SDLUI_MARGIN + tex_w;
		}

		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				tb->checked = !tb->checked;
				return true;
			}
		}

		return false;
	}
	return false;
}

bool SDLUI_RadioButton(SDLUI_Control_RadioButton *rb)
{
	rb->base.do_process = true;

	if(rb->base.visible && rb->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window->is_hovered)
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		float tex_w, tex_h;
		SDL_GetTextureSize(rb->tex_text, &tex_w , &tex_h);

		SDL_FRect r = {rb->base.x,rb->base.y,rb->base.w,rb->base.h};
		if(rb->tex_text != NULL)
		{
			r.w += SDLUI_MARGIN + tex_w;
		}

		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				SDLUI_Control_RadioButton *cur;

				for (int i = 0; i < rb->group->size; ++i)
				{
					cur = (SDLUI_Control_RadioButton*)rb->group->data[i];
					cur->checked = false;
				}

				rb->checked = true;

				return true;
			}
		}

		return false;
	}
	return false;
}

bool SDLUI_Text(SDLUI_Control_Text *txt)
{
	txt->base.do_process = true;

	if(txt->base.visible && txt->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window->is_hovered)
	{
		return true;
	}
	return false;
}

bool SDLUI_TabContainer(SDLUI_Control_TabContainer *tbc)
{
	tbc->base.do_process = true;

	if(tbc->base.visible && tbc->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window->is_hovered)
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_FRect r = {tbc->base.x,tbc->base.y,tbc->base.w,tbc->bar_height};
		SDL_FRect tab_r;
		i32 offset = 0;
		SDLUI_Control_Tab *tab;

		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				for (int i = 0; i < tbc->tabs.size; ++i)
				{
					tab = (SDLUI_Control_Tab*)tbc->tabs.data[i];
					tab_r = (SDL_FRect){tbc->base.x + offset, tbc->base.y, tab->base.w + SDLUI_MARGIN, 30};

					if(SDLUI_PointInRect(tab_r, mx, my))
					{
						for (int j = 0; j < tbc->active_tab->children.size; ++j)
						{
							SDLUI_Control *ctrl = tbc->active_tab->children.data[j];
							ctrl->visible = false;
						}
						tbc->active_tab = tab;
					}

					offset += SDLUI_MARGIN + tab->base.w;
				}
			}
		}
	}
	return false;
}

bool SDLUI_ScrollArea(SDLUI_Control_ScrollArea *sa)
{
	sa->base.do_process = true;

	if(sa->base.visible && sa->base.parent == CTRL(SDLUI_Core.active_window) )
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_FRect r, rv, rh;

		r = (SDL_FRect){sa->base.x, sa->base.y, sa->base.w, sa->base.h};

		if(sa->content_height > sa->base.h)
		{
			if(SDLUI_PointInRect(r, mx, my))
			{
				// mouse wheel vertical
				if(SDLUI_Core.mouse_wheel_y != 0)
				{
					sa->scroll_y -= SDLUI_Core.mouse_wheel_y;
					sa->scroll_y = SDLUI_Clamp(sa->scroll_y, 0, sa->track_size_v - sa->thumb_size_v);
					SDLUI_Core.mouse_wheel_y = 0;
					return true;
				}
			}

			static float my_offset;

			rv = (SDL_FRect){sa->base.x + sa->base.w - sa->scrollbar_thickness, sa->base.y + sa->scroll_y, sa->scrollbar_thickness, sa->thumb_size_v};
			if(SDLUI_PointInRect(rv, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					my_offset = my - sa->scroll_y;
					sa->is_changing_v = true;
				}
			}

			// scroll per page
			rv.y = sa->base.y;
			rv.h = sa->scroll_y;

			if(SDLUI_PointInRect(rv, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					float pages = (float)sa->content_height / (float)sa->base.h;
					float pixels_per_page = (float)sa->track_size_v / pages;
					sa->scroll_y -= pixels_per_page;
					sa->scroll_y = SDLUI_Clamp(sa->scroll_y, 0, sa->track_size_v - sa->thumb_size_v);
					return true;
				}
			}

			rv.y = sa->base.y + sa->scroll_y + sa->thumb_size_v;
			rv.h = sa->track_size_v - sa->thumb_size_v - sa->scroll_y;

			if(SDLUI_PointInRect(rv, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					float pages = (float)sa->content_height / (float)sa->base.h;
					float pixels_per_page = (float)sa->track_size_v / pages;
					sa->scroll_y += pixels_per_page;
					sa->scroll_y = SDLUI_Clamp(sa->scroll_y, 0, sa->track_size_v - sa->thumb_size_v);
					return true;
				}
			}

			if(sa->is_changing_v)
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
				{
					sa->scroll_y = my - my_offset;
					sa->scroll_y = SDLUI_Clamp(sa->scroll_y, 0, sa->track_size_v - sa->thumb_size_v);
				}

				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED)
				{
					sa->is_changing_v = false;
					return true;
				}
			}
		}

		if(sa->content_width > sa->base.w)
		{
			if(SDLUI_PointInRect(r, mx, my))
			{
				// mouse wheel horizontal
				if(SDLUI_Core.mouse_wheel_x != 0)
				{
					sa->scroll_x += SDLUI_Core.mouse_wheel_x;
					sa->scroll_x = SDLUI_Clamp(sa->scroll_x, 0, sa->track_size_h - sa->thumb_size_h);
					SDLUI_Core.mouse_wheel_x = 0;
					return true;
				}
			}

			static float mx_offset;

			rh = (SDL_FRect){sa->base.x + sa->scroll_x, sa->base.y + sa->base.h - sa->scrollbar_thickness, sa->thumb_size_h, sa->scrollbar_thickness};
			if(SDLUI_PointInRect(rh, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					mx_offset = mx - sa->scroll_x;
					sa->is_changing_h = true;
				}
			}

			// scroll per page
			rh.x = sa->base.x;
			rh.w = sa->scroll_x;

			if(SDLUI_PointInRect(rh, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					float pages = (float)sa->content_width / (float)sa->base.w;
					float pixels_per_page = (float)sa->track_size_h / pages;
					sa->scroll_x -= pixels_per_page;
					sa->scroll_x = SDLUI_Clamp(sa->scroll_x, 0, sa->track_size_h - sa->thumb_size_h);
					return true;
				}
			}
			

			rh.x = sa->base.x + sa->scroll_x + sa->thumb_size_h;
			rh.w = sa->track_size_h - sa->thumb_size_h - sa->scroll_x;

			if(SDLUI_PointInRect(rh, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					float pages = (float)sa->content_width / (float)sa->base.w;
					float pixels_per_page = (float)sa->track_size_h / pages;
					sa->scroll_x += pixels_per_page;
					sa->scroll_x = SDLUI_Clamp(sa->scroll_x, 0, sa->track_size_h - sa->thumb_size_h);
					return true;
				}
			}

			if(sa->is_changing_h)
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
				{
					sa->scroll_x = mx - mx_offset;
					sa->scroll_x = SDLUI_Clamp(sa->scroll_x, 0, sa->track_size_h - sa->thumb_size_h);
				}

				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED)
				{
					sa->is_changing_h = false;
					return true;
				}
			}

		}
		return false;
	}
	return false;
}

bool SDLUI_List(SDLUI_Control_List *lst, const char *cur_item, i32 num_items, i32 cur_index)
{
	lst->base.do_process = true;

	i32 clicked = false;

	if(lst->scroll_area->base.visible && lst->scroll_area->base.parent == CTRL(SDLUI_Core.active_window))
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_FRect r = {lst->scroll_area->base.x, lst->scroll_area->base.y, lst->scroll_area->client_width, lst->scroll_area->client_height};

		if(SDLUI_PointInRect(r, mx, my) && cur_index == 0)
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				float ratio = (float)lst->scroll_area->content_height / (float)lst->scroll_area->client_height;
				float oy = my - lst->scroll_area->base.y + ((float)lst->scroll_area->scroll_y * ratio);
				lst->selected_index = oy / SDLUI_Font.height;
				lst->selected_index = SDLUI_Clamp(lst->selected_index, 0, lst->num_items - 1);
				clicked = true;
			}
		}
	}

	if(lst->num_items != num_items)
	{
		lst->num_items = num_items;
		SDL_DestroyTexture(lst->scroll_area->tex_rect);
		float h = num_items * SDLUI_Font.height;

		if(h < lst->scroll_area->base.h)
		{
			h = lst->scroll_area->base.h;
		}

		lst->scroll_area->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, lst->scroll_area->base.w, h);
		lst->scroll_area->content_width = lst->scroll_area->base.w;
		lst->scroll_area->content_height = h;
	}

	static i32 counter;
	
	if(counter == 0)
	{
		SDLUI_SetColor(SDLUI_Core.theme.col_list_bg);
		SDL_FRect r = {0, 0, lst->scroll_area->content_width, lst->scroll_area->content_height};
		SDL_SetRenderTarget(SDLUI_Core.renderer, lst->scroll_area->tex_rect);
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
	}

	if(counter == lst->selected_index)
	{
		SDL_SetRenderTarget(SDLUI_Core.renderer, lst->scroll_area->tex_rect);
		SDLUI_SetColor(SDLUI_Core.theme.col_highlight);

		float ratio = (float)lst->scroll_area->content_width / (float)lst->scroll_area->client_width;
		float offset_x = lst->scroll_area->scroll_x * ratio;
		SDL_FRect r = {offset_x, lst->selected_index * SDLUI_Font.height, lst->scroll_area->base.w, SDLUI_Font.height};
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		
	}
	
	if(strlen(cur_item) * SDLUI_Font.width > lst->max_string_width)
	{
		lst->max_string_width = strlen(cur_item) * SDLUI_Font.width;
	}
	float cur_tex_width;
	SDL_GetTextureSize(lst->scroll_area->tex_rect , &cur_tex_width, NULL);

	if(lst->max_string_width > cur_tex_width)
	{
		SDL_DestroyTexture(lst->scroll_area->tex_rect);
		float h = num_items * SDLUI_Font.height;
		if(h < lst->scroll_area->base.h)
		{
			h = lst->scroll_area->base.h;
		}
		lst->scroll_area->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, lst->max_string_width, h);
		lst->scroll_area->content_width = lst->max_string_width;
		lst->scroll_area->content_height = h;
	}

	static float offset_y;

	SDLUI_DrawText(SDLUI_MARGIN, offset_y, cur_item, lst->scroll_area->tex_rect);
	offset_y += SDLUI_Font.height;
	counter++;

	if(counter == num_items)
	{
		offset_y = 0;
		counter = 0;
        clicked = false;
	}

	return clicked;
}

bool SDLUI_TextBox(SDLUI_Control_TextBox *tbx)
{
	tbx->base.do_process = true;

	if(tbx->base.visible && tbx->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window->is_hovered)
	{
		float mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_FRect r = {tbx->base.x,tbx->base.y,tbx->base.w,tbx->base.h};

		if(SDLUI_PointInRect(r, mx, my))
		{
			SDL_SetCursor(SDLUI_Core.cursor_ibeam);
		}
		else
		{
			SDL_SetCursor(SDLUI_Core.cursor_arrow);
		}


		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
		{
			if(SDLUI_PointInRect(r, mx, my))
			{
				tbx->focused = true;
				SDL_StartTextInput(SDLUI_Core.window);
				return true;
			}
			else
			{
				SDL_StopTextInput(SDLUI_Core.window);
				tbx->focused = false;
			}
		}
	}

	if(tbx->focused && tbx->base.parent == CTRL(SDLUI_Core.active_window))
	{
		if(SDLUI_Core.e.type == SDL_EVENT_TEXT_INPUT)
		{
			SDLUI_String_insert_char(&tbx->text, SDLUI_Core.e.text.text[0], tbx->cursor_pos);
			tbx->cursor_pos++;

			if(tbx->cursor_pos > tbx->max_chars)
			{
				tbx->scroll++;
			}
		}

		if(SDLUI_Core.e.type == SDL_EVENT_KEY_DOWN)
		{
			if(SDLUI_Core.e.key.scancode == SDL_SCANCODE_LEFT)
			{
				if(SDLUI_Core.e.key.mod == SDL_KMOD_LCTRL)
				{
					while(tbx->cursor_pos > 0)
					{
						tbx->cursor_pos--;
						if(tbx->text.data[tbx->cursor_pos] != ' ')
						{
							break;
						}
					}

					while(tbx->cursor_pos > 0)
					{
						int index = tbx->cursor_pos > 0 ? tbx->cursor_pos - 1 : 0;
						if(tbx->text.data[index] == ' ')
						{
							break;
						}

						tbx->cursor_pos--;
					}
				}
				else
				{
					if(tbx->cursor_pos > 0)
					{
						tbx->cursor_pos--;

						if(tbx->cursor_pos < tbx->scroll)
						{
							tbx->scroll--;
        						//0123456789-----15
        						//asdfghjkl1234567
        						//          |    |
						}
					}
				}
			}
			else if(SDLUI_Core.e.key.scancode == SDL_SCANCODE_RIGHT)
			{
				if(SDLUI_Core.e.key.mod == SDL_KMOD_LCTRL)
				{
					if(tbx->text.data[tbx->cursor_pos] == ' ')
					{
						while(tbx->cursor_pos < tbx->text.length)
						{
							tbx->cursor_pos++;
							if(tbx->text.data[tbx->cursor_pos] != ' ')
							{
								break;
							}
						}
					}

					while(tbx->cursor_pos < tbx->text.length)
					{
						if(tbx->text.data[tbx->cursor_pos] == ' ')
						{
							break;
						}

						tbx->cursor_pos++;
					}
				}
				else
				{
					if(tbx->cursor_pos < tbx->text.length)
					{
						tbx->cursor_pos++;

						if(tbx->cursor_pos > tbx->max_chars)
						{
							tbx->scroll++;
						}
					}
				}
			}
			else if(SDLUI_Core.e.key.scancode == SDL_SCANCODE_BACKSPACE)
			{
				if(SDLUI_Core.e.key.mod == SDL_KMOD_LCTRL)
				{
					int start_to_reverse = tbx->cursor_pos;
					while(tbx->cursor_pos > 0)
					{
						tbx->cursor_pos--;
						if(tbx->text.data[tbx->cursor_pos] != ' ')
						{
							break;
						}
					}

					while(tbx->cursor_pos > 0)
					{
						int index = tbx->cursor_pos > 0 ? tbx->cursor_pos - 1 : 0;
						if(tbx->text.data[index] == ' ')
						{
							break;
						}

						tbx->cursor_pos--;
					}

					int now_cursor_pos = tbx->cursor_pos;
					while(start_to_reverse > now_cursor_pos)
					{
						SDLUI_String_delete_char(
							&tbx->text, start_to_reverse - 1
						);
						start_to_reverse--;
					}
				} else
				{
					if(tbx->cursor_pos > 0)
					{
						SDLUI_String_delete_char(
							&tbx->text, tbx->cursor_pos - 1
						);
						tbx->cursor_pos--;

						if(tbx->text.length > tbx->max_chars - 1)
						{
							tbx->scroll--;
						}
					}
				}
			} else if(SDLUI_Core.e.key.scancode == SDL_SCANCODE_HOME)
			{
				tbx->cursor_pos = 0;
				tbx->scroll = 0;
			} else if(SDLUI_Core.e.key.scancode == SDL_SCANCODE_END)
			{
				tbx->cursor_pos = tbx->text.length;
				tbx->scroll = tbx->text.length - tbx->max_chars;
				if(tbx->scroll < 0)
				{
					tbx->scroll = 0;
				}
			}
		}
		memset(&SDLUI_Core.e,0, sizeof(SDL_Event));
	}

	return false;
}

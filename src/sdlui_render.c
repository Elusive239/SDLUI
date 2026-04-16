void SDLUI_DrawText(i32 x, i32 y, const char *text, SDL_Texture *dst)
{
	SDL_SetRenderTarget(SDLUI_Core.renderer, dst);
	i32 cur_char;
	SDL_FRect glyph_rect;
	SDL_FRect pos = (SDL_FRect) {x, y, SDLUI_Font.width, SDLUI_Font.height};

	i32 count = strlen(text);

	for (int i = 0; i < count; ++i)
	{
		cur_char = (i32)text[i];
		glyph_rect = (SDL_FRect) {(cur_char - 32) * SDLUI_Font.width, 0, SDLUI_Font.width, SDLUI_Font.height};

		SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Font.tex_font, &glyph_rect, &pos);
		pos.x += SDLUI_Font.width;
	}

	SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
}

void SDLUI_DrawTextRange(i32 x, i32 y, const char *text, i32 start, i32 length, SDL_Texture *dst)
{
	SDL_SetRenderTarget(SDLUI_Core.renderer, dst);
	i32 cur_char;
	SDL_FRect glyph_rect;
	SDL_FRect pos = (SDL_FRect) {x, y, SDLUI_Font.width, SDLUI_Font.height};

	for (int i = start; i < start + length; ++i)
	{
		cur_char = (i32)text[i];
		glyph_rect = (SDL_FRect) {(cur_char - 32) * SDLUI_Font.width, 0, SDLUI_Font.width, SDLUI_Font.height};

		SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Font.tex_font, &glyph_rect, &pos);
		pos.x += SDLUI_Font.width;
	}

	SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
}

void SDLUI_Render_Button(SDLUI_Control_Button *btn)
{
	if(btn->base.visible)
	{
		if(btn->text.modified)
		{
			SDL_Color c = {255, 255, 255, 255};
			SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, btn->text.data, 0, c);
			btn->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
			SDL_DestroySurface(s);
		}

		i32 xx = btn->base.x - btn->base.parent->x;
		i32 yy = btn->base.y - btn->base.parent->y;

		SDL_FRect r = (SDL_FRect) {xx, yy, btn->base.w, btn->base.h};

		switch (btn->state)
		{
			case SDLUI_BUTTON_STATE_NORMAL:
			SDLUI_SetColor(SDLUI_Core.theme.col_button_normal);
			break;

			case SDLUI_BUTTON_STATE_HOVER:
			SDLUI_SetColor(SDLUI_Core.theme.col_highlight);
			break;

			case SDLUI_BUTTON_STATE_CLICK:
			SDLUI_SetColor(SDLUI_Core.theme.col_button_click);
			break;
		}

		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_SetColor(SDLUI_Core.theme.col_grey);
		SDL_RenderRect(SDLUI_Core.renderer, &r);

		float tex_w, tex_h;
		SDL_GetTextureSize(btn->tex_text,  &tex_w, &tex_h);

		i32 diff = btn->base.w - tex_w;
		i32 offset = diff * btn->align / 2;

		if(btn->align == SDLUI_ALIGN_LEFT)
		{
			offset += SDLUI_MARGIN;
		}
		else if(btn->align == SDLUI_ALIGN_RIGHT)
		{
			offset -= SDLUI_MARGIN;
		}

		r = (SDL_FRect) {xx + offset, yy + SDLUI_MARGIN, tex_w, tex_h};
		SDL_RenderTexture(SDLUI_Core.renderer, btn->tex_text, NULL, &r);
	}
}

void SDLUI_Render_SliderInt(SDLUI_Control_SliderInt *si)
{
	if(si->base.visible)
	{
		if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
		{
			i32 xx = si->base.x - si->base.parent->x;
			i32 yy = si->base.y - si->base.parent->y;

			i32 fill = (i32)SDLUI_Map(si->min, si->max, 0, si->base.w, si->value);

			SDLUI_SetColor(SDLUI_Core.theme.col_highlight);
			SDL_RenderLine(SDLUI_Core.renderer, xx, yy + 8, xx + fill, yy + 8);

			SDLUI_SetColor(SDLUI_Core.theme.col_border);
			SDL_RenderLine(SDLUI_Core.renderer, xx + fill, yy + 8, xx + si->base.w, yy + 8);

			SDL_FRect r = (SDL_FRect) {xx + fill-8, yy, 16, 16};
			r.x = SDLUI_Clamp(r.x, xx, xx + si->base.w - 14);
			SDLUI_Colorize(SDLUI_Core.tex_circle_fill_1, SDLUI_Core.theme.col_scrollbar_thumb);
			SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_1, NULL, &r);
			SDLUI_Colorize(SDLUI_Core.tex_circle, SDLUI_Core.theme.col_white);
			SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_circle, NULL, &r);
		}
		else
		{
			i32 xx = si->base.x - si->base.parent->x;
			i32 yy = si->base.y - si->base.parent->y;

			i32 fill = (i32)SDLUI_Map(si->min, si->max, 0, si->base.h, si->value);

			SDLUI_SetColor(SDLUI_Core.theme.col_highlight);
			SDL_RenderLine(SDLUI_Core.renderer, xx + 8, yy + si->base.h, xx + 8, yy + si->base.h - fill);

			SDLUI_SetColor(SDLUI_Core.theme.col_border);
			SDL_RenderLine(SDLUI_Core.renderer, xx + 8, yy + si->base.h - fill, xx + 8, yy);

			SDL_FRect r = (SDL_FRect) {xx, yy + si->base.h - fill-8, 16, 16};
			r.y = SDLUI_Clamp(r.y, yy, yy + si->base.h - 14);
			SDLUI_Colorize(SDLUI_Core.tex_circle_fill_1, SDLUI_Core.theme.col_scrollbar_thumb);
			SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_1, NULL, &r);
			SDLUI_Colorize(SDLUI_Core.tex_circle, SDLUI_Core.theme.col_white);
			SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_circle, NULL, &r);
		}
	}
}

void SDLUI_Render_CheckBox(SDLUI_Control_CheckBox *chk)
{
	if(chk->base.visible)
	{
		i32 xx = chk->base.x - chk->base.parent->x;
		i32 yy = chk->base.y - chk->base.parent->y;

		SDLUI_SetColor(SDLUI_Core.theme.col_white);

		SDL_FRect r = (SDL_FRect) {xx, yy, chk->base.w, chk->base.h};
		SDL_RenderRect(SDLUI_Core.renderer, &r);

		if(chk->checked)
		{
			SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_tick, NULL, &r);
		}

		if(chk->tex_text != NULL)
		{
			float tex_w, tex_h;
			SDL_GetTextureSize(chk->tex_text,  &tex_w, &tex_h);
			r = (SDL_FRect) {xx + chk->base.w + SDLUI_MARGIN, yy+2, tex_w, tex_h};
			SDL_RenderTexture(SDLUI_Core.renderer, chk->tex_text, NULL, &r);
		}
	}
}

void SDLUI_Render_Text(SDLUI_Control_Text *txt)
{
	if(txt->base.visible)
	{
		if(txt->text.modified)
		{
			txt->base.w = (txt->text.length) * SDLUI_Font.width;
			SDL_Color c = {255, 255, 255, 255};
			SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,txt->text.data, 0, c);
			txt->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
			SDL_DestroySurface(s);
			txt->text.modified = false;
		}

		float xx = txt->base.x - txt->base.parent->x;
		float yy = txt->base.y - txt->base.parent->y;

		SDLUI_SetColor(SDLUI_Core.theme.col_white);
		SDL_FRect r = (SDL_FRect) {xx, yy, txt->base.w, txt->base.h};
		SDL_RenderTexture(SDLUI_Core.renderer, txt->tex_text, NULL, &r);
	}
}

void SDLUI_Render_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
	if(tb->base.visible)
	{
		float xx = tb->base.x - tb->base.parent->x;
		float yy = tb->base.y - tb->base.parent->y;

		SDL_FRect r = (SDL_FRect) {xx, yy, tb->base.w, tb->base.h};
		SDLUI_Colorize(SDLUI_Core.tex_circle, SDLUI_Core.theme.col_white);
		SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_toggle, NULL, &r);

		if(tb->checked)
		{
			r = (SDL_FRect) {xx + 16, yy, tb->base.w - 16, tb->base.h};
			SDLUI_Colorize(SDLUI_Core.tex_circle_fill_2, SDLUI_Core.theme.col_highlight);
			SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_2, NULL, &r);
		}
		else
		{
			r = (SDL_FRect) {xx, yy, tb->base.w - 16, tb->base.h};
			SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_2, NULL, &r);
		}

		SDLUI_Colorize(SDLUI_Core.tex_circle_fill_2, SDLUI_Core.theme.col_white);

		if(tb->tex_text != NULL)
		{
			float tex_w, tex_h;
			SDL_GetTextureSize(tb->tex_text,  &tex_w, &tex_h);
			r = (SDL_FRect) {xx + tb->base.w + SDLUI_MARGIN, yy+2, tex_w, tex_h};
			SDL_RenderTexture(SDLUI_Core.renderer, tb->tex_text, NULL, &r);
		}
	}
}

void SDLUI_Render_RadioButton(SDLUI_Control_RadioButton *rb)
{
	if(rb->base.visible)
	{
		float xx = rb->base.x - rb->base.parent->x;
		float yy = rb->base.y - rb->base.parent->y;

		SDLUI_Colorize(SDLUI_Core.tex_circle, SDLUI_Core.theme.col_white);
		SDL_FRect r = (SDL_FRect) {xx, yy, rb->base.w, rb->base.h};
		SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_circle, NULL, &r);

		if(rb->checked)
		{
			SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_2, NULL, &r);
		}

		if(rb->tex_text != NULL)
		{
			float tex_w, tex_h;
			SDL_GetTextureSize(rb->tex_text,  &tex_w, &tex_h);
			r = (SDL_FRect) {xx + rb->base.w + SDLUI_MARGIN, yy+2, tex_w, tex_h};
			SDL_RenderTexture(SDLUI_Core.renderer, rb->tex_text, NULL, &r);
		}
	}
}

void SDLUI_Render_Tabcontainer(SDLUI_Control_TabContainer *tbc)
{
	if(tbc->base.visible)
	{
		float xx = tbc->base.x - tbc->base.parent->x;
		float yy = tbc->base.y - tbc->base.parent->y;
		float offset = 0;

		SDL_FRect r = (SDL_FRect) {xx, yy, tbc->base.w, tbc->bar_height};
		SDLUI_SetColor(SDLUI_Core.theme.col_inactive_window_bar);
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_Control_Tab *tab;

		for (int i = 0; i < tbc->tabs.size; ++i)
		{
			tab = (SDLUI_Control_Tab*)tbc->tabs.data[i];

			SDLUI_SetColor(SDLUI_Core.theme.col_white);
			r = (SDL_FRect) {SDLUI_MARGIN + xx + offset, yy + SDLUI_MARGIN, tab->base.w, tab->base.h};
			SDL_RenderTexture(SDLUI_Core.renderer, tab->tex_text, NULL, &r);

			if(tab == tbc->active_tab)
			{
				SDLUI_SetColor(SDLUI_Core.theme.col_highlight);
				r.y += 2*SDLUI_MARGIN;
				r.h = 2;
				SDL_RenderFillRect(SDLUI_Core.renderer, &r);
			}

			offset += SDLUI_MARGIN + tab->base.w;
		}

		SDLUI_SetColor(SDLUI_Core.theme.col_window_bg);
		r = (SDL_FRect) {xx, yy + tbc->bar_height, tbc->base.w, tbc->base.h - tbc->bar_height};
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_SetColor(SDLUI_Core.theme.col_grey);
		r = (SDL_FRect) {xx, yy, tbc->base.w, tbc->base.h};
		SDL_RenderRect(SDLUI_Core.renderer, &r);

		tab = (SDLUI_Control_Tab*)tbc->active_tab;

		SDLUI_CONTROL_TYPE type;
		SDLUI_Control *ctrl;

		for (int j = 0; j < tab->children.size; ++j)
		{
			type = tab->children.data[j]->type;
			ctrl = tab->children.data[j];

			ctrl->visible = true;

			if (ctrl->do_process)
			{
				SDLUI_RenderChild(type, ctrl);
			}
		}
	}
}

void SDLUI_Render_ScrollArea(SDLUI_Control_ScrollArea *sa)
{
	if(sa->base.visible)
	{
		float xx = sa->base.x - sa->base.parent->x;
		float yy = sa->base.y - sa->base.parent->y;

		SDL_FRect r;
		SDL_FRect dst = {xx, yy, sa->client_width, sa->client_height};

		float ratio_y = (float)sa->content_height / sa->client_height;
		float ratio_x = (float)sa->content_width / sa->client_width;

		float offset_x = sa->scroll_x * ratio_x  ;
		float offset_y = sa->scroll_y * ratio_y  ;

		offset_x = SDLUI_Clamp(offset_x, 0, sa->content_width -  sa->client_width);
		offset_y = SDLUI_Clamp(offset_y, 0, sa->content_height - sa->client_height);

		SDL_FRect src = {offset_x, offset_y, sa->client_width, sa->client_height};
		SDL_RenderTexture(SDLUI_Core.renderer, sa->tex_rect, &src, &dst);

		if(sa->content_height > sa->base.h)
		{
			// SDL_Log("Scroll Height!");
			// vertical scrollbar
			SDLUI_SetColor(SDLUI_Core.theme.col_scrollbar_track);
			r = (SDL_FRect) {xx + sa->base.w - sa->scrollbar_thickness, yy, sa->scrollbar_thickness, sa->track_size_v};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_scrollbar_thumb);
			sa->thumb_size_v = (float)(sa->track_size_v * (sa->base.h - sa->scrollbar_thickness)) / (float)sa->content_height;

			r = (SDL_FRect) {xx + sa->base.w - sa->scrollbar_thickness+1, yy + sa->scroll_y, sa->scrollbar_thickness-4, sa->thumb_size_v};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		}

		if(sa->content_width > sa->base.w)
		{
			// SDL_Log("Scroll Width!");
			// horizontal scrollbar
			SDLUI_SetColor(SDLUI_Core.theme.col_scrollbar_track);
			r = (SDL_FRect) {xx, yy + sa->base.h - sa->scrollbar_thickness, sa->track_size_h, sa->scrollbar_thickness};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_scrollbar_thumb);
			sa->thumb_size_h = (float)(sa->track_size_h * (sa->base.w - sa->scrollbar_thickness)) / (float)sa->content_width;

			r = (SDL_FRect) {xx + sa->scroll_x, yy + sa->base.h - sa->scrollbar_thickness+1, sa->thumb_size_h, sa->scrollbar_thickness-4};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		}

		// Texture
		if(SDL_GetTextureSize(sa->tex_rect,  &sa->content_width, &sa->content_height) )
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

	
		// Container rect
		SDLUI_SetColor(SDLUI_Core.theme.col_grey);
		r = (SDL_FRect) {xx, yy, sa->base.w, sa->base.h};
		SDL_RenderRect(SDLUI_Core.renderer, &r);
	}
}

void SDLUI_Render_TextBox(SDLUI_Control_TextBox *tbx)
{
	if(tbx->base.visible)
	{
		i32 xx = tbx->base.x - tbx->base.parent->x;
		i32 yy = tbx->base.y - tbx->base.parent->y;

		SDLUI_SetColor(SDLUI_Core.theme.col_textbox_bg);
		SDL_FRect r = (SDL_FRect) {xx, yy, tbx->base.w, tbx->base.h};
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_SetColor(SDLUI_Core.theme.col_grey);
		SDL_RenderRect(SDLUI_Core.renderer, &r);

		if(tbx->text.length > 0)
		{
			float tex_w, tex_h;
			SDL_GetTextureSize(tbx->tex_text,  &tex_w, &tex_h);
			SDL_FRect src = {0, 0, tex_w, tex_h};
			SDL_FRect dst = {xx + SDLUI_MARGIN, yy + SDLUI_MARGIN, src.w, src.h};
			SDL_RenderTexture(SDLUI_Core.renderer, tbx->tex_text, &src, &dst);
		}

		if(tbx->focused && tbx->base.parent == CTRL(SDLUI_Core.active_window) && SDLUI_Core.active_window != NULL)
		{
			r = (SDL_FRect) {xx + SDLUI_MARGIN + ((tbx->cursor_pos - tbx->scroll) * SDLUI_Font.width), yy + 6, 2, tbx->base.h - 12};
			SDLUI_SetColor(SDLUI_Core.theme.col_white);
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		}

		SDL_SetRenderTarget(SDLUI_Core.renderer, tbx->tex_text);
		r = (SDL_FRect) {0, 0, tbx->base.w, tbx->base.h};
		SDLUI_SetColor(SDLUI_Core.theme.col_textbox_bg);
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_SetColor(SDLUI_Core.theme.col_white);
		if(tbx->text.length <= tbx->max_chars)
		{
			SDLUI_DrawText(0, 0, tbx->text.data, tbx->tex_text);
		}
		else
		{
			SDLUI_DrawTextRange(0, 0, tbx->text.data, tbx->scroll, tbx->max_chars, tbx->tex_text);
		}
	}
}

void SDLUI_RenderChild(SDLUI_CONTROL_TYPE type, SDLUI_Control *ctrl)
{
	switch (type)
	{
		case SDLUI_CONTROL_TYPE_BUTTON:
		{
			SDLUI_Render_Button((SDLUI_Control_Button*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_SLIDER_INT:
		{
			SDLUI_Render_SliderInt((SDLUI_Control_SliderInt*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_CHECKBOX:
		{
			SDLUI_Render_CheckBox((SDLUI_Control_CheckBox*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_TOGGLE_BUTTON:
		{
			SDLUI_Render_ToggleButton((SDLUI_Control_ToggleButton*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_RADIO_BUTTON:
		{
			SDLUI_Render_RadioButton((SDLUI_Control_RadioButton*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_TEXT:
		{
			SDLUI_Render_Text((SDLUI_Control_Text*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_TAB_CONTAINER:
		{
			SDLUI_Render_Tabcontainer((SDLUI_Control_TabContainer*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_SCROLL_AREA:
		{
			SDLUI_Render_ScrollArea((SDLUI_Control_ScrollArea*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_TEXTBOX:
		{
			SDLUI_Render_TextBox((SDLUI_Control_TextBox*)ctrl);
		}
		break;

		default:
    		break;
	}
}

void SDLUI_Render_Window(SDLUI_Control_Window *wnd)
{
	if(!wnd->base.do_process)
	{
		return;
	}

	if(wnd->base.visible != wnd->visible_last_frame)
	{
		for (int i = 0; i < wnd->children.size; ++i)
		{
			wnd->children.data[i]->visible = wnd->base.visible;
			wnd->visible_last_frame = wnd->base.visible;
		}

	}

	if(wnd->base.enabled != wnd->enabled_last_frame)
	{
		for (int i = 0; i < wnd->children.size; ++i)
		{
			wnd->children.data[i]->enabled = wnd->base.enabled;
			wnd->enabled_last_frame = wnd->base.enabled;
		}

	}

	if(wnd->base.visible)
	{
		if(wnd == SDLUI_Core.active_window)
		{
			SDL_SetRenderTarget(SDLUI_Core.renderer, wnd->tex_rect);
			SDLUI_SetColor(SDLUI_Core.theme.col_active_window_bar);
			SDL_FRect r = (SDL_FRect) {0, 0, wnd->base.w, 30};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_white);
			float tex_w, tex_h;
			SDL_GetTextureSize(wnd->tex_title,  &tex_w, &tex_h);
			r = (SDL_FRect) {SDLUI_MARGIN, SDLUI_MARGIN, tex_w, tex_h};
			SDL_RenderTexture(SDLUI_Core.renderer, wnd->tex_title, NULL, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_window_bg);
			r = (SDL_FRect) {0, 0 + 30, wnd->base.w, wnd->base.h - 30};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			float mx, my;
			SDL_GetMouseState(&mx, &my);
            #ifdef __APPLE__
            mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
            my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
            #endif
            
			r = (SDL_FRect) {wnd->base.x + wnd->base.w - 30, wnd->base.y, 30, 30};

			if(wnd->has_close_button)
			{
				if(SDLUI_PointInRect(r, mx, my) && !wnd->is_resized)
				{
					SDLUI_SetColor(SDLUI_Core.theme.col_red);
					r = (SDL_FRect) {wnd->base.w - 29, 1, 28, 28};
				}
				else
				{
					SDLUI_SetColor(SDLUI_Core.theme.col_active_window_bar);
					r = (SDL_FRect) {wnd->base.w - 29, 1, 28, 28};
				}

				r = (SDL_FRect) {0 + wnd->base.w - 30, 0, 30, 30};
        			SDL_RenderFillRect(SDLUI_Core.renderer, &r);
				SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_close, NULL, &r);
			}

			SDLUI_CONTROL_TYPE type;
			SDLUI_Control *ctrl;

			for (int j = 0; j < wnd->children.size; ++j)
			{
				type = wnd->children.data[j]->type;
				ctrl = wnd->children.data[j];

				if(ctrl->owned_by_window && ctrl->do_process)
				{
					if(ctrl->x < wnd->base.x + wnd->base.w && ctrl->y < wnd->base.y + wnd->base.h)
					{
						SDLUI_RenderChild(type, ctrl);
					}
				}

				/* Make sure to reset the render target back to the window's
                                 * because the child might have changed it */
				SDL_SetRenderTarget(SDLUI_Core.renderer, wnd->tex_rect);
			}

			SDLUI_SetColor(SDLUI_Core.theme.col_grey);
			r = (SDL_FRect) {0, 0, wnd->base.w, wnd->base.h};
			SDL_RenderRect(SDLUI_Core.renderer, &r);
			r = (SDL_FRect) {wnd->base.x, wnd->base.y, wnd->base.w, wnd->base.h};
			SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
			SDL_RenderTexture(SDLUI_Core.renderer, wnd->tex_rect, NULL, &r);
		}
		else
		{
			SDL_SetRenderTarget(SDLUI_Core.renderer, wnd->tex_rect);
			SDL_FRect r = (SDL_FRect) {0, 0, wnd->base.w, 30};
			SDLUI_SetColor(SDLUI_Core.theme.col_inactive_window_bar);
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_white);
			float tex_w, tex_h;
			SDL_GetTextureSize(wnd->tex_title,  &tex_w, &tex_h);
			r = (SDL_FRect) {SDLUI_MARGIN, SDLUI_MARGIN, tex_w, tex_h};
			SDL_RenderTexture(SDLUI_Core.renderer, wnd->tex_title, NULL, &r);

			if(wnd->has_close_button)
			{
				r = (SDL_FRect) {0 + wnd->base.w - 30, 0, 30, 30};
				SDL_RenderTexture(SDLUI_Core.renderer, SDLUI_Core.tex_close, NULL, &r);
			}

			SDLUI_SetColor(SDLUI_Core.theme.col_grey);
			r = (SDL_FRect) {0, 0, wnd->base.w, wnd->base.h};
			SDL_RenderRect(SDLUI_Core.renderer, &r);

			r = (SDL_FRect) {wnd->base.x, wnd->base.y, wnd->base.w, wnd->base.h};
			SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
			SDL_RenderTexture(SDLUI_Core.renderer, wnd->tex_rect, NULL, &r);
		}
	}
}

void SDLUI_ForceReDraw()
{
	SDLUI_Control_Window *wnd;
	SDLUI_Control_Window *aw = SDLUI_Core.active_window;

	for (int i = 0; i < SDLUI_Window_Collection.size; ++i)
	{
		wnd = (SDLUI_Control_Window*)SDLUI_Window_Collection.data[i];
		SDLUI_Core.active_window = wnd;
		SDLUI_Render_Window(wnd);
	}

	SDLUI_Core.active_window = aw;
}

void SDLUI_Render()
{
	SDLUI_Control_Window *wnd;

	static bool once = false;
	if(!once)
	{
		SDLUI_ForceReDraw();
		once = true;
	}

	for (int i = 0; i < SDLUI_Window_Collection.size; ++i)
	{
		wnd = (SDLUI_Control_Window*)SDLUI_Window_Collection.data[i];
		SDLUI_Render_Window(wnd);
	}

	SDLUI_MouseStateReset();
}

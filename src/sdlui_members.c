void SDLUI_String_create(SDLUI_String* self, char const *str)
{
	self->length = strlen(str);
	self->capacity = ((self->length / SDLUI_STRING_CAPACITY) + 1) * SDLUI_STRING_CAPACITY;
	self->data = (char*)malloc(self->capacity);
	memcpy(self->data, str, self->length);
	memset(self->data + self->length, 0, 1);

	self->modified = false;
}

void SDLUI_String_destroy(SDLUI_String* self)
{
	free(self->data);
	self->data = NULL;
}

void SDLUI_String_modify(SDLUI_String* self, const char *str)
{
	i32 new_length = strlen(str);
	if(new_length >= self->capacity)
	{
		self->capacity = ((new_length / SDLUI_STRING_CAPACITY) + 1) * SDLUI_STRING_CAPACITY;
		self->data = (char*)realloc(self->data, self->capacity);
		memcpy(self->data, str, new_length);
		memset(self->data + new_length, 0, 1);
		self->length = new_length;
	}
	else
	{
		memcpy(self->data, str, new_length);
		memset(self->data + new_length, 0, self->capacity - new_length);
		self->length = new_length;
	}

	self->modified = true;
}

bool SDLUI_String_insert_char(SDLUI_String* self, char c, i32 pos)
{
	if(pos < 0 || pos > self->length)
	{
		return false;
	}

	i32 new_length = self->length + 1;
	if(new_length >= self->capacity)
	{
		self->capacity = ((new_length / SDLUI_STRING_CAPACITY) + 1) * SDLUI_STRING_CAPACITY;
		self->data = (char*)realloc(self->data, self->capacity);
	}

	if(pos == 0)
	{
		memmove(self->data + 1, self->data, self->length);
		memset(self->data, c, 1);
	}
	else if(pos == self->length)
	{
		memset(self->data + self->length, c, 1);
	}
	else
	{
		memmove(self->data + pos + 1, self->data + pos, self->length - pos);
		memset(self->data + pos, c, 1);
	}

	self->length = new_length;
	memset(self->data + self->length, 0, 1);
	self->modified = true;
	return true;
}

bool SDLUI_String_delete_char(SDLUI_String* self, i32 pos)
{
	if(pos < 0 || pos > self->length - 1)
	{
		return false;
	}

	if(pos == self->length - 1)
	{
		memset(self->data + self->length - 1, 0, 1);
	}
	else
	{
		memmove(self->data + pos, self->data + pos + 1, self->length - pos - 1);
		memset(self->data + self->length - 1, 0, 1);
	}

	self->length--;
	self->modified = true;
	return true;
}

void SDLUI_ArrayOfControls_create(SDLUI_ArrayOfControls* self)
{
	self->capacity = SDLUI_COLLECTION_CHUNK;
	self->data = (SDLUI_Control**)malloc(self->capacity * sizeof(SDLUI_Control*));
	self->size = 0;
}

void SDLUI_ArrayOfControls_ensure_capacity(SDLUI_ArrayOfControls* self)
{
	if(self->size >= self->capacity)
	{
		self->capacity += SDLUI_COLLECTION_CHUNK;
		self->data = (SDLUI_Control**)realloc(self->data, self->capacity * sizeof(SDLUI_Control*));
	}
}

void SDLUI_ArrayOfControls_push(SDLUI_ArrayOfControls* self, SDLUI_Control *elem)
{
	self->ensure_capacity();
	(self->data)[self->size] = elem;
	self->size++;
}
#define SDLUI_ArrayOfControls_push(self, elem) SDLUI_ArrayOfControls_push(self, CTRL(elem))

void SDLUI_ArrayOfControls_pop(SDLUI_ArrayOfControls* self, SDLUI_Control *elem)
{
	for (int i = 0; i < self->size; ++i)
	{
		if((i < self->size - 1) && elem == (self->data)[i])
		{
			i32 num_elements = self->size - i - 1;
			memmove(self->data + i, self->data + i + 1, num_elements * sizeof(self->data));
			self->size--;
			return;
		}
	}
}
#define SDLUI_ArrayOfControls_pop(self, elem) SDLUI_ArrayOfControls_pop(self, CTRL(elem))

void SDLUI_ArrayOfControls_to_back(SDLUI_ArrayOfControls* self, SDLUI_Control *elem)
{
	for (int i = 0; i < self->size; ++i)
	{
		if((i < self->size - 1) && elem == (self->data)[i])
		{
			i32 num_elements = self->size - i - 1;
			memmove(self->data + i, self->data + i + 1, num_elements * sizeof(self->data));
			(self->data)[self->size - 1] = elem;
			return;
		}
	}
}
#define SDLUI_ArrayOfControls_to_back(self, elem) SDLUI_ArrayOfControls_to_back(self, CTRL(elem))

void SDLUI_TabContainer_add_tab(SDLUI_Control_TabContainer* self, char const *text)
{
	SDLUI_Control_Tab *tab = (SDLUI_Control_Tab*)malloc(sizeof(SDLUI_Control_Tab));
	tab->base.type = SDLUI_CONTROL_TYPE_TAB;
	tab->text.create(text);
	tab->base.w = (tab->text.length) * SDLUI_Font.width;
	tab->base.h = SDLUI_Font.height;
	tab->children.create();
	tab->index = self->tabs.size;
	SDL_Color c = {255, 255, 255, 255};
	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,tab->text.data, 0, c);
	tab->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
	SDL_DestroySurface(s);
	self->tabs.push(CTRL(tab));
	self->active_tab = tab;
}

void SDLUI_TabContainer_set_active_tab(SDLUI_Control_TabContainer* self, i32 index)
{
	if(index < 0 || index > self->tabs.size - 1)
	{
		return;
	}
	self->active_tab = (SDLUI_Control_Tab*)self->tabs.data[index];
}

void SDLUI_TabContainer_add_child (SDLUI_Control_TabContainer* self, i32 tab_index, SDLUI_Control *ctrl)
{
	SDLUI_Control_Window *wnd = (SDLUI_Control_Window*)self->base.parent;
	SDLUI_Control_Tab *tab;
	SDLUI_Control *cur_ctrl;
	for (int i = 0; i < wnd->children.size; ++i)
	{
		cur_ctrl = wnd->children.data[i];
		if(cur_ctrl == ctrl)
		{
			for (int j = 0; j < self->tabs.size; ++j)
			{
				tab = (SDLUI_Control_Tab*)self->tabs.data[j];
				if(tab->index == tab_index)
				{
					ctrl->visible = false;
					tab->children.push(ctrl);
					ctrl->owned_by_window = false;
				}
			}
		}
	}
}
#define SDLUI_TabContainer_add_child (self, tab_index, ctrl) SDLUI_TabContainer_add_child((self), (tab_index), CTRL(ctrl))

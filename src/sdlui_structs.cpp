struct SDLUI_String
{
	i32 capacity = 0;
	i32 length = 0;
	bool modified = false;
	char *data;

	inline void create(char const *str) {
		SDLUI_String_create(this, str);
	}

	inline void destroy() {
		SDLUI_String_destroy(this);
	}

	inline void modify(const char *str) {
		SDLUI_String_modify(this, str);
	}

	inline bool insert_char(char c, i32 pos) {
		return SDLUI_String_insert_char(this,c, pos);
	}

	inline bool delete_char(i32 pos) {
		return SDLUI_String_delete_char(this, pos);
	}

};

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

struct SDLUI_Theme
{
	SDL_Color col_window_bg = {30, 30, 30, 255};
	SDL_Color col_border = {68, 68, 68, 255};
	SDL_Color col_active_window_bar = {0, 0, 0, 255};
	SDL_Color col_inactive_window_bar = {22, 22, 22, 255};
	SDL_Color col_button_normal = {28, 28, 28, 255};
	SDL_Color col_button_click = {0, 0, 0, 255};
	SDL_Color col_highlight = {0, 120, 215, 255};
	SDL_Color col_test = {255, 0, 225, 255};
	SDL_Color col_scrollbar_thumb = {26, 26, 26, 255};
	SDL_Color col_scrollbar_track = {56, 56, 56, 255};
	SDL_Color col_white = {255, 255, 255, 255};
	SDL_Color col_grey = {58, 58, 58, 255};
	SDL_Color col_red = {222, 17, 35, 255};
	SDL_Color col_black = {0, 0, 0, 255};
	SDL_Color col_list_bg = {32, 32, 32, 255};
	SDL_Color col_textbox_bg = {22, 22, 22, 255};
};

struct SDLUI_Control
{
	SDLUI_CONTROL_TYPE type;
	float x;
	float y;
	float w;
	float h;
	bool visible = true;
	bool enabled = true;
	bool owned_by_window = true;
	bool do_process = false;
	SDLUI_Control *parent;
};

struct __SDLUI_Font
{
	TTF_Font *handle;
	i32 size = 13;
	i32 width;
	i32 height;
	char ascii[96];
	SDL_Texture *tex_font;
}SDLUI_Font;

struct SDLUI_ArrayOfControls {
	i32 capacity;
	i32 size;
	SDLUI_Control **data;

	inline void create() {
		SDLUI_ArrayOfControls_create(this);
	}

	inline void ensure_capacity(){
		SDLUI_ArrayOfControls_ensure_capacity(this);
	}

	inline void push(SDLUI_Control *elem) {
		SDLUI_ArrayOfControls_push(this, elem);
	}

	inline void pop(SDLUI_Control *elem) {
		SDLUI_ArrayOfControls_pop(this, elem);
	}

	inline void to_back(SDLUI_Control *elem) {
		SDLUI_ArrayOfControls_to_back(this, elem);	
	}
};

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

SDLUI_ArrayOfControls SDLUI_Window_Collection;

struct SDLUI_Control_Window : SDLUI_Control
{
	float drag_x;
	float drag_y;
	bool is_dragged = false;
	bool is_resized = false;
	bool is_hovered = false;
	SDLUI_RESIZE_DIRECTION resize_direction;
	bool visible_last_frame = false;
	bool enabled_last_frame = false;
	SDLUI_String title;
	SDL_Texture *tex_title;
	SDL_Texture *tex_rect;
	bool active;
	bool has_close_button;
	bool can_be_resized;
	SDLUI_ArrayOfControls children;
};

struct SDLUI_Control_Button : SDLUI_Control
{
	SDLUI_String text;
	SDLUI_ALIGN align;
	SDLUI_BUTTON_STATE state;
	SDL_Texture *tex_text;
};

// Generisize for different int types?
struct SDLUI_Control_SliderInt : SDLUI_Control
{
	i32 min;
	i32 max;
	i32 value;
	SDLUI_ORIENTATION orientation;
	float thumb_size;
	bool is_changing;
};

struct SDLUI_Control_CheckBox : SDLUI_Control
{
	bool checked;
	SDLUI_String text;
	SDL_Texture *tex_text;
};

struct SDLUI_Control_ToggleButton : SDLUI_Control
{
	bool checked;
	SDLUI_String text;
	SDL_Texture *tex_text;
};

struct SDLUI_Control_RadioButton : SDLUI_Control
{
	SDLUI_ArrayOfControls *group;
	bool checked;
	bool checked_changed;
	SDLUI_String text;
	SDL_Texture *tex_text;
};

struct SDLUI_Control_TextBox : SDLUI_Control
{
	SDLUI_String text;
	SDL_Texture *tex_text;
	i32 select_start;
	i32 select_end;
	i32 cursor_pos;
	i32 max_chars;
	i32 scroll;
	bool focused;
};

struct __SDLUI_Core
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	i32 window_width;
	i32 window_height;
    float texture_window_hdpi_ratio_x;
    float texture_window_hdpi_ratio_y;
    
	u8 mouse_current_frame[5] = {0};
	u8 mouse_last_frame[5] = {0};
	float mouse_wheel_y;
	float mouse_wheel_x;
	SDLUI_Theme theme;
	SDLUI_Control_Window *active_window;

	SDL_Event e;

	SDL_Cursor *cursor_arrow;
	SDL_Cursor *cursor_ibeam;
	SDL_Cursor *cursor_size_we;
	SDL_Cursor *cursor_size_ns;

	SDL_Cursor *cursor_size_nwse;
	SDL_Cursor *cursor_size_nesw;

	SDL_Texture *tex_tick;
	SDL_Texture *tex_circle;
	SDL_Texture *tex_circle_fill_1;
	SDL_Texture *tex_circle_fill_2;
	SDL_Texture *tex_toggle;
	SDL_Texture *tex_close;
}SDLUI_Core;

struct SDLUI_Control_Tab : SDLUI_Control
{
	i32 index;
	SDLUI_String text;
	SDL_Texture *tex_text;
	SDLUI_ArrayOfControls children;
};

struct SDLUI_Control_TabContainer : SDLUI_Control
{
	float bar_height;
	SDLUI_ArrayOfControls tabs;
	SDLUI_Control_Tab *active_tab;
	SDLUI_ORIENTATION orientation;
	inline void add_tab(char const *text) {
		SDLUI_TabContainer_add_tab(this, text);
	}

	inline void set_active_tab(i32 index) {
		SDLUI_TabContainer_set_active_tab(this, index);
	}

	inline void add_child(i32 tab_index, SDLUI_Control *ctrl) {
		SDLUI_TabContainer_add_child(this, tab_index, ctrl);
	}

};


void SDLUI_TabContainer_add_tab(SDLUI_Control_TabContainer* self, char const *text)
{
	SDLUI_Control_Tab *tab = (SDLUI_Control_Tab*)malloc(sizeof(SDLUI_Control_Tab));
	tab->type = SDLUI_CONTROL_TYPE_TAB;
	tab->text.create(text);
	tab->w = (tab->text.length) * SDLUI_Font.width;
	tab->h = SDLUI_Font.height;
	tab->children.create();
	tab->index = self->tabs.size;
	SDL_Color c = {255, 255, 255, 255};
	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,tab->text.data, 0, c);
	tab->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
	SDL_DestroySurface(s);
	self->tabs.push(tab);
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

void SDLUI_TabContainer_add_child(SDLUI_Control_TabContainer* self, i32 tab_index, SDLUI_Control *ctrl)
{
	SDLUI_Control_Window *wnd = (SDLUI_Control_Window*)self->parent;
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

struct SDLUI_Control_Label : SDLUI_Control
{
	SDL_Texture *tex_text;
};

struct SDLUI_Control_Text : SDLUI_Control
{
	SDLUI_String text;
	bool modified;
	SDL_Texture *tex_text;
};

struct SDLUI_Control_ScrollArea : SDLUI_Control
{
	float content_width;
	float content_height;
	float client_width;
	float client_height;
	float scrollbar_thickness;
	float track_size_h;
	float track_size_v;
	float thumb_size_h;
	float thumb_size_v;
	float scroll_x;
	float scroll_y;
	bool is_changing_v;
	bool is_changing_h;
	SDL_Texture *tex_rect;
};

struct SDLUI_Control_List : SDLUI_Control
{
	SDLUI_Control_ScrollArea *scroll_area;
	i32 num_items;
	i32 selected_index;
	i32 max_string_width;
	const char *cur_item;
};

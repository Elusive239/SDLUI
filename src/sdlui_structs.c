typedef struct SDLUI_String
{
	i32 capacity ;
	i32 length ;
	bool modified ;
	char *data;

	#ifdef __cplusplus 
	inline void create(char const *str) {
		this->capacity = 0;
		this->length = 0;
		this->modified = false;
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
	#endif
} SDLUI_String;

typedef struct SDLUI_Theme
{
	SDL_Color col_window_bg ;
	SDL_Color col_border ;
	SDL_Color col_active_window_bar ;
	SDL_Color col_inactive_window_bar ;
	SDL_Color col_button_normal ;
	SDL_Color col_button_click ;
	SDL_Color col_highlight ;
	SDL_Color col_test ;
	SDL_Color col_scrollbar_thumb ;
	SDL_Color col_scrollbar_track ;
	SDL_Color col_white ;
	SDL_Color col_grey ;
	SDL_Color col_red ;
	SDL_Color col_black ;
	SDL_Color col_list_bg ;
	SDL_Color col_textbox_bg ;
} SDLUI_Theme;

const SDLUI_Theme DEFAULT_THEME = {
	.col_window_bg = {30, 30, 30, 255},
	.col_border = {68, 68, 68, 255},
	.col_active_window_bar = {0, 0, 0, 255},
	.col_inactive_window_bar = {22, 22, 22, 255},
	.col_button_normal = {28, 28, 28, 255},
	.col_button_click = {0, 0, 0, 255},
	.col_highlight = {0, 120, 215, 255},
	.col_test = {255, 0, 225, 255},
	.col_scrollbar_thumb = {26, 26, 26, 255},
	.col_scrollbar_track = {56, 56, 56, 255},
	.col_white = {255, 255, 255, 255},
	.col_grey = {58, 58, 58, 255},
	.col_red = {222, 17, 35, 255},
	.col_black = {0, 0, 0, 255},
	.col_list_bg = {32, 32, 32, 255},
	.col_textbox_bg = {22, 22, 22, 255},
};

typedef struct SDLUI_Control
{
	SDLUI_CONTROL_TYPE type;
	float x;
	float y;
	float w;
	float h;
	bool visible ;
	bool enabled ;
	bool owned_by_window ;
	bool do_process ;
	struct SDLUI_Control *parent;
} SDLUI_Control;

typedef struct __SDLUI_Font
{
	TTF_Font *handle;
	i32 size;
	i32 width;
	i32 height;
	char ascii[96];
	SDL_Texture *tex_font;
} __SDLUI_Font;

typedef struct SDLUI_ArrayOfControls {
	i32 capacity;
	i32 size;
	SDLUI_Control **data;

	#ifdef __cplusplus
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
	#endif
} SDLUI_ArrayOfControls;

typedef struct SDLUI_Control_Window 
{
	struct SDLUI_Control base;
	float drag_x;
	float drag_y;
	bool is_dragged ;
	bool is_resized ;
	bool is_hovered ;
	SDLUI_RESIZE_DIRECTION resize_direction;
	bool visible_last_frame ;
	bool enabled_last_frame ;
	SDLUI_String title;
	SDL_Texture *tex_title;
	SDL_Texture *tex_rect;
	bool active;
	bool has_close_button;
	bool can_be_resized;
	SDLUI_ArrayOfControls children;
} SDLUI_Control_Window;

typedef struct SDLUI_Control_Button 
{
	struct SDLUI_Control base;
	SDLUI_String text;
	SDLUI_ALIGN align;
	SDLUI_BUTTON_STATE state;
	SDL_Texture *tex_text;
} SDLUI_Control_Button;

// Generisize for different int types?
typedef struct SDLUI_Control_SliderInt 
{
	struct SDLUI_Control base;
	i32 min;
	i32 max;
	i32 value;
	SDLUI_ORIENTATION orientation;
	float thumb_size;
	bool is_changing;
} SDLUI_Control_SliderInt;

typedef struct SDLUI_Control_CheckBox 
{
	struct SDLUI_Control base;
	bool checked;
	SDLUI_String text;
	SDL_Texture *tex_text;
} SDLUI_Control_CheckBox;

typedef struct SDLUI_Control_ToggleButton 
{
	struct SDLUI_Control base;
	bool checked;
	SDLUI_String text;
	SDL_Texture *tex_text;
} SDLUI_Control_ToggleButton;

typedef struct SDLUI_Control_RadioButton 
{
	struct SDLUI_Control base;
	SDLUI_ArrayOfControls *group;
	bool checked;
	bool checked_changed;
	SDLUI_String text;
	SDL_Texture *tex_text;
} SDLUI_Control_RadioButton;

typedef struct SDLUI_Control_TextBox 
{
	struct SDLUI_Control base;
	SDLUI_String text;
	SDL_Texture *tex_text;
	i32 select_start;
	i32 select_end;
	i32 cursor_pos;
	i32 max_chars;
	i32 scroll;
	bool focused;
} SDLUI_Control_TextBox;

typedef struct __SDLUI_Core
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	i32 window_width;
	i32 window_height;
    float texture_window_hdpi_ratio_x;
    float texture_window_hdpi_ratio_y;
    
	u8 mouse_current_frame[5];
	u8 mouse_last_frame[5] ;
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
} __SDLUI_Core;

typedef struct SDLUI_Control_Tab 
{
	struct SDLUI_Control base;
	i32 index;
	SDLUI_String text;
	SDL_Texture *tex_text;
	SDLUI_ArrayOfControls children;
} SDLUI_Control_Tab;

typedef struct SDLUI_Control_TabContainer 
{
	struct SDLUI_Control base;
	float bar_height;
	SDLUI_ArrayOfControls tabs;
	SDLUI_Control_Tab *active_tab;
	SDLUI_ORIENTATION orientation;
	#ifdef __cplusplus
	inline void add_tab(char const *text) {
		SDLUI_TabContainer_add_tab(this, text);
	}

	inline void set_active_tab(i32 index) {
		SDLUI_TabContainer_set_active_tab(this, index);
	}

	inline void add_child(i32 tab_index, SDLUI_Control *ctrl) {
		SDLUI_TabContainer_add_child(this, tab_index, ctrl);
	}
	#endif
} SDLUI_Control_TabContainer;

typedef struct SDLUI_Control_Label 
{
	struct SDLUI_Control base;
	SDL_Texture *tex_text;
} SDLUI_Control_Label;

typedef struct SDLUI_Control_Text 
{
	struct SDLUI_Control base;
	SDLUI_String text;
	bool modified;
	SDL_Texture *tex_text;
} SDLUI_Control_Text;

typedef struct SDLUI_Control_ScrollArea 
{
	struct SDLUI_Control base;
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
} SDLUI_Control_ScrollArea;

typedef struct SDLUI_Control_List 
{
	struct SDLUI_Control base;
	SDLUI_Control_ScrollArea *scroll_area;
	i32 num_items;
	i32 selected_index;
	i32 max_string_width;
	const char *cur_item;
} SDLUI_Control_List ;

SDLUI_ArrayOfControls SDLUI_Window_Collection;
__SDLUI_Core SDLUI_Core;
__SDLUI_Font SDLUI_Font;
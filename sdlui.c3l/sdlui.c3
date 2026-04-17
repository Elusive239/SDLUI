module sdlui;
import sdl3;
import sdl3_ttf;
import sdl3_image;

constdef SDLUIButtonState : int 
{
	SDLUI_BUTTON_STATE_NORMAL,
	SDLUI_BUTTON_STATE_HOVER,
	SDLUI_BUTTON_STATE_CLICK
} 

constdef SDLUIAlign : int 
{
	SDLUI_ALIGN_LEFT = 0,
	SDLUI_ALIGN_CENTER = 1,
	SDLUI_ALIGN_RIGHT = 2
} 

constdef SDLUIOrientation : int
{
	SDLUI_ORIENTATION_HORIZONTAL,
	SDLUI_ORIENTATION_VERTICAL
}

constdef SDLUIMousebutton : int 
{
	SDLUI_MOUSEBUTTON_NONE,
	SDLUI_MOUSEBUTTON_PRESSED,
	SDLUI_MOUSEBUTTON_HELD,
	SDLUI_MOUSEBUTTON_RELEASED
} 

constdef SDLUIControlType : int
{
	SDLUI_CONTROL_TYPE_WINDOW,
	SDLUI_CONTROL_TYPE_BUTTON,
	SDLUI_CONTROL_TYPE_SLIDER_INT,
	SDLUI_CONTROL_TYPE_CHECKBOX,
	SDLUI_CONTROL_TYPE_TOGGLE_BUTTON,
	SDLUI_CONTROL_TYPE_TAB_CONTAINER,
	SDLUI_CONTROL_TYPE_TAB,
	SDLUI_CONTROL_TYPE_LABEL,
	SDLUI_CONTROL_TYPE_TEXT,
	SDLUI_CONTROL_TYPE_RADIO_BUTTON,
	SDLUI_CONTROL_TYPE_SCROLL_AREA,
	SDLUI_CONTROL_TYPE_LIST,
	SDLUI_CONTROL_TYPE_TEXTBOX
}

constdef SDLUIResizeDirection : int
{
	SDLUI_RESIZE_NONE,
	SDLUI_RESIZE_LEFT,
	SDLUI_RESIZE_TOP,
	SDLUI_RESIZE_RIGHT,
	SDLUI_RESIZE_BOTTOM,

	SDLUI_RESIZE_LEFT_TOP,
	SDLUI_RESIZE_RIGHT_TOP,
	SDLUI_RESIZE_LEFT_BOTTOM,
	SDLUI_RESIZE_RIGHT_BOTTOM
}

struct SDLUI_String
{
	int capacity ;
	int length ;
	bool modified ;
	char *data;
} 

macro void SDLUI_String.create(&this, char *str) {
	this.capacity = 0;
	this.length = 0;
	this.modified = false;
	string_create(this, str);
}
macro void SDLUI_String.destroy(&this) {
	string_destroy(this);
}
macro void SDLUI_String.modify(&this, char *str) {
	string_modify(this, str);
}
macro bool SDLUI_String.insert_char(&this, char c, int pos) {
	return string_insert_char(this,c, pos);
}
macro bool SDLUI_String.delete_char(&this, int pos) {
	return string_delete_char(this, pos);
}

struct SDLUI_Theme
{
	SdlColor col_window_bg ;
	SdlColor col_border ;
	SdlColor col_active_window_bar ;
	SdlColor col_inactive_window_bar ;
	SdlColor col_button_normal ;
	SdlColor col_button_click ;
	SdlColor col_highlight ;
	SdlColor col_test ;
	SdlColor col_scrollbar_thumb ;
	SdlColor col_scrollbar_track ;
	SdlColor col_white ;
	SdlColor col_grey ;
	SdlColor col_red ;
	SdlColor col_black ;
	SdlColor col_list_bg ;
	SdlColor col_textbox_bg ;
} 

extern const SDLUI_Theme DEFAULT_THEME @cname("DEFAULT_THEME");
// = {
	// .col_window_bg = {30, 30, 30, 255},
	// .col_border = {68, 68, 68, 255},
	// .col_active_window_bar = {0, 0, 0, 255},
	// .col_inactive_window_bar = {22, 22, 22, 255},
	// .col_button_normal = {28, 28, 28, 255},
	// .col_button_click = {0, 0, 0, 255},
	// .col_highlight = {0, 120, 215, 255},
	// .col_test = {255, 0, 225, 255},
	// .col_scrollbar_thumb = {26, 26, 26, 255},
	// .col_scrollbar_track = {56, 56, 56, 255},
	// .col_white = {255, 255, 255, 255},
	// .col_grey = {58, 58, 58, 255},
	// .col_red = {222, 17, 35, 255},
	// .col_black = {0, 0, 0, 255},
	// .col_list_bg = {32, 32, 32, 255},
	// .col_textbox_bg = {22, 22, 22, 255},
// };

struct SDLUI_Control
{
	SDLUIControlType type;
	float x;
	float y;
	float w;
	float h;
	bool visible ;
	bool enabled ;
	bool owned_by_window ;
	bool do_process ;
	SDLUI_Control *parent;
} 

struct SDLUI_Font
{
	SdlTTFFont *handle;
	int size;
	int width;
	int height;
	char[96] ascii;
	SdlTexture *tex_font;
} 

struct SDLUI_ArrayOfControls {
	int capacity;
	int size;
	SDLUI_Control **data;
} 

macro void SDLUI_ArrayOfControls.create(&this) {
	arrayOfControls_create(this);
}
macro void SDLUI_ArrayOfControls.ensure_capacity(&this){
	arrayOfControls_ensure_capacity(this);
}
macro void SDLUI_ArrayOfControls.push(&this, SDLUI_Control *elem) {
	arrayOfControls_push(this, elem);
}
macro void SDLUI_ArrayOfControls.pop(&this, SDLUI_Control *elem) {
	arrayOfControls_pop(this, elem);
}
macro void SDLUI_ArrayOfControls.to_back(&this, SDLUI_Control *elem) {
	arrayOfControls_to_back(this, elem);	
}

struct SDLUI_Control_Window 
{
	inline SDLUI_Control base;
	float drag_x;
	float drag_y;
	bool is_dragged ;
	bool is_resized ;
	bool is_hovered ;
	SDLUIResizeDirection resize_direction;
	bool visible_last_frame ;
	bool enabled_last_frame ;
	SDLUI_String title;
	SdlTexture *tex_title;
	SdlTexture *tex_rect;
	bool active;
	bool has_close_button;
	bool can_be_resized;
	SDLUI_ArrayOfControls children;
} 

struct SDLUI_Control_Button 
{
	inline SDLUI_Control base;
	SDLUI_String text;
	SDLUIAlign align;
	SDLUIButtonState state;
	SdlTexture *tex_text;
} 

// Generisize for different int types?
struct SDLUI_Control_SliderInt 
{
	inline SDLUI_Control base;
	int min;
	int max;
	int value;
	SDLUIOrientation orientation;
	float thumb_size;
	bool is_changing;
} 

struct SDLUI_Control_CheckBox 
{
	inline SDLUI_Control base;
	bool checked;
	SDLUI_String text;
	SdlTexture *tex_text;
}

struct SDLUI_Control_ToggleButton 
{
	inline SDLUI_Control base;
	bool checked;
	SDLUI_String text;
	SdlTexture *tex_text;
}

struct SDLUI_Control_RadioButton 
{
	inline SDLUI_Control base;
	SDLUI_ArrayOfControls *group;
	bool checked;
	bool checked_changed;
	SDLUI_String text;
	SdlTexture *tex_text;
}

struct SDLUI_Control_TextBox 
{
	inline SDLUI_Control base;
	SDLUI_String text;
	SdlTexture *tex_text;
	int select_start;
	int select_end;
	int cursor_pos;
	int max_chars;
	int scroll;
	bool focused;
} 

struct SDLUI_Core
{
	SdlWindow *window;
	SdlRenderer *renderer;
	int window_width;
	int window_height;
    float texture_window_hdpi_ratio_x;
    float texture_window_hdpi_ratio_y;
    
	char[5] mouse_current_frame;
	char[5] mouse_last_frame ;
	float mouse_wheel_y;
	float mouse_wheel_x;
	SDLUI_Theme theme;
	SDLUI_Control_Window *active_window;

	SdlEvent e;

	SdlCursor *cursor_arrow;
	SdlCursor *cursor_ibeam;
	SdlCursor *cursor_size_we;
	SdlCursor *cursor_size_ns;

	SdlCursor *cursor_size_nwse;
	SdlCursor *cursor_size_nesw;

	SdlTexture *tex_tick;
	SdlTexture *tex_circle;
	SdlTexture *tex_circle_fill_1;
	SdlTexture *tex_circle_fill_2;
	SdlTexture *tex_toggle;
	SdlTexture *tex_close;
} 

struct SDLUI_Control_Tab 
{
	inline SDLUI_Control base;
	int index;
	SDLUI_String text;
	SdlTexture *tex_text;
	SDLUI_ArrayOfControls children;
}

struct SDLUI_Control_TabContainer 
{
	inline SDLUI_Control base;
	float bar_height;
	SDLUI_ArrayOfControls tabs;
	SDLUI_Control_Tab *active_tab;
	SDLUIOrientation orientation;
} 

fn void SDLUI_Control_TabContainer.add_tab(&this, char *text) {
	tab_container_add_tab(this, text);
}

fn void SDLUI_Control_TabContainer.set_active_tab(&this, int index) {
	tab_container_set_active_tab(this, index);
}

fn void SDLUI_Control_TabContainer.add_child(&this, int tab_index, SDLUI_Control *ctrl) {
	tab_container_add_child(this, tab_index, ctrl);
}

struct SDLUI_Control_Label 
{
	inline SDLUI_Control base;
	SdlTexture *tex_text;
}

struct SDLUI_Control_Text 
{
	inline SDLUI_Control base;
	SDLUI_String text;
	bool modified;
	SdlTexture *tex_text;
} 

struct SDLUI_Control_ScrollArea 
{
	inline SDLUI_Control base;
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
	SdlTexture *tex_rect;
} 

struct SDLUI_Control_List 
{
	inline SDLUI_Control base;
	SDLUI_Control_ScrollArea *scroll_area;
	int num_items;
	int selected_index;
	int max_string_width;
	char *cur_item;
} 

extern SDLUI_ArrayOfControls window_collection @cname("SDLUI_Window_Collection");
extern SDLUI_Core core @cname("SDLUI_Core");
extern SDLUI_Font font @cname("SDLUI_Font");

// sdlui_create
extern fn SDLUI_Control_Window *createWindow(int x, int y, int w, int h, char  *title) @cname("SDLUI_CreateWindow");
extern fn SDLUI_Control_Button *createButton(SDLUI_Control_Window *wnd, int x, int y, char  *text) @cname("SDLUI_CreateButton");
extern fn SDLUI_Control_SliderInt *createSliderInt(SDLUI_Control_Window *wnd, int x, int y, int min, int max, int value, SDLUIOrientation orientation ) @cname("SDLUI_CreateSliderInt");
extern fn SDLUI_Control_CheckBox *createCheckBox(SDLUI_Control_Window *wnd, int x, int y, char  *text, bool checked) @cname("SDLUI_CreateCheckBox");
extern fn SDLUI_Control_Text *createText(SDLUI_Control_Window *wnd, int x, int y, char  *text) @cname("SDLUI_CreateText");
extern fn SDLUI_Control_ToggleButton *createToggleButton(SDLUI_Control_Window *wnd, int x, int y, char  *text, bool checked) @cname("SDLUI_CreateToggleButton");
extern fn SDLUI_ArrayOfControls createRadioButtonGroup() @cname("SDLUI_CreateRadioButtonGroup");
extern fn SDLUI_Control_RadioButton *createRadioButton(SDLUI_Control_Window *wnd, SDLUI_ArrayOfControls* group, int x, int y, char  *text, bool checked) @cname("SDLUI_CreateRadioButton");
extern fn SDLUI_Control_TabContainer *createTabContainer(SDLUI_Control_Window *wnd, int x, int y, int w, int h) @cname("SDLUI_CreateTabContainer");
extern fn SDLUI_Control_ScrollArea *createScrollArea(SDLUI_Control_Window *wnd, int x, int y, int w, int h, SdlTexture *tex) @cname("SDLUI_CreateScrollArea");
extern fn SDLUI_Control_List *createList(SDLUI_Control_Window *wnd, SDLUI_Control_ScrollArea *sa, int num_items) @cname("SDLUI_CreateList");
extern fn SDLUI_Control_TextBox *createTextBox(SDLUI_Control_Window *wnd, int x, int y, int w) @cname("SDLUI_CreateTextBox");

// sdlui_event
extern fn void eventHandler(SdlEvent* ev) @cname("SDLUI_EventHandler");

// sdlui_helpers
extern fn float map(float in_min, float in_max, float out_min, float out_max, float value) @cname("SDLUI_Map");
extern fn float clamp(float n, float min, float max) @cname("SDLUI_Clamp");
extern fn bool pointInRect(SdlFRect rect, float x, float y) @cname("SDLUI_PointInRect");
extern fn void init(SdlRenderer *r, SdlWindow *w) @cname("SDLUI_Init");
extern fn void mouseStateReset() @cname("SDLUI_MouseStateReset");
extern fn void mouseStateSet(int event_type, int mbutton) @cname("SDLUI_MouseStateSet");
extern fn SDLUIMousebutton mouseButton(int mbutton) @cname("SDLUI_MouseButton");
extern fn void setColor(SdlColor c) @cname("SDLUI_SetColor");
extern fn void gradientToTexture(SdlTexture *t, SdlColor c, int width, int height, int interv) @cname("SDLUI_GradientToTexture");
extern fn float max(float a, float b) @cname("SDLUI_Max");
extern fn void colorize(SdlTexture *t, SdlColor c) @cname("SDLUI_Colorize");
extern fn void setActiveWindow(SDLUI_Control_Window *wnd) @cname("SDLUI_SetActiveWindow");
extern fn SDLUIResizeDirection setWindowResizeCursor(SDLUI_Control_Window *wnd, float mousex, float mousey) @cname("SDLUI_SetWindowResizeCursor");
extern fn void resizeWindow(SDLUI_Control_Window *aw, SDLUIResizeDirection res_dir, int mx, int my) @cname("SDLUI_ResizeWindow");
extern fn void windowHandler() @cname("SDLUI_WindowHandler");

// sdlui_render
extern fn void drawText(int x, int y,  char *text, SdlTexture *dst) @cname("SDLUI_DrawText");
extern fn void drawTextRange(int x, int y,  char *text, int start, int length, SdlTexture *dst) @cname("SDLUI_DrawTextRange");
extern fn void render_Button(SDLUI_Control_Button *btn) @cname("SDLUI_Render_Button");
extern fn void render_SliderInt(SDLUI_Control_SliderInt *si) @cname("SDLUI_Render_SliderInt");
extern fn void render_CheckBox(SDLUI_Control_CheckBox *chk) @cname("SDLUI_Render_CheckBox");
extern fn void render_Text(SDLUI_Control_Text *txt) @cname("SDLUI_Render_Text");
extern fn void render_ToggleButton(SDLUI_Control_ToggleButton *tb) @cname("SDLUI_Render_ToggleButton");
extern fn void render_RadioButton(SDLUI_Control_RadioButton *rb) @cname("SDLUI_Render_RadioButton");
extern fn void render_Tabcontainer(SDLUI_Control_TabContainer *tbc) @cname("SDLUI_Render_Tabcontainer");
extern fn void render_ScrollArea(SDLUI_Control_ScrollArea *sa) @cname("SDLUI_Render_ScrollArea");
extern fn void render_TextBox(SDLUI_Control_TextBox *tbx) @cname("SDLUI_Render_TextBox");
extern fn void renderChild(SDLUIControlType type, SDLUI_Control *ctrl) @cname("SDLUI_RenderChild");
extern fn void render_Window(SDLUI_Control_Window *wnd) @cname("SDLUI_Render_Window");
extern fn void forceReDraw() @cname("SDLUI_ForceReDraw");
extern fn void render() @cname("SDLUI_Render");

// sdlui_usage

extern fn bool window(SDLUI_Control_Window *wnd) @cname("SDLUI_Window");
extern fn bool button(SDLUI_Control_Button *btn) @cname("SDLUI_Button");
extern fn bool sliderInt(SDLUI_Control_SliderInt *si) @cname("SDLUI_SliderInt");
extern fn bool checkBox(SDLUI_Control_CheckBox *chk) @cname("SDLUI_CheckBox");
extern fn bool toggleButton(SDLUI_Control_ToggleButton *tb) @cname("SDLUI_ToggleButton");
extern fn bool radioButton(SDLUI_Control_RadioButton *rb) @cname("SDLUI_RadioButton");
extern fn bool text(SDLUI_Control_Text *txt) @cname("SDLUI_Text");
extern fn bool tabContainer(SDLUI_Control_TabContainer *tbc) @cname("SDLUI_TabContainer");
extern fn bool scrollArea(SDLUI_Control_ScrollArea *sa) @cname("SDLUI_ScrollArea");
extern fn bool list(SDLUI_Control_List *lst,  char *cur_item, int num_items, int cur_index) @cname("SDLUI_List");
extern fn bool textBox(SDLUI_Control_TextBox *tbx) @cname("SDLUI_TextBox");

// wrapped member functions
extern fn void tab_container_add_tab(SDLUI_Control_TabContainer* self, char  *text) @cname("SDLUI_TabContainer_add_tab");
extern fn void tab_container_set_active_tab(SDLUI_Control_TabContainer* self, int index) @cname("SDLUI_TabContainer_set_active_tab");
extern fn void tab_container_add_child(SDLUI_Control_TabContainer* self, int tab_index, SDLUI_Control *ctrl) @cname("SDLUI_TabContainer_add_child");

extern fn void arrayOfControls_create(SDLUI_ArrayOfControls* self) @cname("SDLUI_ArrayOfControls_create");
extern fn void arrayOfControls_ensure_capacity(SDLUI_ArrayOfControls* self) @cname("SDLUI_ArrayOfControls_ensure_capacity");
extern fn void arrayOfControls_push(SDLUI_ArrayOfControls* self, SDLUI_Control *elem) @cname("SDLUI_ArrayOfControls_push");
extern fn void arrayOfControls_pop(SDLUI_ArrayOfControls* self, SDLUI_Control *elem) @cname("SDLUI_ArrayOfControls_pop");
extern fn void arrayOfControls_to_back(SDLUI_ArrayOfControls* self, SDLUI_Control *elem) @cname("SDLUI_ArrayOfControls_to_back");

extern fn void string_create(SDLUI_String* self, char  *str) @cname("SDLUI_String_create") ;
extern fn void string_destroy(SDLUI_String* self) @cname("SDLUI_String_destroy") ;
extern fn void string_modify(SDLUI_String* self,  char *str) @cname("SDLUI_String_modify") ;
extern fn bool string_insert_char(SDLUI_String* self, char c, int pos) @cname("SDLUI_String_insert_char") ;
extern fn bool string_delete_char(SDLUI_String* self, int pos) @cname("SDLUI_String_delete_char") ;
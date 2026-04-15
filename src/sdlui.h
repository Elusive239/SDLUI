#pragma once

#include <iostream>
#include <stdlib.h>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#pragma comment(lib, "Shcore.lib")
	#include <windows.h>
	#include <ShellScalingAPI.h>
	#include <comdef.h>
#endif

#ifdef __APPLE__
    #include <SDL3/SDL.h>
    #include <SDL3_image/SDL_image.h>
    #include <SDL3_ttf/SDL_ttf.h>
#else
    #include "SDL.h"
    #include "SDL_ttf.h"
    #include "SDL_image.h"
#endif

// Constants

#define SDLUI_COLLECTION_CHUNK 10
#define SDLUI_STRING_CAPACITY 20
#define SDLUI_MARGIN 8
#define SDLUI_WINDOW_MIN_SIZE 120

// Types

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

struct SDLUI_String;
struct SDLUI_Theme;
struct SDLUI_Control;
struct SDLUI_Font;
struct SDLUI_ArrayOfControls;
struct SDLUI_Control_Window;
struct SDLUI_Control_Button;
struct SDLUI_Control_SliderInt;
struct SDLUI_Control_CheckBox;
struct SDLUI_Control_ToggleButton;
struct SDLUI_Control_RadioButton;
struct SDLUI_Control_TextBox;
struct SDLUI_Core;
struct SDLUI_Control_Tab;
struct SDLUI_Control_TabContainer;
struct SDLUI_Control_Label;
struct SDLUI_Control_Text;
struct SDLUI_Control_ScrollArea;
struct SDLUI_Control_List;

#include "sdlui_enums.c"

// Function Declarations

extern "C" {
    // sdlui_create
	SDLUI_Control_Window *SDLUI_CreateWindow(i32 x, i32 y, i32 w, i32 h, char const *title);
	SDLUI_Control_Button *SDLUI_CreateButton(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text);
	SDLUI_Control_SliderInt *SDLUI_CreateSliderInt(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 min, i32 max, i32 value, SDLUI_ORIENTATION orientation );
	SDLUI_Control_CheckBox *SDLUI_CreateCheckBox(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text, bool checked);
	SDLUI_Control_Text *SDLUI_CreateText(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text);
	SDLUI_Control_ToggleButton *SDLUI_CreateToggleButton(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text, bool checked);
	SDLUI_ArrayOfControls SDLUI_CreateRadioButtonGroup();
	SDLUI_Control_RadioButton *SDLUI_CreateRadioButton(SDLUI_Control_Window *wnd, SDLUI_ArrayOfControls &group, i32 x, i32 y, char const *text, bool checked);
	SDLUI_Control_TabContainer *SDLUI_CreateTabContainer(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w, i32 h);
	SDLUI_Control_ScrollArea *SDLUI_CreateScrollArea(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w, i32 h, SDL_Texture *tex);
	SDLUI_Control_List *SDLUI_CreateList(SDLUI_Control_Window *wnd, SDLUI_Control_ScrollArea *sa, i32 num_items);
	SDLUI_Control_TextBox *SDLUI_CreateTextBox(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w);

    // sdlui_event
    void SDLUI_EventHandler(SDL_Event const &ev);

    // sdlui_helpers
	float SDLUI_Map(float in_min, float in_max, float out_min, float out_max, float value);
	float SDLUI_Clamp(float n, float min, float max);
	bool SDLUI_PointInRect(SDL_FRect rect, float x, float y);
	void SDLUI_Init(SDL_Renderer *r, SDL_Window *w);
	void SDLUI_MouseStateReset();
	void SDLUI_MouseStateSet(i32 event_type, i32 mbutton);
	SDLUI_MOUSEBUTTON SDLUI_MouseButton(i32 mbutton);
	void SDLUI_SetColor(SDL_Color c);
	void SDLUI_GradientToTexture(SDL_Texture *t, SDL_Color c, i32 width, i32 height, i32 interv);
	float SDLUI_Max(float a, float b);
	void SDLUI_Colorize(SDL_Texture *t, SDL_Color c);
	void SDLUI_SetActiveWindow(SDLUI_Control_Window *wnd);
	SDLUI_RESIZE_DIRECTION SDLUI_SetWindowResizeCursor(SDLUI_Control_Window *wnd, float mousex, float mousey);
	void SDLUI_ResizeWindow(SDLUI_Control_Window *aw, SDLUI_RESIZE_DIRECTION res_dir, i32 mx, i32 my);
	void SDLUI_WindowHandler();
	void save_texture(SDL_Renderer *ren, SDL_Texture *tex, const char *filename);

    // sdlui_render
    void SDLUI_DrawText(i32 x, i32 y, const char *text, SDL_Texture *dst);
    void SDLUI_DrawTextRange(i32 x, i32 y, const char *text, i32 start, i32 length, SDL_Texture *dst);
    void SDLUI_Render_Button(SDLUI_Control_Button *btn);
    void SDLUI_Render_SliderInt(SDLUI_Control_SliderInt *si);
    void SDLUI_Render_CheckBox(SDLUI_Control_CheckBox *chk);
    void SDLUI_Render_Text(SDLUI_Control_Text *txt);
    void SDLUI_Render_ToggleButton(SDLUI_Control_ToggleButton *tb);
    void SDLUI_Render_RadioButton(SDLUI_Control_RadioButton *rb);
    void SDLUI_Render_Tabcontainer(SDLUI_Control_TabContainer *tbc);
    void SDLUI_Render_ScrollArea(SDLUI_Control_ScrollArea *sa);
    void SDLUI_Render_TextBox(SDLUI_Control_TextBox *tbx);
    void SDLUI_RenderChild(SDLUI_CONTROL_TYPE type, SDLUI_Control *ctrl);
    void SDLUI_Render_Window(SDLUI_Control_Window *wnd);
    void SDLUI_ForceReDraw();
    void SDLUI_Render();

    // sdlui_usage

	bool SDLUI_Window(SDLUI_Control_Window *wnd);
	bool SDLUI_Button(SDLUI_Control_Button *btn);
	bool SDLUI_SliderInt(SDLUI_Control_SliderInt *si);
	bool SDLUI_CheckBox(SDLUI_Control_CheckBox *chk);
	bool SDLUI_ToggleButton(SDLUI_Control_ToggleButton *tb);
	bool SDLUI_RadioButton(SDLUI_Control_RadioButton *rb);
	bool SDLUI_Text(SDLUI_Control_Text *txt);
	bool SDLUI_TabContainer(SDLUI_Control_TabContainer *tbc);
	bool SDLUI_ScrollArea(SDLUI_Control_ScrollArea *sa);
	bool SDLUI_List(SDLUI_Control_List *lst, const char *cur_item, i32 num_items, i32 cur_index);
	bool SDLUI_TextBox(SDLUI_Control_TextBox *tbx);

	// wrapped member functions
	void SDLUI_TabContainer_add_tab(SDLUI_Control_TabContainer* self, char const *text);
	void SDLUI_TabContainer_set_active_tab(SDLUI_Control_TabContainer* self, i32 index);
	void SDLUI_TabContainer_add_child(SDLUI_Control_TabContainer* self, i32 tab_index, SDLUI_Control *ctrl);

	void SDLUI_ArrayOfControls_create(SDLUI_ArrayOfControls* self);
	void SDLUI_ArrayOfControls_ensure_capacity(SDLUI_ArrayOfControls* self);
	void SDLUI_ArrayOfControls_push(SDLUI_ArrayOfControls* self, SDLUI_Control *elem);
	void SDLUI_ArrayOfControls_pop(SDLUI_ArrayOfControls* self, SDLUI_Control *elem);
	void SDLUI_ArrayOfControls_to_back(SDLUI_ArrayOfControls* self, SDLUI_Control *elem);

	void SDLUI_String_create(SDLUI_String* self, char const *str);
	void SDLUI_String_destroy(SDLUI_String* self);
	void SDLUI_String_modify(SDLUI_String* self, const char *str);
	bool SDLUI_String_insert_char(SDLUI_String* self, char c, i32 pos);
	bool SDLUI_String_delete_char(SDLUI_String* self, i32 pos);
}

#ifdef __cplusplus
#define CPP
#endif

#define CTRL(value) ((SDLUI_Control*)(void*)(value))

#include "sdlui_structs.c"
#include "sdlui_members.c"
#include "sdlui_embeded.c"
#include "sdlui_helpers.c"
#include "sdlui_render.c"
#include "sdlui_create.c"
#include "sdlui_event.c"
#include "sdlui_usage.c"


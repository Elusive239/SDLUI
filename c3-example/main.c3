// --- SDL skeleton application demonstrating how to use SDLUI. ---

// There are 4 functions that handle initialization, input, window management and rendering:
// sdlui::init
// sdlui::eventHandler
// sdlui::windowHandler
// sdlui::render

// Control creation and usage is performed with functions of the following convention:
// Control creation: 	sdlui::create<control name>
// Control usage: 		sdlui::<control name>

// The rest of the code is just what one would find in a typical SDL application.
// NOTE: The in-place includes were done for clarity.

module main;
import std;
import sdlui;
import sdl3;
import sdl3_image;
import sdl3_ttf;

fn int main()
{
	sdl3::init({ .video = true });
	SdlWindow* w = sdl3::createWindow("SDLUI app", 1200, 800, { }); // windowFlags
	SdlRenderer* r = sdl3::createRenderer(w, null);

	// Initializes SDLUI. Called after a window and renderer have been created.
	sdlui::init(r, w);

	// Each sdlui::create_xxx function returns a pointer to a control. It is used to access its properties,
	// and is later passed on to its usage function inside tha main application loop.
	// sldui_demo_controls_creation

	// Create some windows.
	SDLUI_Control_Window* wnd1 = sdlui::createWindow(20, 100, 350, 400, "Window1");
	SDLUI_Control_Window* wnd2 = sdlui::createWindow(460, 30, 450, 350, "Window2");
	SDLUI_Control_Window* wnd3 = sdlui::createWindow(410, 400, 370, 200, "Window3");
	SDLUI_Control_Window* wnd4 = sdlui::createWindow(120, 620, 820, 170, "Colors");

	// Add some common controls. Simple controls such as these do not need much explaining.
	SDLUI_Control_Button* btn1 = sdlui::createButton(wnd1, 10, 40, "PushButton");
	SDLUI_Control_TextBox* tbx1 = sdlui::createTextBox(wnd1, 160, 40, 150);
	SDLUI_Control_SliderInt* si1 = sdlui::createSliderInt(wnd1, 10, 90, 0, 100, 20, SDLUI_ORIENTATION_HORIZONTAL);
	SDLUI_Control_SliderInt* si2 = sdlui::createSliderInt(wnd1, 140, 90, 0, 100, 60, SDLUI_ORIENTATION_VERTICAL);
	SDLUI_Control_CheckBox* chk1 = sdlui::createCheckBox(wnd1, 10, 130, "Checkbox", false);

	// A ScrollArea is a dual purpose control. It can either host an image (in the form of an SdlTexture),
	// or a list, where it effectively replicates the functionality of a listbox.
	// In both cases it draws and handles scrollbars when the content is bigger than the ScrollArea's size.
	// In the example bellow we create a ScrollArea that will be used as a listbox.
	// We pass NULL as the last parameter since we don't have an image texture.
	SDLUI_Control_ScrollArea* sa1 = sdlui::createScrollArea(wnd1, 10, 220, 320, 160, null);

	// A list needs an array of strings. It doesn't matter what kind of container it is (std::vector, C array of strings, etc),
	// as long as it can feed the list with a char* representing the caption for each element. This will make more sense
	// when we'll later call the control's usage function inside the application loop.
	List{String} list_items;
	list_items.init(mem);
	for (int i = 0; i < 100; ++i)
	{
		const String BASE = "List Item: %d\0";
		// cur_item += std::to_string(i);
		list_items.push(string::format(mem, BASE, i));
	}

	// We can now create a list and bind it to the ScrollArea above.
	// If the size of the data container isn't known at compile time we can simply pass zero to the last function parameter.
	// In this case we pass the size of the list_items vector.
	SDLUI_Control_List* lst1 = sdlui::createList(wnd1, sa1, (int)list_items.len());


	// Some more controls...
	SDLUI_Control_Button* btn2 = sdlui::createButton(wnd2, 10, 40, "ClickMe");
	SDLUI_Control_CheckBox* chk2 = sdlui::createCheckBox(wnd2, 10, 90, "Another checkbox", true);

	// Here's another ScrollArea. This time it hosts an image. If the image texture isn't availlable at compile time,
	// we can pass NULL as the last parameter. Here we first create a texture and pass it on to the ScrollArea.
	SdlSurface* surf = sdl3_image::load("test_image.png");
	SdlTexture* tex = sdl3::createTextureFromSurface(r, surf);
	sdl3::destroySurface(surf);
	SDLUI_Control_ScrollArea* sa2 = sdlui::createScrollArea(wnd2, 10, 120, 430, 220, tex);


	// RadioButtons are handled in groups. A special kind of array(SDLUI_ArrayOfControls) is used
	// to store pointers to RadioButtons which belong in the same group. The sdlui::createRadioButtonGroup() function
	// is just a helper to create and initialize such an array.
	SDLUI_ArrayOfControls rb_group1 = sdlui::createRadioButtonGroup();
	SDLUI_ArrayOfControls rb_group2 = sdlui::createRadioButtonGroup();

	// Create a bunch of RadioButtons, assigning them to their respective group.
	SDLUI_Control_RadioButton* rb1 = sdlui::createRadioButton(wnd3, &rb_group1, 30, 80, "RadioButton1", true);
	SDLUI_Control_RadioButton* rb2 = sdlui::createRadioButton(wnd3, &rb_group1, 30, 110, "RadioButton2", false);
	SDLUI_Control_RadioButton* rb3 = sdlui::createRadioButton(wnd3, &rb_group1, 30, 140, "RadioButton3", false);

	SDLUI_Control_RadioButton* rb4 = sdlui::createRadioButton(wnd3, &rb_group2, 220, 80, "RadioButton4", false);
	SDLUI_Control_RadioButton* rb5 = sdlui::createRadioButton(wnd3, &rb_group2, 220, 110, "RadioButton5", true);
	SDLUI_Control_RadioButton* rb6 = sdlui::createRadioButton(wnd3, &rb_group2, 220, 140, "RadioButton6", false);

	// More controls...
	SDLUI_Control_ToggleButton* tb1 = sdlui::createToggleButton(wnd3, 30, 80, "Toggle Button", true);
	SDLUI_Control_Button* btn3 = sdlui::createButton(wnd3, 30, 110, "Test");

	// A TabContainer is a container for child controls, and all it does is manage their visibility.
	// It can be created with either a horizontal (default) or vertical strip of tabs.
	// It's a 3-step proccess: Create a TabContainer, add some tabs, and finally add previously created controls to each tab.
	// Note that positioning of controls is relative to the window and not to the position of the TabContainer inside the window.
	SDLUI_Control_TabContainer* tbc1 = sdlui::createTabContainer(wnd3, 10, 40, 350, 140);

	tbc1.add_tab("First");
	tbc1.add_tab("Second");
	tbc1.add_tab("Third");

	tbc1.set_active_tab(0);

	tbc1.add_child(0, rb1);
	tbc1.add_child(0, rb2);
	tbc1.add_child(0, rb3);
	tbc1.add_child(0, rb4);
	tbc1.add_child(0, rb5);
	tbc1.add_child(0, rb6);

	tbc1.add_child(1, tb1);
	tbc1.add_child(1, btn3);

	// Yet more controls...
	SDLUI_Control_Button* btn_copy = sdlui::createButton(wnd4, 680, 40, "Copy");
	SDLUI_Control_Text* txt01 = sdlui::createText(wnd4, 10, 40, "Active window bar");
	SDLUI_Control_Text* txt02 = sdlui::createText(wnd4, 10, 70, "Inactive window bar");
	SDLUI_Control_Text* txt03 = sdlui::createText(wnd4, 10, 100, "Window bg");
	SDLUI_Control_Text* txt04 = sdlui::createText(wnd4, 10, 130, "Highlight");

	// Here's an example creating controls in batch and storing their pointers in an SDLUI_ArrayOfControls.
	SDLUI_ArrayOfControls color_sliders;
	color_sliders.create();

	int x = 200;
	int y = 40;
	int num_sliders = 12;

	for (int i = 0; i < num_sliders; ++i)
	{
		SDLUI_Control_SliderInt* si = sdlui::createSliderInt(wnd4, x, y, 0, 255, 0, SDLUI_ORIENTATION_HORIZONTAL);
		si.base.w = 128;
		color_sliders.push(si);
		x += 150;

		if ((i + 1) % 3 == 0 && i > 0)
		{
			x = 200;
			y += 30;
		}
	}

	// We modify the 'value' field of each slider after creation. Most control fields are safe to modify directly,
	// but not all of them (currently undocumented).
	SDLUI_Control_SliderInt* col_slider;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[0];
	col_slider.value = sdlui::core.theme.col_active_window_bar.r;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[1];
	col_slider.value = sdlui::core.theme.col_active_window_bar.g;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[2];
	col_slider.value = sdlui::core.theme.col_active_window_bar.b;

	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[3];
	col_slider.value = sdlui::core.theme.col_inactive_window_bar.r;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[4];
	col_slider.value = sdlui::core.theme.col_inactive_window_bar.g;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[5];
	col_slider.value = sdlui::core.theme.col_inactive_window_bar.b;

	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[6];
	col_slider.value = sdlui::core.theme.col_window_bg.r;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[7];
	col_slider.value = sdlui::core.theme.col_window_bg.g;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[8];
	col_slider.value = sdlui::core.theme.col_window_bg.b;

	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[9];
	col_slider.value = sdlui::core.theme.col_highlight.r;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[10];
	col_slider.value = sdlui::core.theme.col_highlight.g;
	col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[11];
	col_slider.value = sdlui::core.theme.col_highlight.b;


	SDLUI_Control_Window* calc = sdlui::createWindow(980, 280, 170, 280, "Calc");
	calc.can_be_resized = false;
	SDLUI_Control_Text* display = sdlui::createText(calc, 150, 50, "0");

	SDLUI_Control_Button* calc_btnc = sdlui::createButton(calc, 130, 80, "C");
	calc_btnc.base.w = 30;

	SDLUI_Control_Button* calc_btn7 = sdlui::createButton(calc, 10, 120, "7");
	calc_btn7.base.w = 30;
	SDLUI_Control_Button* calc_btn8 = sdlui::createButton(calc, 50, 120, "8");
	calc_btn8.base.w = 30;
	SDLUI_Control_Button* calc_btn9 = sdlui::createButton(calc, 90, 120, "9");
	calc_btn9.base.w = 30;
	SDLUI_Control_Button* calc_btndiv = sdlui::createButton(calc, 130, 120, "/");
	calc_btndiv.base.w = 30;

	SDLUI_Control_Button* calc_btn4 = sdlui::createButton(calc, 10, 160, "4");
	calc_btn4.base.w = 30;
	SDLUI_Control_Button* calc_btn5 = sdlui::createButton(calc, 50, 160, "5");
	calc_btn5.base.w = 30;
	SDLUI_Control_Button* calc_btn6 = sdlui::createButton(calc, 90, 160, "6");
	calc_btn6.base.w = 30;
	SDLUI_Control_Button* calc_btnmul = sdlui::createButton(calc, 130, 160, "x");
	calc_btnmul.base.w = 30;

	SDLUI_Control_Button* calc_btn1 = sdlui::createButton(calc, 10, 200, "1");
	calc_btn1.base.w = 30;
	SDLUI_Control_Button* calc_btn2 = sdlui::createButton(calc, 50, 200, "2");
	calc_btn2.base.w = 30;
	SDLUI_Control_Button* calc_btn3 = sdlui::createButton(calc, 90, 200, "3");
	calc_btn3.base.w = 30;
	SDLUI_Control_Button* calc_btnsub = sdlui::createButton(calc, 130, 200, "-");
	calc_btnsub.base.w = 30;

	SDLUI_Control_Button* calc_btn0 = sdlui::createButton(calc, 10, 240, "0");
	calc_btn0.base.w = 30;
	SDLUI_Control_Button* calc_btndot = sdlui::createButton(calc, 50, 240, ".");
	calc_btndot.base.w = 30;
	SDLUI_Control_Button* calc_btneq = sdlui::createButton(calc, 90, 240, "=");
	calc_btneq.base.w = 30;
	SDLUI_Control_Button* calc_btnadd = sdlui::createButton(calc, 130, 240, "+");
	calc_btnadd.base.w = 30;


	bool quit = false;
	SdlEvent e;
	while (!quit)
	{
		if (sdl3::pollEvent(&e))
		{
			if (e.type == QUIT)
			{
				quit = true;
			}
			if (e.type == WINDOW_MINIMIZED || e.type == WINDOW_FOCUS_LOST)
			{
				while (sdl3::pollEvent(&e))
				{
					if (e.type == WINDOW_RESTORED || e.type == WINDOW_FOCUS_GAINED)
					{
						sdlui::forceReDraw();
						break;
					}
				}
			}

			// Captures mouse input and window events.
			sdlui::eventHandler(&e);
		}

		// Manages SDLUI windows and their child controls
		sdlui::windowHandler();

		sdl3::setRenderDrawColor(r, 20, 20, 20, 255);
		sdl3::renderClear(r);

		// sdlui_demo_controls_usage
		// We call each controls' usage function. Most of them return a boolean representing user interaction.
		// (a click of a button, a slider's change in value, etc.)

		sdlui::window(wnd1);
		sdlui::window(wnd2);
		sdlui::window(wnd3);
		sdlui::window(wnd4);

		if(sdlui::button(btn1)) { io::printfn("%s", "btn1" );}
		if(sdlui::textBox(tbx1)) { io::printfn("%s", "tbx1" );}
		if(sdlui::sliderInt(si1)) { io::printfn("%s", si1.value );}
		if(sdlui::sliderInt(si2)) { io::printfn("%s", si2.value );}

		sdlui::scrollArea(sa1);

		// This is how we "feed" the list with elements. It's simply a loop where we pass the element caption,
		// the total count of elements, and current iteration index.
		for (int i = 0; i < list_items.len(); ++i)
		{
			if(sdlui::list(lst1, &(list_items[i])[0], (int) list_items.len(), i))
			{
				 io::printfn("%s", lst1.selected_index );
			}
		}

		if(sdlui::checkBox(chk1)) { io::printfn("%s", "chk1" );}

		if(sdlui::button(btn2)) { io::printfn("%s", "btn2" );}
		if(sdlui::checkBox(chk2)) { io::printfn("%s", "chk2" );}
		if(sdlui::scrollArea(sa2)) {;}

		if(sdlui::radioButton(rb1)) { io::printfn("%s", "rb1" );}
		if(sdlui::radioButton(rb2)) { io::printfn("%s", "rb2" );}
		if(sdlui::radioButton(rb3)) { io::printfn("%s", "rb3" );}
		if(sdlui::radioButton(rb4)) { io::printfn("%s", "rb4" );}
		if(sdlui::radioButton(rb5)) { io::printfn("%s", "rb5" );}
		if(sdlui::radioButton(rb6)) { io::printfn("%s", "rb6" );}

		if(sdlui::toggleButton(tb1)) { io::printfn("%s", "tb1" );}
		if(sdlui::button(btn3)) { io::printfn("%s", "btn3" );}
		sdlui::tabContainer(tbc1);

		// Copy the current theme in clipboard. This can be used to create an instance of an SDLUI_Theme struct
		// for a custom color theme definition.
		if(sdlui::button(btn_copy))
		{
			SDLUI_Control_SliderInt *cur;
			DString cb;
			cb.init(mem);
			defer cb.free();
			cb.append("SDLUI_Theme my_theme = {\n\t{");

			for (int i = 0; i < num_sliders; ++i)
			{
				cur = (SDLUI_Control_SliderInt*)color_sliders.data[i];

				// cb += std::to_string(cur.value);
				cb.appendf("%d", cur.value);

				if((i + 1) % 3 == 0)
				{
					if(i < num_sliders - 1)
					{
						cb .append( ", 255},\n\t{");
					}
					else
					{
						cb .append (", 255}\n};");
					}
				}
				else
				{
					cb .append (", ");
				}
			}
			cb.append_char('\0');
			sdl3::setClipboardText(&cb.str_view()[0]);
		}

		// SDLUI_Control_SliderInt *col_slider;

		sdlui::text(txt01);
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[0];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_active_window_bar.r = (char) col_slider.value; sdlui::forceReDraw();}
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[1];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_active_window_bar.g = (char) col_slider.value; sdlui::forceReDraw();}
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[2];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_active_window_bar.b = (char) col_slider.value; sdlui::forceReDraw();}

		sdlui::text(txt02);
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[3];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_inactive_window_bar.r = (char) col_slider.value; sdlui::forceReDraw();}
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[4];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_inactive_window_bar.g = (char) col_slider.value; sdlui::forceReDraw();}
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[5];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_inactive_window_bar.b = (char) col_slider.value; sdlui::forceReDraw();}

		sdlui::text(txt03);
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[6];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_window_bg.r = (char) col_slider.value; sdlui::forceReDraw();}
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[7];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_window_bg.g = (char) col_slider.value; sdlui::forceReDraw();}
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[8];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_window_bg.b = (char) col_slider.value; sdlui::forceReDraw();}

		sdlui::text(txt04);
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[9];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_highlight.r = (char) col_slider.value; sdlui::forceReDraw();}
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[10];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_highlight.g = (char) col_slider.value; sdlui::forceReDraw();}
		col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[11];
		if(sdlui::sliderInt(col_slider)) {sdlui::core.theme.col_highlight.b = (char) col_slider.value; sdlui::forceReDraw();}


		// TODO: Make a tiny calc app

		sdlui::window(calc);

		sdlui::text(display);

		if(sdlui::button(calc_btnc)) {;}
		if(sdlui::button(calc_btn7)) {;}
		if(sdlui::button(calc_btn8)) {;}
		if(sdlui::button(calc_btn9)) {;}
		if(sdlui::button(calc_btndiv)) {;}

		if(sdlui::button(calc_btn4)) {;}
		if(sdlui::button(calc_btn5)) {;}
		if(sdlui::button(calc_btn6)) {;}
		if(sdlui::button(calc_btnmul)) {;}

		if(sdlui::button(calc_btn1)) {;}
		if(sdlui::button(calc_btn2)) {;}
		if(sdlui::button(calc_btn3)) {;}
		if(sdlui::button(calc_btnsub)) {;}

		if(sdlui::button(calc_btn0)) {;}
		if(sdlui::button(calc_btndot)) {;}
		if(sdlui::button(calc_btneq)) {;}
		if(sdlui::button(calc_btnadd)) {;}

		// Draws the UI. Regular SDL drawing should take place before this, for the UI to appear on top.
		sdlui::render();
		sdl3::renderPresent(r);
	}
	sdl3::destroyRenderer(r);
	sdl3::destroyWindow(w);
	sdl3::quit();
	return 0;
}

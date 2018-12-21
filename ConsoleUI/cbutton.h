#pragma once
#include "cwbase.h"

enum T_button_style
{
	T_circle_button,
	T_rect_button
};

class cbutton :
	public cwbase
{
private:
	T_button_style button_type= T_rect_button;
public:
	cbutton();
	~cbutton();

	bool update(bool redraw = false);
	void click_in(c_point p);
	void double_click(c_point p);
	void set_type(T_button_style bt) { button_type = bt; };
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
};


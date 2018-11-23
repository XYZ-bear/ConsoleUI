#pragma once
#include "cwbase.h"

enum T_button
{
	T_circle_button,
	T_rect_button
};
class cbutton :
	public cwbase
{
private:
	T_button button_type= T_rect_button;
public:
	cbutton();
	~cbutton();
	bool update();
	bool init();
	void click_in(c_point p);
	void double_click(c_point p);
	void set_type(T_button bt) { button_type = bt; };
};


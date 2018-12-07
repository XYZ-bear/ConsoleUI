#include "stdafx.h"
#include "cbutton.h"


cbutton::cbutton()
{
	set_ctr_type(T_button);
}


cbutton::~cbutton()
{
}

bool cbutton::update()
{
	//_gdi.fill_rect({ 0,0 }, { _width ,_height }, 1, RGB(0, 255, 0));
	if (button_type == T_circle_button)
		_gdi.draw_ellipse({ 0,0 }, _width, _active_color);
	else
		_gdi.fill_rect({ 0,0 }, { _width ,_height }, 1, RGB(0, 255, 0));
	return true;
}

void cbutton::click_in(c_point p) {
}

bool cbutton::init() {
	return true;
}

void cbutton::double_click(c_point p) {

}

void cbutton::mouse_move_in(c_point p) {
	_active_color = _mouse_in_color;
}

void cbutton::mouse_move_out(c_point p) {
	_active_color = _color;
}
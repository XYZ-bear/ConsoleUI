#include "stdafx.h"
#include "cbutton.h"


cbutton::cbutton()
{
}


cbutton::~cbutton()
{
}

bool cbutton::update()
{
	//_gdi.fill_rect({ 0,0 }, { _width ,_height }, 1, RGB(0, 255, 0));
	if (button_type == T_circle_button)
		_gdi.draw_ellipse({ 0,0 }, _width, _bk_color);
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
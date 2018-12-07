#include "stdafx.h"
#include "cwbase.h"


cwbase::cwbase()
{
	m_console_hand_= CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}

cwbase::~cwbase()
{
}

int cwbase::get_width() { return _width; }

int cwbase::get_height() { return _height; }

int cwbase::get_console_width() {
	RECT r;
	GetClientRect(GetConsoleWindow(),&r);
	return r.right;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(m_console_hand_, &bInfo);
	return bInfo.srWindow.Right - bInfo.srWindow.Left;
}

int cwbase::get_console_height() {
	RECT r;
	GetClientRect(GetConsoleWindow(), &r);
	return r.bottom;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(m_console_hand_, &bInfo);
	return bInfo.srWindow.Bottom - bInfo.srWindow.Top;
}

cgdi& cwbase::get_gdi() { return _gdi; }

bool cwbase::create(c_point op, int width, int height, COLORREF bk_color) {
	_left_top = op;
	_right_bottom = { op.x + width,op.y + height };
	_width = width;
	_height = height;

	_active_color = bk_color;
	_color = bk_color;
	_mouse_in_color = bk_color;

	_align = T_v_align_left;
	_gdi.set_rng(width, height);
	_gdi.set_refer_point(_left_top);
	_gdi.init();
	return true;
}

HDC& cwbase::get_hdc() {
	return buffer_hdc_;
}

bool cwbase::is_point_in(c_point p) {
	if (p > _left_top&&p < _right_bottom)
		return true;
	return false;
}

HWND cwbase::get_hand() {
	return GetConsoleWindow();
}

void cwbase::set_point(c_point point){
	_left_top = point;
	_right_bottom = { point.x + _width,point.y + _height };
	_gdi.set_refer_point(_left_top);
}

void cwbase::set_size(c_point op, int width, int height) {
	_left_top = op;
	_right_bottom = { op.x + width,op.y + height };
	_width = width;
	_height = height;
	_gdi.set_rng(width, height);
	_gdi.set_refer_point(_left_top);
	update();
}

c_point cwbase::get_point() {
	return _left_top;
}

c_point &cwbase::get_left_top() {
	return _left_top;
}
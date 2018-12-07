#pragma once

#include "cgdi.h"
#include <map>


class cwbase
{
private:
	HANDLE m_console_hand_;
	HDC buffer_hdc_;
	T_ctr_type ctr_type_;

protected:
	c_point _left_top;
	c_point _right_bottom;
	int _width =0;
	int _height =0;
	cgdi _gdi;
	T_align _align;
	/*color*/
	COLORREF _active_color;
	COLORREF _color;
	COLORREF _mouse_in_color;
public:
	cwbase();
	virtual ~cwbase();
	int get_width();
	int get_height();
	c_point &get_left_top();
	int get_console_width();
	int get_console_height();
	bool is_point_in(c_point p);
	cgdi& get_gdi();
	HDC& get_hdc();
	HWND get_hand();
	c_point get_point();
	void set_point(c_point point);
	void set_size(c_point op, int width, int height);
	void set_align(T_align align) { _align = align; }
	T_align get_align() { return _align; }

	void set_ctr_type(T_ctr_type ctr_type) { ctr_type_ = ctr_type; }
	T_ctr_type get_ctr_type() { return ctr_type_ ; }

	void set_color(COLORREF color) { _active_color = color; _color = color; };
	void set_mouse_in_color(COLORREF color) { _mouse_in_color = color; };
public:
	virtual bool init() = 0;
	virtual bool update() = 0;
	virtual bool create(c_point op, int width, int height, COLORREF _bk_color = RGB(255, 255, 255));
	virtual void click_in(c_point p) {};
	virtual void click_out(c_point p) {};
	virtual void double_click(c_point p) {};
	virtual void mouse_move(c_point p){};
	virtual void mouse_move_in(c_point p) {};
	virtual void mouse_move_out(c_point p) {};
};


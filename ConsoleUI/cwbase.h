#pragma once

#include "cgdi.h"
#include <map>


class cwbase
{
private:
	HANDLE m_console_hand_;
	HDC buffer_hdc_;
	
protected:
	c_point _left_top;
	c_point _right_bottom;
	int _width =0;
	int _height =0;
	cgdi _gdi;
	COLORREF _bk_color;
	T_align _align;
public:
	cwbase();
	virtual ~cwbase();
	int get_width();
	int get_height();
	c_point &get_left_top();
	bool create(c_point op, int width, int height, COLORREF _bk_color=RGB(255,255,255));
	int get_console_width();
	int get_console_height();
	bool is_point_in(c_point p);
	cgdi& get_gdi();
	HDC& get_hdc();
	HWND get_hand();
	c_point get_point();
	void set_point(c_point point);
	void set_size(c_point op, int width, int height);
	void set_align(T_align align);
public:
	virtual bool init() = 0;
	virtual bool update() = 0;
	virtual void click_in(c_point p) {};
	virtual void click_out(c_point p) {};
	virtual void double_click(c_point p) {};
	virtual void mouse_move(c_point p){};
};


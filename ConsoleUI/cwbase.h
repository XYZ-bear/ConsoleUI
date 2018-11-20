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
	map<int, void*> event_func;
public:
	cwbase();
	virtual ~cwbase();
	int get_width();
	int get_height();
	bool create(c_point op, int width, int height);
	int get_console_width();
	int get_console_height();
	bool is_point_in(c_point p);
	cgdi& get_cgdi();
	HDC& get_hdc();
	HWND get_hand();
	c_point get_point();
	void set_point(c_point point);
public:
	virtual bool update() = 0;
	virtual void click() = 0;
};


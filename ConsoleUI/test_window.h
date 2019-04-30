#pragma once
#include "cwindow.h"
#include "cmessage_box.h"

class testwindow :public cwindow
{
public:
	testwindow();
	~testwindow();
public:
	bool init();
	bool update(bool redraw);
	void click_in(c_point p);
	void click_out(c_point p);
	void mouse_move(c_point p);
	void double_click(c_point p);
	void draw();
	int dl = 0;
	cmessage_box *mb = nullptr;
public:
	void close_click( const void* p);
	void message_click(const void* p);
};


#pragma once
#include "cwindow.h"

class cmessage_box :
	public cwindow
{
public:
	cmessage_box();
	~cmessage_box();

	bool update(bool redraw = true);
	bool init();
//	void click_in(c_point p);
//	void double_click(c_point p);
//	void drag(drag_info p);
	void do_mode();
//public:
//	void mouse_move_in(c_point p);
//	void mouse_move_out(c_point p);
};


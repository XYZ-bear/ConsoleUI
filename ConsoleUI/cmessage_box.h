#pragma once
#include "cwbase.h"

class cmessage_box :
	public cwbase
{
public:
	cmessage_box();
	~cmessage_box();

	bool update(bool redraw = false);
	void click_in(c_point p);
	void double_click(c_point p);
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
};


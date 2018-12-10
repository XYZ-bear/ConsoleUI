#pragma once
#include "cwbase.h"

class cedit :
	public cwbase
{
public:
	cedit();
	~cedit();

	bool update();
	bool init();
	void click_in(c_point p);
	void click_out(c_point p);
	void double_click(c_point p);
	void test();
private:
	bool is_spin;
	bool is_focus;
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
};


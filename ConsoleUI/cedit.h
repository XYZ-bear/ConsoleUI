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
	void input_key(char key);
	void test();
private:
	bool is_spin;
	int spin_x_;
	int spin_y_;
	string text_;
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
};


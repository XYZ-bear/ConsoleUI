#pragma once
#include "cwbase.h"

class ctips :
	public cwbase
{
public:
	ctips();
	~ctips();

	bool update();
	bool init();
	void click_in(c_point p);
	void click_out(c_point p);
	void double_click(c_point p);

	void add_tip(cwbase *bind_ob,string str);
	void show_tip(cwbase *bind_ob,c_point p);
private:
	map<cwbase*,string> text_;
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
};


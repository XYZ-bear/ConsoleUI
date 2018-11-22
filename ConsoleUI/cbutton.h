#pragma once
#include "cwbase.h"
class cbutton :
	public cwbase
{
public:
	cbutton();
	~cbutton();
	bool update();
	bool init();
	void click_in(c_point p);
	void double_click(c_point p);
};


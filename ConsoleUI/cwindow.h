#pragma once

#include "cwbase.h"
#include <list>

class cwindow :public cwbase
{
public:
	cwindow();
	~cwindow();
    bool update();
	void click();
	bool add_child(cwbase *child);
	bool create(string title,c_point op, int width, int height);
	string get_title() { return title_; }
	void set_title(string title) { title_ = title; }
private:
	list<cwbase *> chidren_list;
	string title_;
};


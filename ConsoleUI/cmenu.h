#pragma once
#include "cwbase.h"
#include <iostream>
using namespace std;

class cmenu :
	public cwbase
{
	struct item
	{
		map<int, item> items;
		string text = "";
		void* extern_data;
		item& operator [](const int index) {
			return items[index];
		}
	};
private:
	item root;
	item *active_item=nullptr;
	cgdi cgg;
	cwbase *parent_window_;
	int item_height_ = 16;
	int comb_height_ = 30;
	int comb_width_ = 70;
public:
	cmenu(cwbase *parent_window);
	~cmenu();
	void update_item(item *roo,int height);
	void check_active_item(item *roo, c_point p,int height);
	bool update(bool redraw = false);
	void click_in(c_point p);
	void double_click(c_point p);
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);

public:
	item & get_root() { return root; }
};

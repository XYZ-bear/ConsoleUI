#include "stdafx.h"
#include "cmenu.h"


cmenu::cmenu(cwbase *parent_window)
{
	this->parent_window_ = parent_window;
	set_ctr_type(T_button);
}


cmenu::~cmenu()
{
}

bool cmenu::update(bool redraw)
{
	_gdi.fill_rect({ 0,0 }, { _width,_height }, _bk_color);

	_gdi.draw_text(root.text, { 0,0 });

	
	if (active_item) {
		update_item(active_item, item_height_);
	}
	return cwbase::update(redraw);
}

void cmenu::click_in(c_point p) {

	active_item = &root;
	//for (auto &item : root.items) {
	//	_gdi.draw_text(item.second.text, { 10,10 });
	//}
}

void cmenu::double_click(c_point p) {

}

void cmenu::mouse_move_in(c_point p) {
	_active_color = _mouse_in_color;
}

void cmenu::mouse_move_out(c_point p) {
	_active_color = _bk_color;
}

void cmenu::update_item(item *roo,int height){
	//cout << roo.text << endl;
	for (auto &item : roo->items) {
		_gdi.draw_text(item.second.text, { 0,height });
		height += item_height_;
		update_item(&item.second, height);
	}
}

void cmenu::check_active_item(item *roo, c_point p, int height) {
	for (auto &item : roo->items) {
		_gdi.draw_text(item.second.text, { 0,height });
		height += item_height_;
		check_active_item(&item.second,p, height);
	}
}
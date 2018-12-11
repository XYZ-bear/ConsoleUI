#include "stdafx.h"
#include "ccombox.h"


ccombox::ccombox(cwbase *parent_window)
{
	this->parent_window_ = parent_window;
	set_ctr_type(T_button);
}


ccombox::~ccombox()
{
}

bool ccombox::update()
{
	_gdi.fill_rect({ 0,0 }, { comb_width_,comb_height_ }, 1, _color);
	_gdi.draw_text(root.text, { 0,0 });

	if (active_item) {
		for (auto &item : active_item->items) {
			_gdi.draw_text(item.second.text, { 10,10 });
		}
	}
	return cwbase::update();
}

void ccombox::click_in(c_point p) {
	active_item = &root;
	//for (auto &item : root.items) {
	//	_gdi.draw_text(item.second.text, { 10,10 });
	//}
}

bool ccombox::init() {
	return true;
}

void ccombox::double_click(c_point p) {

}

void ccombox::mouse_move_in(c_point p) {
	_active_color = _mouse_in_color;
}

void ccombox::mouse_move_out(c_point p) {
	_active_color = _color;
}

void ccombox::update_item(item roo){
	cout << roo.text << endl;
	for (auto &item : roo.items) {
		update_item(item.second);
	}
}
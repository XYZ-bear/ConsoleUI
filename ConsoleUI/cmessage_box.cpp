#include "stdafx.h"
#include "cmessage_box.h"


cmessage_box::cmessage_box()
{
	set_ctr_type(T_message_box);
}


cmessage_box::~cmessage_box()
{
}

bool cmessage_box::update(bool redraw)
{
	return cwbase::update(redraw);
}

void cmessage_box::click_in(c_point p) {
}

void cmessage_box::double_click(c_point p) {

}

void cmessage_box::mouse_move_in(c_point p) {
	_active_color = _mouse_in_color;
	update();
	cwbase::mouse_move_out(p);
}

void cmessage_box::mouse_move_out(c_point p) {
	_active_color = _bk_color;
	update();
	cwbase::mouse_move_out(p);
}
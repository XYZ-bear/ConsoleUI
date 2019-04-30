#include "stdafx.h"
#include "cmessage_box.h"

cmessage_box::cmessage_box()
{
	set_ctr_mode(T_ctr_run_mode::T_single_mode);
	set_is_show(false);
	set_ctr_type(T_ctr_type::T_message_box);
}

cmessage_box::~cmessage_box()
{
}

bool cmessage_box::update(bool redraw)
{
	//_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	return cwindow::update(redraw);
}

bool cmessage_box::init() {
	return cwindow::init();
}

//void cmessage_box::click_in(c_point p) {
//	cwindow::click_in(p);
//}
//
//void cmessage_box::double_click(c_point p) {
//	cwindow::double_click(p);
//}
//
//void cmessage_box::mouse_move_in(c_point p) {
//	cwindow::mouse_move_in(p);
//}
//
//void cmessage_box::mouse_move_out(c_point p) {
//	cwindow::mouse_move_out(p);
//}
//
//void cmessage_box::drag(drag_info p) {
//	cwindow::drag(p);
//	update();
//}

void cmessage_box::do_mode() {
	set_is_show(true);
	cframe::instance().set_active_ctr(this);
	to_top(this);
	update();
}
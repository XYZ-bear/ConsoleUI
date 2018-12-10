#include "stdafx.h"
#include "cedit.h"
#include "ctimer.h"

cedit::cedit()
{
	set_ctr_type(T_edit);
	is_spin = false;
	is_focus = false;
}


cedit::~cedit()
{
}

bool cedit::update()
{
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	return true;
}

void cedit::click_in(c_point p) {
	is_focus = true;
}

void cedit::click_out(c_point p) {
	is_focus = false;
}

bool cedit::init() {
	ctimer::instance().add_timer(this,500,&cedit::test);
	return true;
}

void cedit::double_click(c_point p) {

}

void cedit::mouse_move_in(c_point p) {
	_active_color = _mouse_in_color;
}

void cedit::mouse_move_out(c_point p) {
	_active_color = _color;
}

void cedit::test() {
	if (is_focus) {
		if (is_spin) {
			_gdi.draw_line({ 2,2 }, { 2 ,_height - 2 });
			is_spin = false;
		}
		else {
			_gdi.draw_line({ 2,2 }, { 2 ,_height - 2 }, 1, RGB(0, 0, 0));
			is_spin = true;
		}
	}
}
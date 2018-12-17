#include "stdafx.h"
#include "cscroll.h"
#include "ctimer.h"
#include "cwindow.h"

cscroll::cscroll()
{
	set_ctr_type(T_scroll);
}


cscroll::~cscroll()
{
}

bool cscroll::update()
{
	erase_bk();
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	_gdi.fill_rect(scroll_bar_, RGB(255, 0, 144));
	return cwbase::update();
}

void cscroll::click_in(c_point p) {
	//ctimer::instance().add_timer(this, 500, &cscroll::test);
}

void cscroll::click_out(c_point p) {
	//ctimer::instance().kill_timer(this, &cscroll::test);
}

bool cscroll::init() {
	//ctimer::instance().add_timer(this, 500, &cscroll::test);
	scroll_bar_ = { {0,0},_width,70 };
	return true;
}

void cscroll::double_click(c_point p) {

}

void cscroll::mouse_move_in(c_point p) {
	_active_color = _mouse_in_color;
}

void cscroll::mouse_move_out(c_point p) {
	_active_color = _color;
}
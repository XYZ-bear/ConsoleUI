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

bool cscroll::update(bool redraw)
{
	//erase_bk();
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	_gdi.fill_rect({ 0,0 }, { _width ,_height }, _bk_color);
	_gdi.fill_rect(scroll_bar_, bar_active_color_);
	return cwbase::update(redraw);
}

void cscroll::click_in(c_point p) {
	old_point_= p;
	if (style_ == T_h_scroll) {
		if (old_point_.y < scroll_bar_.p.y)
			scroll_to_(scroll_bar_.p.y - _height * one_step_);
		else if (old_point_.y > scroll_bar_.p.y + scroll_bar_.height)
			scroll_to_(scroll_bar_.p.y + _height * one_step_);
	}
	else if (style_ == T_v_scroll)
		scroll_bar_ = { { scroll_bar_offset,scroll_bar_offset },70,_height - 2 * scroll_bar_offset };
	update();
	//if(scroll_bar_.is_point_in(old_point_))
	//ctimer::instance().add_timer(this, 500, &cscroll::test);
}

void cscroll::click_out(c_point p) {
	//ctimer::instance().kill_timer(this, &cscroll::test);
}

bool cscroll::init() {
	//ctimer::instance().add_timer(this, 500, &cscroll::test);
	if (style_ == T_h_scroll)
		scroll_bar_ = { { scroll_bar_offset,scroll_bar_offset },_width - 2 * scroll_bar_offset,70 };
	else if (style_ == T_v_scroll)
		scroll_bar_ = { { scroll_bar_offset,scroll_bar_offset },70,_height - 2 * scroll_bar_offset };
	bar_active_color_ = bar_color_;
	return cwbase::init();
}

void cscroll::double_click(c_point p) {

}

void cscroll::mouse_move_in(c_point p) {
	_active_color = _mouse_in_color;
	bar_active_color_ = bar_drag_color_;
	update();
}

void cscroll::mouse_move_out(c_point p) {
	_active_color = _bk_color;
	bar_active_color_ = bar_color_;
	update();
}

void cscroll::drag(c_point p) {
	if (style_ == T_h_scroll) {
		scroll_to_(scroll_bar_.p.y + (p.y));
	}
	else if (style_ == T_v_scroll) {
		scroll_to_(scroll_bar_.p.x + (p.x - old_point_.x));
	}
	old_point_ = p;
	update();
}

void cscroll::scroll_to_(int xy) {
	if (style_ == T_h_scroll) {
		int res_y_bottom = xy + scroll_bar_.height;
		if (xy < scroll_bar_offset)
			scroll_bar_.p.y = scroll_bar_offset;
		else if (res_y_bottom > _height - scroll_bar_offset)
			scroll_bar_.p.y = _height - scroll_bar_offset - scroll_bar_.height;
		else
			scroll_bar_.p.y = xy;
	}
	else if (style_ == T_v_scroll) {
		int res_y_right = xy + scroll_bar_.width;
		if (xy<scroll_bar_offset || res_y_right>_width - scroll_bar_offset)
			return;
		scroll_bar_.p.x = xy;
	}
	if (call_func_)
		call_func_(T_scroll_event, nullptr);
}
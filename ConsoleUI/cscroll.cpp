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
	erase_bk();
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
	else if (style_ == T_v_scroll) {
		if (old_point_.x < scroll_bar_.p.x)
			scroll_to_(scroll_bar_.p.x - _width * one_step_);
		else if (old_point_.x > scroll_bar_.p.x + scroll_bar_.width)
			scroll_to_(scroll_bar_.p.x + _width * one_step_);
	}
	update();
}

void cscroll::click_out(c_point p) {
}

bool cscroll::init() {
	if (style_ == T_h_scroll)
		scroll_bar_ = { { scroll_bar_offset,scroll_bar_offset },_width - 2 * scroll_bar_offset,scroll_bar_.height };
	else if (style_ == T_v_scroll)
		scroll_bar_ = { { scroll_bar_offset,scroll_bar_offset },scroll_bar_.width,_height - 2 * scroll_bar_offset };
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
		scroll_to_(scroll_bar_.p.x + p.x);
	}
	old_point_ = p;
	update();
}

void cscroll::mouse_wheeled(bool up) {
	int move;
	if (up) {
		move = -1;
		scroll(-1);
	}
	else {
		move = 1;
		scroll(1);
	}
	update();
	call_func_(T_scroll_event, &move);
}

void cscroll::scroll_to_(int xy) {
	if (style_ == T_h_scroll) {
		int old_y = scroll_bar_.p.y;
		int res_y_bottom = xy + scroll_bar_.height;
		if (xy < scroll_bar_offset)
			scroll_bar_.p.y = scroll_bar_offset;
		else if (res_y_bottom > _height - scroll_bar_offset)
			scroll_bar_.p.y = _height - scroll_bar_offset - scroll_bar_.height;
		else
			scroll_bar_.p.y = xy;
	}
	else if (style_ == T_v_scroll) {
		int old_x = scroll_bar_.p.x;
		int res_x_right = xy + scroll_bar_.width;
		if (xy < scroll_bar_offset)
			scroll_bar_.p.x = scroll_bar_offset;
		else if (res_x_right > _width - scroll_bar_offset)
			scroll_bar_.p.x = _width - scroll_bar_offset - scroll_bar_.width;
		else
			scroll_bar_.p.x = xy;
	}
	scroll_info info{ min_pos_,max_pos_,(scroll_bar_.p.y- scroll_bar_offset )*max_pos_ / (_height - 2 * scroll_bar_offset) };
	call_func_(T_scroll_event, &info);
}

void cscroll::scroll(int xy) {
	if (style_ == T_h_scroll) {
		scroll_to_(scroll_bar_.p.y + xy);
	}
	else if (style_ == T_v_scroll) {
		scroll_to_(scroll_bar_.p.x + xy);
	}
	
	update();
}

void cscroll::set_range(int min, int max) { 
	min_pos_ = min;
	max_pos_ = max;
	if (style_ == T_h_scroll)
		scroll_bar_.height = (_height - 2 * scroll_bar_offset) * (_height - 2 * scroll_bar_offset) / (max - min);
	else
		scroll_bar_.width = (_width - 2 * scroll_bar_offset) * (_width - 2 * scroll_bar_offset) / (max - min);
}
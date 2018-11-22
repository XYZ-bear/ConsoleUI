#include "stdafx.h"
#include "cframe.h"
#include "cwindow.h"
#include "cbutton.h"

cwindow::cwindow()
{
}


cwindow::~cwindow()
{
	for (auto &child : chidren_list)
		if (child)
			delete child;
}

bool cwindow::init() {
	is_mouse_in_header = false;
	header_height = 30;
	cbutton *button = new cbutton();;
	button->create({ 10,60 }, 30, 20);
	add_child(button);

	cbutton *close_button = new cbutton();;
	close_button->create({ _width-30,5 }, 20, 20);
	regist_control_call(MOUSE_CLICK_EVENT, close_button, &cwindow::close_click);
	add_child(close_button);

	return true;
}

bool cwindow::update() {
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height }, 10, RGB(255, 0, 0));
	_gdi.draw_line({ 0,header_height }, { _width,header_height }, 2, RGB(255, 144, 0));
	for (auto &child : chidren_list) {
		child->update();
		BitBlt(get_gdi().buffer_hdc_, child->get_gdi().refer_c_point_.x, child->get_gdi().refer_c_point_.y, child->get_gdi().width_, child->get_gdi().height_, child->get_gdi().buffer_hdc_, 0, 0, SRCCOPY);
	}
	return true;
}

bool cwindow::add_child(cwbase *child) {
	if (child) {
		chidren_list.push_back(child);
		return true;
	}
	return false;
}

bool cwindow::create(string title, c_point op, int width, int height) {
	title_ = title;
	old_init_point = op;
	pre_point = op;
	old_rect = { op,width,height };
	return cwbase::create(op,width,height);
}

void cwindow::click_in(c_point p) {
	if (p.x<_right_bottom.x&&p.x>_left_top.x&&p.y > _left_top.y&&p.y < _left_top.y + header_height)
		is_mouse_in_header = true;
	else
		is_mouse_in_header = false;
	
	pre_point = p;
	auto it = chidren_list.end();
	while (chidren_list.begin() != it) {
		auto child = *(--it);
		if (child&&child->is_point_in(p-get_left_top())) {
			child->click_in(p);
			call_func_(MOUSE_CLICK_EVENT, child,&p);
			break;
		}
	}
}

void cwindow::click_out(c_point p) {
	is_mouse_in_header = false;
}

void cwindow::mouse_move(c_point p) {
	if (p > c_point{ 0,0 }&&p < c_point{ get_console_width(),get_console_height() }) {
		if (is_mouse_in_header) {
			auto move = p - pre_point;
			set_point(move + _left_top);
			old_rect.p = move + _left_top;
		}
	}
}

void cwindow::close_click(cwbase *base, void* p) {
	c_point *res = (c_point*)p;
	MessageBox(GetConsoleWindow(), "close", "", 1);
	is_close_ = true;
}

void cwindow::double_click(c_point p) {
	if (is_mouse_in_header) {
		if (!is_max_) {
			cwbase::create({ 0,0 }, get_console_width(), get_console_height());
			is_max_ = true;
		}
		else {
			cwbase::create(old_rect.p, old_rect.width, old_rect.height);
			is_max_ = false;
		}
	}
}
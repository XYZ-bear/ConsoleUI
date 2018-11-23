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
	close_button->create({ _width-25,5 }, 15, 15,RGB(255,0,0));
	close_button->set_type(T_circle_button);
	regist_control_call(D_mouse_click_event, close_button, &cwindow::close_click);
	add_child(close_button);

	cbutton *close_button2 = new cbutton();;
	close_button2->create({ _width - 45,5 }, 15, 15, RGB(0, 128, 0));
	close_button2->set_type(T_circle_button);
	add_child(close_button2);

	cbutton *close_button3 = new cbutton();;
	close_button3->create({ _width - 65,5 }, 15, 15, RGB(255, 215, 0));
	close_button3->set_type(T_circle_button);
	add_child(close_button3);
	return true;
}

bool cwindow::update() {
	_gdi.fill_rect(get_client_point(_left_top), get_client_point(_right_bottom),1,RGB(0,0,0));
	_gdi.draw_ellipse({ 50,50 },20, RGB(255, 0, 0));
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height }, 10, RGB(255, 0, 0));
	_gdi.draw_line({ 0,header_height }, { _width,header_height }, 2, RGB(255, 144, 0));
	_gdi.draw_text(title_, {10,5});
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
			call_func_(D_mouse_click_event, child,&p);
			break;
		}
	}
}

void cwindow::click_out(c_point p) {
	is_mouse_in_header = false;
}

void cwindow::mouse_move(c_point p) {
	if (p.x > _right_bottom.x - 5 && p.x < _right_bottom.x + 5 && p.y > _right_bottom.y - 5 && p.y < _right_bottom.y + 5) {
		hint_t_ = T_hint_right_bottom;
		hint(p);
	}
	else if (p.x > _right_bottom.x - 5&& p.x < _right_bottom.x + 5) {
		hint_t_ = T_hint_right;
		hint(p);
	}
	else if (p.y > _right_bottom.y - 5 && p.y < _right_bottom.y + 5) {
		hint_t_ = T_hint_bottom;
		hint(p);
	}
	else
		hint_t_ = T_hint_nono;
}

void cwindow::close_click(cwbase *base, void* p) {
	c_point *res = (c_point*)p;
	MessageBox(GetConsoleWindow(), "close", "", 1);
	is_close_ = true;
}

void cwindow::double_click(c_point p) {
	if (is_mouse_in_header) {
		if (!is_max_) {
			set_size({ 0,0 }, get_console_width(), get_console_height());
			is_max_ = true;
			for (auto &child : chidren_list) {
				int move = old_rect.width - _width;
				child->set_point({ child->get_point().x - move,child->get_point().y });
			}
		}
		else {
			set_size(old_rect.p, old_rect.width, old_rect.height);
			is_max_ = false;
			for (auto &child : chidren_list) {
				int move = get_console_width() - _width;
				child->set_point({ child->get_point().x - move,child->get_point().y });
			}
		}

	}
}

void cwindow::hint(c_point p) {
	p = get_client_point(p);
	if (hint_t_ == T_hint_right) {
		_gdi.draw_line({ _width - 20,p.y }, { _width - 4 ,p.y }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ _width - 10,p.y - 6 }, { _width - 4 ,p.y }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ _width - 10,p.y + 6 }, { _width - 4 ,p.y }, 1, RGB(255, 0, 0));
	}
	else if (hint_t_ == T_hint_bottom) {
		_gdi.draw_line({ p.x,_height - 20 }, { p.x ,_height - 4 }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ p.x - 6,_height - 10 }, { p.x ,_height - 4 }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ p.x + 6,_height - 10 }, { p.x ,_height - 4 }, 1, RGB(255, 0, 0));
	}
	else if (hint_t_ == T_hint_right_bottom) {
		_gdi.draw_line({ _width - 20,_height - 20 }, { _width - 4 ,_height - 4 }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ _width - 15,_height - 4 }, { _width-3 ,_height - 4 }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ _width - 4,_height - 4 }, { _width- 4 ,_height - 15 }, 1, RGB(255, 0, 0));
	}
}

void cwindow::drag(c_point p) {
	if (p > c_point{ 0,0 }&&p < c_point{ get_console_width(),get_console_height() }) {
		if (is_mouse_in_header) {
			auto move = p - pre_point;
			set_point(move + _left_top);
			old_rect.p = move + _left_top;
			is_drag_ = true;
		}
	}
	old_rect.width = _width;
	old_rect.height = _height;
	auto move = p - pre_point;
	if (hint_t_== T_hint_right) {
		set_size(_left_top, _width + move.x, _height);
	}
	else if (hint_t_ == T_hint_bottom) {
		set_size(_left_top, _width, _height + move.y);
	}
	else if (hint_t_ == T_hint_right_bottom) {
		set_size(_left_top, _width + move.x, _height + move.y);
	}
	size_change(old_rect);
}

c_point cwindow::get_client_point(c_point p) {
	return p - _left_top;
}

void cwindow::size_change(c_rect rect) {
	for (auto &child : chidren_list) {
		int move = old_rect.width - _width;
		child->set_point({ child->get_point().x - move,child->get_point().y });
	}
}
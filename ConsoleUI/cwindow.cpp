#include "stdafx.h"
#include "cframe.h"
#include "cwindow.h"
#include "cbutton.h"
#include "ccombox.h"
#include "cmenu.h"
#include "cedit.h"
#include "ctimer.h"
#include "cscroll.h"

cwindow::cwindow()
{
	set_ctr_type(T_window);
}


cwindow::~cwindow()
{
	for (auto &child : chidren_list)
		if (child)
			delete child;
	delete brigde_;
}

bool cwindow::init() {
	is_mouse_in_header = false;
	header_height = 30;
	cbutton *button = new cbutton();;
	button->create({ 10,60 }, 30, 20);
	add_child(button);

	cbutton *close_button = new cbutton();;
	close_button->create({ _width-25,5 }, 15, 15,this,RGB(144,0,0));
	close_button->set_type(T_circle_button);
	close_button->set_mouse_in_color(RGB(255, 0, 0));
	regist_control_call(D_mouse_click_event, close_button, &cwindow::close_click);
	add_child(close_button);

	cbutton *close_button2 = new cbutton();;
	close_button2->create({ _width - 45,5 }, 15, 15, this, RGB(0, 128, 0));
	close_button2->set_type(T_circle_button);
	regist_control_call(D_mouse_click_event, close_button2, &cwindow::max_click);
	add_child(close_button2);

	cbutton *close_button3 = new cbutton();;
	close_button3->create({ _width - 65,5 }, 15, 15, this, RGB(255, 215, 0));
	close_button3->set_type(T_circle_button);
	add_child(close_button3);


	cmenu *comb = new cmenu(this);
	comb->get_root().text = "com1";
	comb->get_root()[0].text = "root1";
	comb->get_root()[1].text = "root2";
	comb->get_root()[2].text = "root3";
	comb->get_root()[3].text = "root4";
	comb->create({ 65,65 }, 70,90, this,RGB(255, 0, 0));
	add_child(comb);

	cedit *edit = new cedit();
	edit->create({ 200,100 }, 100, 200, this, RGB(255, 215, 0));
	edit->set_style(T_multiline_edit);
	add_child(edit);

	tips.create({ 0,0 }, 100, 20, this);
	tips.add_tip(close_button, "关闭");
	tips.add_tip(edit, "编辑sfsfsds");
	tips.add_tip(close_button3, "最小化fsdsssssssssssssssssssssdfsd");
	add_child(&tips);

	cscroll *scroll = new cscroll();
	scroll->create({ 310,30 }, 20, 200, this);
	add_child(scroll);
	return true;
}

bool cwindow::update() {
	erase_bk();
	_gdi.draw_ellipse({ 50,50 },20, RGB(255, 0, 0));
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height }, 10, RGB(255, 0, 0));
	_gdi.draw_line({ 0,header_height }, { _width,header_height }, 2, RGB(255, 144, 0));
	_gdi.draw_text(title_, {10,5});

	if (hint_t_ == T_hint_right) {
		_gdi.draw_line({ _width - 20,hint_point.y }, { _width - 4 ,hint_point.y }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ _width - 10,hint_point.y - 6 }, { _width - 4 ,hint_point.y }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ _width - 10,hint_point.y + 6 }, { _width - 4 ,hint_point.y }, 1, RGB(255, 0, 0));
	}
	else if (hint_t_ == T_hint_bottom) {
		_gdi.draw_line({ hint_point.x,_height - 20 }, { hint_point.x ,_height - 4 }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ hint_point.x - 6,_height - 10 }, { hint_point.x ,_height - 4 }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ hint_point.x + 6,_height - 10 }, { hint_point.x ,_height - 4 }, 1, RGB(255, 0, 0));
	}
	else if (hint_t_ == T_hint_right_bottom) {
		_gdi.draw_line({ _width - 20,_height - 20 }, { _width - 4 ,_height - 4 }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ _width - 15,_height - 4 }, { _width - 3 ,_height - 4 }, 1, RGB(255, 0, 0));
		_gdi.draw_line({ _width - 4,_height - 4 }, { _width - 4 ,_height - 15 }, 1, RGB(255, 0, 0));
	}

	return true;
}

void cwindow::update_window(bool redraw) {
	update();
	for (auto &child : chidren_list) {
		if (child->is_show()) {
			if (redraw)
				child->update();
			else if (child->get_gdi().get_change())
				BitBlt(get_gdi().buffer_hdc_, child->get_gdi().refer_c_point_.x, child->get_gdi().refer_c_point_.y, child->get_gdi().width_, child->get_gdi().height_, child->get_gdi().buffer_hdc_, 0, 0, SRCCOPY);
		}
	}
}

bool cwindow::add_child(cwbase *child) {
	if (child) {
		child->set_align(prejudge_align_v(child->get_point()));
		chidren_list.push_back(child);
		child->init();
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
	bool is_top_click_ctr = false;
	while (chidren_list.begin() != it) {
		auto child = *(--it);
		if (child&&child->is_point_in(p-get_left_top())&&
			is_top_click_ctr==false && child->enable_focus()) {
			child->click_in(p);
			call_func_(D_mouse_click_event, child,&p);
			child->set_is_focus(true);
			focus_ctr = child;
			is_top_click_ctr = true;
			//break;
		}
		else {
			child->click_out(p);
			child->set_is_focus(false);
		}
	}
}

void cwindow::click_out(c_point p) {
	is_mouse_in_header = false;
	auto it = chidren_list.end();
	while (chidren_list.begin() != it) {
		auto child = *(--it);
		child->click_out(p);
		child->set_is_focus(false);
	}
}

void cwindow::mouse_move(c_point p) {
	if (p.x > _right_bottom.x - 5 && p.x < _right_bottom.x + 5 && p.y > _right_bottom.y - 5 && p.y < _right_bottom.y + 5)
		hint_t_ = T_hint_right_bottom;
	else if (p.x > _right_bottom.x - 5&& p.x < _right_bottom.x + 5) 
		hint_t_ = T_hint_right;
	else if (p.y > _right_bottom.y - 5 && p.y < _right_bottom.y + 5) 
		hint_t_ = T_hint_bottom;
	else 
		hint_t_ = T_hint_nono;
	hint(p);

	auto client_p = get_client_point(p);
	if (auto t = point_in_ctr(p)) {
		if (!point_ctr) {
			t->mouse_move_in(client_p);
			point_ctr = t;
		}
	}
	else {
		if (point_ctr) {
			point_ctr->mouse_move_out(client_p);
			point_ctr = nullptr;
		}
	}

	if (point_ctr) {
		tips.show_tip(point_ctr, client_p);
	}
	else
		tips.set_is_show(false);
}

void cwindow::close_click(cwbase *base, void* p) {
	c_point *res = (c_point*)p;
	//MessageBox(GetConsoleWindow(), "close", "", 1);
	is_close_ = true;
}

void cwindow::max_click(cwbase *base, void* p) {
	c_point *res = (c_point*)p;
	double_click(*res);
}

void cwindow::double_click(c_point p) {
	if (is_mouse_in_header) {
		if (!is_max_) {
			set_size({ 0,0 }, get_console_width(), get_console_height());
			is_max_ = true;
			size_change(old_rect);
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
	hint_point = get_client_point(p);
	update_window();
}

void cwindow::drag(c_point p) {
	if (p > c_point{ 0,0 }&&p < c_point{ get_console_width(),get_console_height() }) {
		if (is_mouse_in_header) {
			auto move = p - pre_point;
			set_point(move + _left_top);
			old_rect.p = move + _left_top;
			is_drag_ = true;
			if (is_max_) {
				set_size(p - c_point{(p.x*old_rect.width )/ get_console_width(),(p.y*old_rect.height) / get_console_height()}, old_rect.width, old_rect.height);
				is_max_ = false;
				for (auto &child : chidren_list) {
					int move = get_console_width() - _width;
					child->set_point({ child->get_point().x - move,child->get_point().y });
				}
			}
		}
	}
	//if (hint_t_ != T_hint_nono) {
		old_rect.width = _width;
		old_rect.height = _height;
		auto move = p - pre_point;
		if (hint_t_ == T_hint_right) {
			set_size(_left_top, _width + move.x, _height);
		}
		else if (hint_t_ == T_hint_bottom) {
			set_size(_left_top, _width, _height + move.y);
		}
		else if (hint_t_ == T_hint_right_bottom) {
			set_size(_left_top, _width + move.x, _height + move.y);
		}
		size_change(old_rect);
		update_window();
	//}
}

c_point cwindow::get_client_point(c_point p) {
	return p - _left_top;
}

void cwindow::size_change(c_rect rect) {
	for (auto &child : chidren_list) {
		if (child->get_align() == T_v_align_right) {
			int move = old_rect.width - _width;
			child->set_point({ child->get_point().x - move,child->get_point().y });
		}
	}
}

void cwindow::input_key(c_key key) {
	if (focus_ctr) {
		if (focus_ctr->is_focus()) {
			focus_ctr->input_key(key);
		}
	}
}

T_align cwindow::prejudge_align_v(c_point p) {
	if (p.x <= _width && p.x > _width / 2)
		return T_v_align_right;
	return T_v_align_left;
}

T_align cwindow::prejudge_align_h(c_point p) {
	if (p.y <= _height && p.y > _height / 2)
		return T_h_align_right;
	return T_h_align_left;
}

cwbase* cwindow::point_in_ctr(c_point p) {
	auto it = chidren_list.end();
	while (chidren_list.begin() != it) {
		auto child = *(--it);
		if (child&&child->is_point_in(get_client_point(p))) {
			return child;
		}
	}
	return nullptr;
}

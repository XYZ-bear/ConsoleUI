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

	cbutton *close_button = new cbutton();;
	close_button->create({ _width-25,5 }, 15, 15,this,RGB(144,0,0));
	close_button->set_type(T_circle_button);
	close_button->set_mouse_in_color(RGB(255, 0, 0));
	close_button->add_cmd(this, T_click_in_event, &cwindow::close_click);
	close_button->set_tip("¹Ø±Õ");
	//regist_control_call(D_mouse_click_event, close_button, &cwindow::close_click);

	cbutton *close_button2 = new cbutton();;
	close_button2->create({ _width - 45,5 }, 15, 15, this, RGB(0, 128, 0));
	close_button2->set_type(T_circle_button);
	close_button2->add_cmd(this, T_click_in_event, &cwindow::max_click);
	//////regist_control_call(D_mouse_click_event, close_button2, &cwindow::max_click);

	cbutton *close_button3 = new cbutton();
	close_button3->create({ _width - 65,5 }, 15, 15, this, RGB(255, 215, 0));
	close_button3->set_type(T_circle_button);


	////cmenu *comb = new cmenu(this);
	////comb->get_root().text = "com1";
	////comb->get_root()[0].text = "root1";
	////comb->get_root()[1].text = "root2";
	////comb->get_root()[2].text = "root3";
	////comb->get_root()[3].text = "root4";
	////comb->create({ 65,65 }, 70,90, this,RGB(255, 0, 0));
	////add_child(comb);

	cedit *edit = new cedit();
	edit->create({ 60,50 }, 100, 200, this, RGB(255, 215, 0));
	edit->set_style(T_multiline_edit);

	cscroll *scroll = new cscroll();
	scroll->create({ 310,30 }, 12, 200, this);
	scroll->set_bk_color(RGB(62,62,62));
	scroll->set_bar_color(RGB(104,104,104));
	scroll->set_bar_drag_color(RGB(200,200,200));
	scroll->set_tip("¹ö¹ö¶´");

	//cscroll *scrollv = new cscroll();
	//scrollv->create({ 100,300 }, 200, 12, this);
	//scrollv->set_style(T_v_scroll);

	//tips.create({ 0,0 }, 100, 20, this);

	return cwbase::init();
}

bool cwindow::update(bool redraw) {
	erase_bk();
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

	return cwbase::update(redraw);
}

bool cwindow::create(string title, c_point op, int width, int height) {
	title_ = title;
	old_init_point = op;
	pre_point = op;
	old_rect = { op,width,height };
	return cwbase::create(op,width,height, &cframe::instance());
}

void cwindow::click_in(c_point p) {
	if (p.y < header_height)
		is_mouse_in_header = true;
	else
		is_mouse_in_header = false;
	
	pre_point = p;
}

void cwindow::click_out(c_point p) {
	is_mouse_in_header = false;
}

void cwindow::mouse_move(c_point p) {
	if (p.x > _width - 5 && p.x < _right_bottom.x + 5 && p.y > _height - 5 && p.y < _height + 5)
		hint_t_ = T_hint_right_bottom;
	else if (p.x > _width - 5 && p.x < _width + 5)
		hint_t_ = T_hint_right;
	else if (p.y > _height - 5 && p.y < _height + 5)
		hint_t_ = T_hint_bottom;
	else
		hint_t_ = T_hint_nono;
	hint(p);
}

void cwindow::close_click(const void* p) {
	c_point *res = (c_point*)p;
	//MessageBox(GetConsoleWindow(), "close", "", 1);
	is_close_ = true;
	remove_self();
}

void cwindow::max_click(const void* p) {
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
			//for (auto &child : chidren_list) {
			//	int move = get_console_width() - _width;
			//	child->set_point({ child->get_point().x - move,child->get_point().y });
			//}
		}

	}
}

void cwindow::hint(c_point p) {
	hint_point = p;
}

void cwindow::drag(c_point p) {
	//if (p > c_point{ 0,0 }&&p < c_point{ get_console_width(),get_console_height() }) {
		if (is_mouse_in_header) {
			set_point(p + _left_top);
			old_rect.p = p + _left_top;
			is_drag_ = true;
			if (is_max_) {
				set_size(p - c_point{(p.x*old_rect.width )/ get_console_width(),(p.y*old_rect.height) / get_console_height()}, old_rect.width, old_rect.height);
				is_max_ = false;
				//for (auto &child : chidren_list) {
				//	int move = get_console_width() - _width;
				//	child->set_point({ child->get_point().x - move,child->get_point().y });
				//}
			}
		}
	//}
	if (hint_t_ != T_hint_nono) {
		old_rect.width = _width;
		old_rect.height = _height;
		if (hint_t_ == T_hint_right) {
			set_size(_left_top, _width + p.x, _height);
		}
		else if (hint_t_ == T_hint_bottom) {
			set_size(_left_top, _width, _height + p.y);
		}
		else if (hint_t_ == T_hint_right_bottom) {
			set_size(_left_top, _width + p.x, _height + p.y);
		}
		size_change(old_rect);
	}

	//pre_point = p;
}

void cwindow::size_change(c_rect rect) {
	//for (auto &child : chidren_list) {
	//	if (child->get_align() == T_v_align_right) {
	//		int move = old_rect.width - _width;
	//		child->set_point({ child->get_point().x - move,child->get_point().y });
	//	}
	//}
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
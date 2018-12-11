#include "stdafx.h"
#include "cedit.h"
#include "ctimer.h"
#include "cwindow.h"

cedit::cedit()
{
	set_ctr_type(T_edit);
	is_spin = false;
	spin_x_ = 2;
	spin_y_ = 2;
}


cedit::~cedit()
{
}

bool cedit::update()
{
	erase_bk();
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	_gdi.draw_text(text_, { 2,2 }, _height - 4);
	if (is_spin) {
		_gdi.draw_line({ spin_x_,spin_y_ }, { spin_x_ ,_height - spin_y_ });
		//is_spin = false;
	}
	else {
		_gdi.draw_line({ spin_x_,spin_y_ }, { spin_x_ ,_height - spin_y_ }, 1, RGB(0, 0, 0));
		//is_spin = true;
	}
	return cwbase::update();
}

void cedit::click_in(c_point p) {
	//ctimer::instance().add_timer(this, 500, &cedit::test);
	test();
	set_timer(this,500, &cedit::test);
}

void cedit::click_out(c_point p) {
	//ctimer::instance().kill_timer(this, &cedit::test);
	kill_timer(this,&cedit::test);
	is_spin = false;
}

bool cedit::init() {
	//ctimer::instance().add_timer(this, 500, &cedit::test);
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
	//update();
	if (is_focus()) {
		if (is_spin) {
			_gdi.draw_line({ spin_x_,spin_y_ }, { spin_x_ ,_height - spin_y_ });
			is_spin = false;
		}
		else {
			_gdi.draw_line({ spin_x_,spin_y_ }, { spin_x_ ,_height - spin_y_ }, 1, RGB(0, 0, 0));
			is_spin = true;
		}
	}
	update_parent();
	//((cwindow*)_parent)->update_window();
	//update();
}

void cedit::input_key(char key) {
	if (key == VK_BACK) {
		if (text_.size() > 0) {
			text_.pop_back();
			spin_x_ -= (_height - 4) / 2;
		}
		return;
	}
	if (key >= 'A' && key <= 'z'||key!=0) {
		text_ += key;
		spin_x_ += (_height - 4) / 2;
	}
}
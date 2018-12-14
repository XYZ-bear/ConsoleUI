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
	if (style_ == T_singleline_edit)
		_gdi.draw_text(text_, { 2,2 }, _font_height);
	else {
		_gdi.draw_text(text_, { 2,2 }, _font_height);

		size_t pre_pos = 0;
		size_t old_pos = 0;
		while (1) {
			//pos = text_.find_first_of('\r', pos);
			//_gdi.draw_text(text_.substr(0, pos - 1), { 2,ypos }, _font_height);
			//if(pos==-1)
		}
		//size_t pos = 0; 
		//text_.substr(0, pos);
		//int ypos = 2;
		//do {
		//	pos = text_.find_first_of('\r', pos);
		//	_gdi.draw_text(text_.substr(0, pos-1), { 2,ypos }, _font_height);
		//	ypos += _font_height;
		//} while (pos != -1);
		//while (pos = text_.find_first_of('\r', pos) != -1) {
		//	_gdi.draw_text(text_.substr(0, pos), { 2,ypos }, _font_height);
		//	ypos += _font_height;
		//}
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
	if (style_ == T_singleline_edit) {
		_font_height = _height - 4;
	}
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
	if (is_focus()) {
		auto rgb = _bk_color;
		if (is_spin) {
			rgb = spin_color_;
			is_spin = false;
		}
		else {
			rgb = _bk_color;
			is_spin = true;
		}
		_gdi.draw_line({ spin_x_,spin_y_ }, { spin_x_ ,_font_height + spin_y_ }, 1, rgb);
	}
	update_parent();
}

void cedit::input_key(c_key key) {
	if (key.wVirtualKeyCode == VK_BACK) {
		if (text_.size() > 0) {
			if ((*(text_.end()-1)) < 0) {   //ºº×ÖË«×Ö½Ú±àÂë
				text_.pop_back();
				spin_x_ -= _font_height / 2;
			}
			text_.pop_back();
			spin_x_ -= _font_height / 2;
			goto update;
		} 
	}
	else if (key.wVirtualKeyCode == VK_RIGHT) {
		if (text_.size() > 0) {
			int t = spin_x_ + _font_height / 2;
			if (t > _width) {
				spin_x_ = 2;
				spin_y_ += _font_height;
			}
			else
				spin_x_ = t;
			goto update;
		}
	}
	else if (key.wVirtualKeyCode == VK_LEFT) {
		if (text_.size() > 0) {
			int t = spin_x_ - _font_height / 2;
			if (t < 0) {
				spin_x_ = _width - 2;
				spin_y_ -= _font_height;
			}
			else
				spin_x_ = t;
			goto update;
		}
	}
	if (key.uChar.AsciiChar!=0) {
		text_ += key.uChar.AsciiChar;
		spin_x_ += _font_height / 2;
	}

	update:
	update();
}
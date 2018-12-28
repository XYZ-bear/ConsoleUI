#include "stdafx.h"
#include "cedit.h"
#include "ctimer.h"
#include "cwindow.h"
#include <fstream>

cedit::cedit()
{
	set_ctr_type(T_edit);
	is_spin = false;
	spin_x_ = 2;
	spin_y_ = 2;
	spin_x_off_ = 0;
	spin_y_off_ = 0;
}


cedit::~cedit()
{
}

//bool cedit::update(bool redraw)
//{
//	erase_bk();
//	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
//	draw_select();
//	if (style_ == T_singleline_edit)
//		_gdi.draw_text(text_, off_margin_, _font_height);
//	else {
//		int ypos = off_margin_.y + line_off;
//		int xpos = scroll_xy.x + off_margin_.x ;
//
//		int len = start_line_ + max_line_;
//		for (int i = start_line_; i < len; i++) {
//			_gdi.draw_text_t(v_text_[i], { xpos,ypos }, _font_height);
//			ypos += _font_height + line_off;
//		}
//	}
//
//	return cwbase::update(redraw);
//}
//
//void cedit::draw_select() {
//	if (drag_start_point_ == drag_end_point_)
//		return;
//	
//	int begin_index = get_point_line_index(drag_start_point_);
//	int end_index = get_point_line_index(drag_end_point_);
//	if (begin_index > end_index)
//		swap(begin_index, end_index);
//	c_point start_point = get_point_spin_xy(drag_start_point_);
//	c_point end_point = get_point_spin_xy(drag_end_point_);
//	spin_point = end_point;
//
//	if (start_point.y > end_point.y)
//		swap(start_point, end_point);
//
//	if (begin_index == end_index)
//		_gdi.fill_rect(start_point, { end_point.x,end_point.y + _font_height + line_off }, RGB(0, 144, 244));
//	else {
//		for (int index = begin_index; index <= end_index; index++) {
//			if (index == end_index)
//				_gdi.fill_rect(start_point, { end_point.x,start_point.y + _font_height + line_off }, RGB(0, 144, 244));
//			else {
//				int end_x = get_text_end_x(get_index_line_text(index));
//				_gdi.fill_rect(start_point, { end_x,start_point.y + _font_height + line_off }, RGB(0, 144, 244));
//			}
//			start_point.x = off_margin_.x;
//			start_point.y += _font_height + line_off;
//		}
//	}
//}
//
//void cedit::click_in(c_point p) {
//	//ctimer::instance().add_timer(this, 500, &cedit::test);
//	
//	spin_point = get_point_spin_xy(p);
//	drag_start_point_ = p;
//	drag_end_point_ = p;
//	set_timer(this,500, &cedit::test);
//	update();
//}
//
//void cedit::click_out(c_point p) {
//	//ctimer::instance().kill_timer(this, &cedit::test);
//	kill_timer(this,&cedit::test);
//	is_spin = false;
//}
//
//bool cedit::init() {
//	if (style_ == T_singleline_edit) {
//		_font_height = _height - 4;
//	}
//
//
//	scroll_ = new cscroll();
//	scroll_->create({ _width - 12,0 }, 12, _height, this);
//	scroll_->set_bk_color(RGB(62, 62, 62));
//	scroll_->set_bar_color(RGB(104, 104, 104));
//	scroll_->set_bar_drag_color(RGB(200, 200, 200));
//	scroll_->add_cmd(this, T_scroll_event, &cedit::on_scroll);
//
//	max_line_ = get_max_line();
//	text_rect_ = {off_margin_,_width-scroll_->get_width(),_height};
//
//	ifstream myfile("log_2018-10-30.txt");
//	string temp;
//	myfile.is_open();
//	while (getline(myfile, temp))
//	{	
//		parase_tab(temp);
//		v_text_.push_back(temp);
//	}
//	myfile.close();
//
//	return cwbase::init();
//}
//
//void cedit::double_click(c_point p) {
//
//}
//
//void cedit::mouse_move_in(c_point p) {
//	_active_color = _mouse_in_color;
//}
//
//void cedit::focus() {
//	test();
//}
//
//void cedit::mouse_move_out(c_point p) {
//	_active_color = _bk_color;
//}
//
//void cedit::test() {
//	if (is_focus()) {
//		auto rgb = _bk_color;
//		if (is_spin) {
//			rgb = spin_color_;
//			is_spin = false;
//		}
//		else {
//			rgb = _bk_color;
//			is_spin = true;
//		}
//		_gdi.draw_line(old_spin_point, { old_spin_point.x ,_font_height + old_spin_point.y }, 1, _bk_color);
//		_gdi.draw_line(spin_point, { spin_point.x ,_font_height + spin_point.y }, 1, rgb);
//		old_spin_point = spin_point;
//	}
//	update_parent();
//}
//
//void cedit::input_key(c_key key) {
//	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('A') & 0x8000)) {
//		select_all();
//		goto update;
//	}
//	if (key.wVirtualKeyCode == VK_BACK) {
//		delete_ch();
//		//if (text_.size() > 0) {
//		//	if ((*(text_.end()-1)) < 0) {   //ºº×ÖË«×Ö½Ú±àÂë
//		//		text_.pop_back();
//		//		spin_x_ -= _font_height / 2;
//		//	}
//		//	text_.pop_back();
//		//	spin_x_ -= _font_height / 2;
//		//} 
//		goto update;
//	}
//	else if (key.wVirtualKeyCode == VK_RIGHT) {
//		set_spin_x( spin_point.x + _font_width );
//	}
//	else if (key.wVirtualKeyCode == VK_LEFT) {
//		set_spin_x(spin_point.x - _font_width);
//	}
//	else if (key.wVirtualKeyCode == VK_UP) {
//		spin_point.y -= (_font_height + line_off);
//	}
//	else if (key.wVirtualKeyCode == VK_DOWN) {
//		spin_point.y += (_font_height + line_off);
//	}
//	if (key.uChar.AsciiChar!=0) {
//		insert_ch(key.uChar.AsciiChar);
//	}
//		
//	update:
//	update();
//}
//
//void cedit::scroll_x(int dis) {
//	if (dis < 0) {//spin
//		if (scroll_xy.x >= 0) { //to up line
//			int index = get_point_line_index(spin_point);
//			if (index == 0)     //first line
//				return;
//			int end_x = get_text_end_x(get_index_line_text(index - 1));
//			if (end_x > text_rect_.width) {  //up line over right
//				spin_point.x = get_point_spin_xy({ text_rect_.width,0 }).x;
//			}
//			else
//				spin_point.x = end_x;
//		}
//		else {
//			scroll_xy.x += _font_width;
//			drag_start_point_.x += _font_width;
//		}
//	}
//	else if (dis > _width) {
//		int end_x = get_text_end_x(get_point_line_text(spin_point));
//		if (text_rect_.width - scroll_xy.x > end_x) {
//			spin_point.x = off_margin_.x;
//		}
//		else {
//			scroll_xy.x -= _font_width;
//			drag_start_point_.x -= _font_width;
//		}
//	}
//	else
//		spin_point.x = dis;
//}
//
//void cedit::scroll_y(int dis) {
//	scroll_xy.y += dis;
//	if (scroll_xy.y >= 0)
//		scroll_xy.y = 0;
//	if (start_line_ != 0) {
//		drag_end_point_.y += dis;
//		drag_start_point_.y += dis;
//	}
//	change_start_line((-dis) / (_font_height + line_off));
//}
//
//void cedit::on_scroll(const void *data) {
//	int move = *(int*)data;
//	//scroll_xy.y += (-move);
//	update();
//}
//
//void cedit::mouse_wheeled(bool up) {
//	if (up) {
//		scroll_->scroll(-(scroll_->get_height()*(_font_height + line_off) / _height));
//		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
//			change_font_height(get_font_size() + 2);
//		else
//			scroll_y(_font_height + line_off);
//		//scroll_xy.y += (_font_height + line_off);
//	}
//	else {
//		scroll_->scroll(scroll_->get_height()*(_font_height + line_off) / _height);
//		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
//			change_font_height(get_font_size() - 2);
//		else
//			scroll_y(-(_font_height + line_off));
//		//scroll_xy.y -= _font_height + line_off;
//	}
//	update();
//}
//
//void cedit::parase_text(string &text) {
//	size_t pre_pos = 0;
//	size_t old_pos = -1;
//	while (1) {
//		pre_pos = text.find_first_of('\r', old_pos + 1);
//		if (pre_pos == -1) {
//			v_text_.push_back(text.substr(old_pos + 1, text_.size() - old_pos - 1));
//			break;
//		}
//		else {
//			v_text_.push_back(text.substr(old_pos + 1, pre_pos - old_pos - 1));
//			old_pos = pre_pos;
//		}
//	}
//}
//
//void cedit::parase_tab(string &text) {
//	size_t pre_pos = 0;
//	size_t old_pos = -1;
//	while (1) {
//		pre_pos = text.find_first_of('\t', old_pos + 1);
//		if (pre_pos == -1) {
//			//v_text_.push_back(text.substr(old_pos + 1, text_.size() - old_pos - 1));
//			break;
//		}
//		else {
//			text.insert(text.begin() + pre_pos, ' ');
//			text.insert(text.begin() + pre_pos, ' ');
//			text.insert(text.begin() + pre_pos, ' ');
//			old_pos = pre_pos + 3;
//		}
//	}
//}
//
//c_point cedit::get_point_spin_xy(c_point p) {
//	string &text = get_point_line_text(p);
//	int x = get_text_end_x(text);
//	if (p.x < x)
//		x = p.x - (p.x - off_margin_.x) % _font_width;
//	int y = get_end_line_y();
//	if (p.y < y) 
//		y = p.y - (p.y - off_margin_.y) % (_font_height + line_off) + line_off + off_margin_.y;
//	return { x,y };
//}
//
//uint16_t cedit::get_text_end_x(string &text) {
//	return text.size()*_font_width + off_margin_.x;
//}
//
//uint16_t cedit::get_end_line_y() {
//	return (v_text_.size() - start_line_)*(_font_height + line_off) + line_off + off_margin_.y;
//}
//
//uint16_t cedit::get_point_line_index(c_point p) {
//	uint16_t index= start_line_ + (p.y - off_margin_.y) / (_font_height + line_off);
//	if (index > v_text_.size() - 1)
//		return v_text_.size() - 1;
//	return index;
//}
//
//string &cedit::get_point_line_text(c_point p) {
//	uint16_t index = get_point_line_index(p);
//	if (index > v_text_.size() - 1)
//		index = v_text_.size() - 1;
//	return v_text_[index];
//}
//
//string &cedit::get_index_line_text(uint16_t index) {
//	if (index < 0)
//		index = 0;
//	if (index > v_text_.size())
//		index = v_text_.size() - 1;
//	return v_text_[index];
//}
//
//string cedit::get_point_ch(c_point p) {
//	/*string &text = get_point_line_text(p);
//	int x_ch = p.x / _font_width;
//	for(auto &ch:text)*/
//	return "";
//}
//
//c_point cedit::get_end_line_spin_xy() {
//	int x = (*--v_text_.end()).size()*_font_width + off_margin_.x;
//	int y = (v_text_.size() - start_line_)*(_font_height + line_off) + line_off + off_margin_.x;
//	return { x,y };
//}
//
//string &cedit::get_end_line_text() {
//	return *--v_text_.end();
//}
//
//void cedit::insert_ch(char ch) {
//	string &text = get_point_line_text(spin_point);
//	int index_x = abs(scroll_xy.x)/_font_width+(spin_point.x - off_margin_.x) / _font_width;
//	if (index_x > text.size())
//		index_x = text.size() - 1;
//	text.insert(text.begin() + index_x , ch);
//	spin_point.x += _font_width;
//}
//
//void cedit::delete_ch() {
//	string &text = get_point_line_text(spin_point);
//	int index_x = abs(scroll_xy.x) / _font_width + (spin_point.x - off_margin_.x) / _font_width;
//	if (index_x > text.size())
//		index_x = text.size() - 1;
//	if (index_x == 1) {
//		set_spin_x(-1);
//		return;
//	}
//	text.erase(text.begin() + index_x - 1);
//	spin_point.x -= _font_width;
//}
//
//void cedit::set_spin_x(int p) {
//	if (p < 0) {//spin
//		if (scroll_xy.x >= 0) { //to up line
//			int index = get_point_line_index(spin_point);
//			if (index == 0)     //first line
//				return;
//			int end_x = get_text_end_x(get_index_line_text(index - 1));
//			spin_point.y -= (_font_height + line_off);
//			if (end_x > text_rect_.width) {  //up line over right
//				scroll_xy.x = -(end_x - text_rect_.width);
//				spin_point.x = get_point_spin_xy({ text_rect_.width,0 }).x;
//			}
//			else
//				spin_point.x = end_x;
//		}
//		else
//			scroll_xy.x += _font_width;
//	}
//	else if (p > _width) {
//		int end_x = get_text_end_x(get_point_line_text(spin_point));
//		if (text_rect_.width - scroll_xy.x > end_x) {
//			scroll_xy.x = 0;
//			spin_point.y += (_font_height + line_off);
//			spin_point.x = off_margin_.x;
//		}
//		else
//			scroll_xy.x -= _font_width;
//	}
//	else
//		spin_point.x = p;
//}
//
//void cedit::set_spin_y(int p) {
//	int x = _width - scroll_xy.x;
//	int end_x = get_text_end_x(get_point_line_text(spin_point));
//	if (x > end_x) {
//		scroll_xy.x = 0;
//		spin_point.y += (_font_height + line_off);
//		spin_point.x = off_margin_.x;
//	}
//
//	if (p < _width)
//		spin_point.x = p;
//	else if (x < end_x) {
//		scroll_xy.x -= _font_width;
//	}
//}
//
//void cedit::drag(c_point p) {
//	//drag_end_point_ += p;
//	scroll_x(spin_point.x + p.x);
//	if (drag_end_point_.y > _height)
//		scroll_y(-(_font_height + line_off));
//	else if (drag_end_point_.y < 0) {
//		scroll_y(_font_height + line_off);
//	}
//	else
//		drag_end_point_ += p;
//	update();
//}
//
//void cedit::mouse_move(c_point p) {
//	//drag_end_point_ = p;
//}
//
//void cedit::select_all() {
//	drag_start_point_ = { 0,0 };
//	drag_end_point_ = { _width,_height };
//	update();
//}
//
//uint16_t cedit:: get_max_line() {
//	return _height / (_font_height + line_off);
//}
//
//void cedit::change_font_height(int height) {
//	set_font_height(height);
//	max_line_ = get_max_line();
//}
//
//void cedit::change_start_line(int move) {
//	auto t_new_line = start_line_ + move;
//	if (move > 0) {
//		if (t_new_line < start_line_)
//			return;
//	}
//	else if (move < 0) {
//		if (t_new_line > start_line_)
//			return;
//	}
//	start_line_ = t_new_line; 
//}

bool cedit::update(bool redraw)
{
	erase_bk();
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	draw_select();
	if (style_ == T_singleline_edit)
		_gdi.draw_text(text_, off_margin_, _font_height);
	else {
		int len = start_line_ + max_line_;
		if (v_text_.size() - start_line_ < max_line_)
			len = v_text_.size() - start_line_;


		//c_point start_point = get_point_spin_xy(drag_start_point_);
		//c_point end_point = get_point_spin_xy(drag_end_point_);
		//c_point sp = get_spin_real_point(start_point);
		//c_point ep = get_spin_real_point(end_point);

		for (int i = start_line_; i < len; i++) {
			draw_line(i);
		}
	}

	return cwbase::update(redraw);
}

void cedit::draw_line(int line) {
	int ypos = (line-start_line_)*line_height_;
	int xpos = off_margin_.x;
	if (old_spin_point.x > text_rect_.width)
		xpos += text_rect_.width - old_spin_point.x;
	if (line == spin_point.y)
		_gdi.draw_frame_rect({ text_rect_.p.x ,ypos }, { text_rect_.p.x + text_rect_.width,ypos + line_height_ }, 2, RGB(70, 70, 70));
	_gdi.draw_text_ex(v_text_[line], { xpos,ypos }, _font_height);
}

void cedit::draw_select() {
	if (drag_start_point_ == drag_end_point_)
		return;

	//int begin_index = get_point_line_index(drag_start_point_);
	//int end_index = get_point_line_index(drag_end_point_);
	//if (begin_index > end_index)
	//	swap(begin_index, end_index);
	c_point start_point = get_point_spin_xy(drag_start_point_);
	c_point end_point = get_point_spin_xy(drag_end_point_);

	int begin_index = start_point.y;
	int end_index = end_point.y;

	start_point = get_spin_real_point(start_point);
	end_point = get_spin_real_point(end_point);
	spin_point = end_point;

	if (start_point.y > end_point.y)
		swap(start_point, end_point);

	if (begin_index == end_index)
		_gdi.fill_rect(start_point, { end_point.x,end_point.y + _font_height + line_off }, RGB(0, 144, 244));
	else {
		for (int index = begin_index; index <= end_index; index++) {
			if (index == end_index)
				_gdi.fill_rect(start_point, { end_point.x,start_point.y + line_height_ }, RGB(0, 144, 244));
			else {
				int end_x = get_line_width(index)*_font_width;// (get_index_line_text(index));
				_gdi.fill_rect(start_point, { end_x,start_point.y + line_height_ }, RGB(0, 144, 244));
			}
			start_point.x = off_margin_.x;
			start_point.y += line_height_;
		}
	}
}

void cedit::click_in(c_point p) {
	//ctimer::instance().add_timer(this, 500, &cedit::test);

	spin_point = get_point_spin_xy(p);

	//OutputDebugString(to_string(get_spin_ch_pos_in_text()).c_str());

	drag_start_point_ = p;
	drag_end_point_ = p;
	set_timer(this, 500, &cedit::test);
	update();
}

void cedit::click_out(c_point p) {
	//ctimer::instance().kill_timer(this, &cedit::test);
	kill_timer(this, &cedit::test);
	is_spin = false;
}

bool cedit::init() {
	if (style_ == T_singleline_edit) {
		_font_height = _height - 4;
	}

	scroll_ = new cscroll();
	scroll_->create({ _width - 12,0 }, 12, _height, this);
	scroll_->set_bk_color(RGB(62, 62, 62));
	scroll_->set_bar_color(RGB(104, 104, 104));
	scroll_->set_bar_drag_color(RGB(200, 200, 200));
	scroll_->add_cmd(this, T_scroll_event, &cedit::on_scroll);

	text_rect_ = { off_margin_,_width - scroll_->get_width(),_height };
	line_height_ = _font_height + line_off;
	max_line_ = text_rect_.height / line_height_;

	ifstream myfile("cframe.h");
	string temp;
	myfile.is_open();
	while (getline(myfile, temp))
	{
		//parase_tab(temp);
		v_text_.push_back(temp);
	}
	myfile.close();

	return cwbase::init();
}

void cedit::double_click(c_point p) {

}

void cedit::mouse_move_in(c_point p) {
	_active_color = _mouse_in_color;
}

void cedit::focus() {
	test();
}

void cedit::mouse_move_out(c_point p) {
	_active_color = _bk_color;
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
		
		_gdi.draw_line(old_spin_point, { old_spin_point.x , old_spin_point.y+line_height_ }, 1, _bk_color);
		auto point = get_spin_real_point(spin_point);
		_gdi.draw_line(point, { point.x ,point.y+line_height_ }, 1, rgb);
		old_spin_point = point;
	}
	update_parent();
}

void cedit::input_key(c_key key) {
	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('A') & 0x8000)) {
		select_all();
		goto update;
	}
	if (key.wVirtualKeyCode == VK_BACK) {
		delete_ch();
		//if (text_.size() > 0) {
		//	if ((*(text_.end()-1)) < 0) {   //ºº×ÖË«×Ö½Ú±àÂë
		//		text_.pop_back();
		//		spin_x_ -= _font_height / 2;
		//	}
		//	text_.pop_back();
		//	spin_x_ -= _font_height / 2;
		//} 
		goto update;
	}
	else if (key.wVirtualKeyCode == VK_RIGHT) {
		///set_spin_x(spin_point.x + _font_width);
		spin_right();
	}
	else if (key.wVirtualKeyCode == VK_LEFT) {
		///set_spin_x(spin_point.x - _font_width);
		spin_left();
	}
	else if (key.wVirtualKeyCode == VK_UP) {
		//spin_point.y -= (_font_height + line_off);
		spin_up();
	}
	else if (key.wVirtualKeyCode == VK_DOWN) {
		//spin_point.y += (_font_height + line_off);
		spin_down();
	}
	if (key.uChar.AsciiChar != 0) {
		insert_ch(key.uChar.AsciiChar);
	}

update:
	update();
}

void cedit::scroll_x(int dis) {
	//if (dis < 0) {//spin
	//	if (scroll_xy.x >= 0) { //to up line
	//		int index = get_point_line_index(spin_point);
	//		if (index == 0)     //first line
	//			return;
	//		int end_x = get_text_end_x(get_index_line_text(index - 1));
	//		if (end_x > text_rect_.width) {  //up line over right
	//			spin_point.x = get_point_spin_xy({ text_rect_.width,0 }).x;
	//		}
	//		else
	//			spin_point.x = end_x;
	//	}
	//	else {
	//		scroll_xy.x += _font_width;
	//		drag_start_point_.x += _font_width;
	//	}
	//}
	//else if (dis > _width) {
	//	int end_x = get_text_end_x(get_point_line_text(spin_point));
	//	if (text_rect_.width - scroll_xy.x > end_x) {
	//		spin_point.x = off_margin_.x;
	//	}
	//	else {
	//		scroll_xy.x -= _font_width;
	//		drag_start_point_.x -= _font_width;
	//	}
	//}
	//else
	//	spin_point.x = dis;
}

void cedit::scroll_y(int dis) {
	scroll_xy.y += dis;
	if (scroll_xy.y >= 0)
		scroll_xy.y = 0;
	if (start_line_ != 0) {
		drag_end_point_.y += dis;
		drag_start_point_.y += dis;
	}
	change_start_line((-dis) / (line_height_));
}

void cedit::on_scroll(const void *data) {
	int move = *(int*)data;
	//scroll_xy.y += (-move);
	update();
}

void cedit::mouse_wheeled(bool up) {
	if (up) {
		scroll_->scroll(-(scroll_->get_height()*(_font_height + line_off) / _height));
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
			change_font_height(get_font_size() + 2);
		else
			scroll_y(_font_height + line_off);
		//scroll_xy.y += (_font_height + line_off);
	}
	else {
		scroll_->scroll(scroll_->get_height()*(_font_height + line_off) / _height);
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
			change_font_height(get_font_size() - 2);
		else
			scroll_y(-(_font_height + line_off));
		//scroll_xy.y -= _font_height + line_off;
	}
	update();
}

void cedit::parase_text(string &text) {
	size_t pre_pos = 0;
	size_t old_pos = -1;
	while (1) {
		pre_pos = text.find_first_of('\r', old_pos + 1);
		if (pre_pos == -1) {
			v_text_.push_back(text.substr(old_pos + 1, text_.size() - old_pos - 1));
			break;
		}
		else {
			v_text_.push_back(text.substr(old_pos + 1, pre_pos - old_pos - 1));
			old_pos = pre_pos;
		}
	}
}

void cedit::parase_tab(string &text) {
	size_t pre_pos = 0;
	size_t old_pos = -1;
	while (1) {
		pre_pos = text.find_first_of('\t', old_pos + 1);
		if (pre_pos == -1) {
			//v_text_.push_back(text.substr(old_pos + 1, text_.size() - old_pos - 1));
			break;
		}
		else {
			text.insert(text.begin() + pre_pos, ' ');
			text.insert(text.begin() + pre_pos, ' ');
			text.insert(text.begin() + pre_pos, ' ');
			old_pos = pre_pos + 3;
		}
	}
}

c_point cedit::get_point_spin_xy(c_point p) {
	spin_info info = get_point_spin_info(p);
	return { info.in_line_pos,info.line };
}


string &cedit::get_line_text(uint16_t index) {
	if (index < 0)
		index = 0;
	if (index > v_text_.size())
		index = v_text_.size() - 1;
	return v_text_[index];
}

string &cedit::get_end_line_text() {
	return *--v_text_.end();
}

void cedit::insert_ch(char ch) {
	string &text = get_line_text(spin_point.y);
	text.insert(text.begin() + get_spin_ch_pos_in_text() + 1, ch);
	if (ch == '\t')
		spin_right(TAB_WIDTH);
	else
		spin_right();
}

void cedit::delete_ch() {
	string &text = get_line_text(spin_point.y);
	if (spin_point.x == 0)
		spin_up_end();
	else {
		spin_info info = get_point_spin_info(spin_point, false);
		if (info.in_line_ch == '\t')
			spin_left(TAB_WIDTH);
		else
		    spin_left();
		
		text.erase(text.begin() + info.in_line_index);
	}
}

int cedit::get_spin_ch_pos() {
	vector<string> v_str;
	string &text = get_line_text(spin_point.y);
	split_with_ch(text,'\t',v_str);
	int start = 0;
	for (auto &s : v_str) {
		if (s == "\t") {
			start += TAB_WIDTH;
			if (start >= spin_point.x)
				return start;
			
		}
		else {
			start += s.size();
			if (start >= spin_point.x) 
				return spin_point.x;
		}
	}
	return start;
}

/*get the index of spin in the text*/
int cedit::get_spin_ch_pos_in_text() {
	vector<string> v_str;
	string &text = get_line_text(spin_point.y);
	split_with_ch(text, '\t', v_str);
	int start = 0;
	int index = 0;
	for (auto &s : v_str) {
		if (s == "\t") {
			start += TAB_WIDTH;
			if (start >= spin_point.x)
				return index;
			index++;
		}
		else {
			
			if (start+ s.size() >= spin_point.x)
				return index + spin_point.x - start -1;
			start += s.size();
			index += s.size();
		}
	}
	return index;
}

void cedit::drag(c_point p) {
	
	update();
}

void cedit::mouse_move(c_point p) {
	drag_end_point_ = p;
}

void cedit::select_all() {
	drag_start_point_ = { 0,0 };
	drag_end_point_ = { _width,_height };
	update();
}

uint16_t cedit::get_max_line() {
	return _height / (_font_height + line_off);
}

void cedit::change_font_height(int height) {
	set_font_height(height);
	line_height_ = height + line_off;
	//max_line_ = get_max_line();
}

void cedit::change_start_line(int move) {
	auto t_new_line = start_line_ + move;
	if (move > 0) {
		if (t_new_line < start_line_)
			return;
	}
	else if (move < 0) {
		if (t_new_line > start_line_)
			return;
	}
	start_line_ = t_new_line;
}

int cedit::get_line_width(int line) {
	vector<string> v_str;
	string &text = get_line_text(line);
	split_with_ch(text, '\t', v_str);
	int start = 0;
	for (auto &s : v_str) {
		if (s == "\t") 
			start += TAB_WIDTH;
		else
			start += s.size();
	}
	return start;
}

void cedit::spin_up_end() {
	if (spin_point.y >= 0) {
		spin_point.y -= 1;
		spin_point.x = get_line_width(spin_point.y);// v_text_[spin_point.y].size();
	}
}

void cedit::spin_down_begin() {
	if (spin_point.y < v_text_.size()) {
		spin_point.y += 1;
		spin_point.x = 0;
	}
}

void cedit::spin_left(int step /*= 1*/) {
	if (spin_point.x > 0)
		spin_point.x -= get_next_spin_step_x(true);
	else
		spin_up_end();
	is_spin = true;
}

void cedit::spin_right(int step /*= 1*/) {
	if (spin_point.x < get_line_width(spin_point.y))
		spin_point.x += get_next_spin_step_x();
	else
		spin_down_begin();
	is_spin = true;
}

c_point cedit::get_spin_real_point(c_point p) {
	int x = p.x*_font_width + off_margin_.x;
	int y = (p.y - (long)start_line_)*line_height_ + off_margin_.y;
	return { x,y };
}

void cedit::spin_up(int step /*= 1*/) {
	if (is_spin_top_line())
		change_start_line(-1);
	if (spin_point.y > 0)
		spin_point.y -= step;
	is_spin = true;
}

void cedit::spin_down(int step /*= 1*/) {
	if (spin_point.y < v_text_.size())
		spin_point.y += step;
	if (is_spin_bottom_line())
		change_start_line(1);
	is_spin = true;
}

bool cedit::is_spin_top_line() {
	if (start_line_ == spin_point.y)
		return true;
	return false;
}

int cedit::get_next_spin_step_x(bool is_left) {
	//spin_info info = get_point_spin_info(spin_point, false);
	//string &text = get_line_text(spin_point.y);
	//if (is_left) {

	//}
	vector<string> v_str;
	string &text = get_line_text(spin_point.y);
	split_with_ch(text, '\t', v_str);
	int start = 0;
	int next = is_left > 0 ? spin_point.x - 1 : spin_point.x + 1;
	for (auto &s : v_str) {
		if (s == "\t") {
			start += TAB_WIDTH;
			if (start > next)
				return TAB_WIDTH;
		}
		else {
			start += s.size();
			if (start >= next)
				return 1;
		}
	}
	return 0;
}

bool cedit::is_spin_bottom_line() {
	if (max_line_ == spin_point.y - start_line_)
		return true;
	return false;
}

int cedit::get_point_line(int y) {
	return start_line_ + (y - off_margin_.y) / line_height_;
}

int cedit::get_point_line_pos(int x) {
	return (x - off_margin_.x) / _font_width;
}

spin_info cedit::get_point_spin_info(c_point p, bool is_screen_point) {
	spin_info info;
	int x;
	if (is_screen_point) {
		info.line = get_point_line(p.y);
		x = get_point_line_pos(p.x);;
	}
	else {
		info.line = p.y;
		x = p.x;
	}

	vector<string> v_str;
	string &text = get_line_text(info.line);
	if (text.size() == 0)
		return info;


	split_with_ch(text, '\t', v_str);
	int start = 0;
	int index = 0;
	
	for (auto &s : v_str) {
		if (s == "\t") {
			start += TAB_WIDTH;
			if (start >= x) {
				info.in_line_pos = start;
				info.in_line_index = index;
				info.in_line_ch = text[info.in_line_index];
				if (info.in_line_index >= 1)
					info.pre_ch = text[info.in_line_index - 1];
				info.pre_ch = text[info.in_line_index + 1];
				return info;
			}
			index++;
		}
		else {
			if (start + s.size() >= x) {
				info.in_line_index = index + x - start - 1;
				info.in_line_pos = x;
				info.in_line_ch = text[info.in_line_index];
				if (info.in_line_index >= 1)
					info.pre_ch = text[info.in_line_index - 1];
				info.pre_ch = text[info.in_line_index + 1];
				return info;
			}
			start += s.size();
			index += s.size();
		}
	}
	info.in_line_index = index;
	info.in_line_pos = start;
	return info;
}

string cedit::get_GB(string &str, int index) {
	if (str.size() > index && str[index] > 0)
		return "";

	if (index <= str.size() / 2) {
		for (int i = index; i >= 0; i--) {
			if (str[i] > 0) {
				if ((index - i) % 2 == 0)
					return str.substr(index - 1,2);
				return str.substr(index, 2);
			}
		}
		return str.substr(index, 2);
	}
	else {
		for (int i = index; i <str.size(); i++) {
			if (str[i] > 0) {
				if ((index - i) % 2 != 0)
					return str.substr(index - 1, 2);
				return str.substr(index, 2);
			}
		}
		return str.substr(index - 1, 2);
	}
}
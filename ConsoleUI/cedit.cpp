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

bool cedit::update(bool redraw)
{
	erase_bk();
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	draw_select();
	if (style_ == T_singleline_edit)
		_gdi.draw_text(text_, off_margin_, _font_height);
	else {
		int ypos = off_margin_.y + line_off;
		int xpos = scroll_xy.x + off_margin_.x ;

		int len = start_line_ + max_line_;
		for (int i = start_line_; i < len; i++) {
			_gdi.draw_text_t(v_text_[i], { xpos,ypos }, _font_height);
			ypos += _font_height + line_off;
		}
	}

	return cwbase::update(redraw);
}

void cedit::draw_select() {
	if (drag_start_point_ == drag_end_point_)
		return;
	
	int begin_index = get_point_line_index(drag_start_point_);
	int end_index = get_point_line_index(drag_end_point_);
	if (begin_index > end_index)
		swap(begin_index, end_index);
	c_point start_point = get_point_spin_xy(drag_start_point_);
	c_point end_point = get_point_spin_xy(drag_end_point_);
	spin_point = end_point;

	if (start_point.y > end_point.y)
		swap(start_point, end_point);

	if (begin_index == end_index)
		_gdi.fill_rect(start_point, { end_point.x,end_point.y + _font_height + line_off }, RGB(0, 144, 244));
	else {
		for (int index = begin_index; index <= end_index; index++) {
			if (index == end_index)
				_gdi.fill_rect(start_point, { end_point.x,start_point.y + _font_height + line_off }, RGB(0, 144, 244));
			else {
				int end_x = get_text_end_x(get_index_line_text(index));
				_gdi.fill_rect(start_point, { end_x,start_point.y + _font_height + line_off }, RGB(0, 144, 244));
			}
			start_point.x = off_margin_.x;
			start_point.y += _font_height + line_off;
		}
	}
}

void cedit::click_in(c_point p) {
	//ctimer::instance().add_timer(this, 500, &cedit::test);
	
	spin_point = get_point_spin_xy(p);
	drag_start_point_ = p;
	drag_end_point_ = p;
	set_timer(this,500, &cedit::test);
	update();
}

void cedit::click_out(c_point p) {
	//ctimer::instance().kill_timer(this, &cedit::test);
	kill_timer(this,&cedit::test);
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

	max_line_ = get_max_line();
	text_rect_ = {off_margin_,_width-scroll_->get_width(),_height};

	ifstream myfile("log_2018-10-30.txt");
	string temp;
	myfile.is_open();
	while (getline(myfile, temp))
	{	
		parase_tab(temp);
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
		_gdi.draw_line(old_spin_point, { old_spin_point.x ,_font_height + old_spin_point.y }, 1, _bk_color);
		_gdi.draw_line(spin_point, { spin_point.x ,_font_height + spin_point.y }, 1, rgb);
		old_spin_point = spin_point;
	}
	update_parent();
}

void cedit::input_key(c_key key) {
	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('A') & 0x8000)) {
		select_all();
		goto update;
	}
	if (key.wVirtualKeyCode == VK_BACK) {
		if (text_.size() > 0) {
			if ((*(text_.end()-1)) < 0) {   //ºº×ÖË«×Ö½Ú±àÂë
				text_.pop_back();
				spin_x_ -= _font_height / 2;
			}
			text_.pop_back();
			spin_x_ -= _font_height / 2;
		} 
		goto update;
	}
	else if (key.wVirtualKeyCode == VK_RIGHT) {
		set_spin_x( spin_point.x + _font_width );
	}
	else if (key.wVirtualKeyCode == VK_LEFT) {
		set_spin_x(spin_point.x - _font_width);
	}
	else if (key.wVirtualKeyCode == VK_UP) {
		spin_point.y -= (_font_height + line_off);
	}
	else if (key.wVirtualKeyCode == VK_DOWN) {
		spin_point.y += (_font_height + line_off);
	}
	if (key.uChar.AsciiChar!=0) {
		insert_ch(key.uChar.AsciiChar);
	}
		
	update:
	update();
}

void cedit::scroll_x(int dis) {
	if (dis < 0) {//spin
		if (scroll_xy.x >= 0) { //to up line
			int index = get_point_line_index(spin_point);
			if (index == 0)     //first line
				return;
			int end_x = get_text_end_x(get_index_line_text(index - 1));
			if (end_x > text_rect_.width) {  //up line over right
				spin_point.x = get_point_spin_xy({ text_rect_.width,0 }).x;
			}
			else
				spin_point.x = end_x;
		}
		else {
			scroll_xy.x += _font_width;
			drag_start_point_.x += _font_width;
		}
	}
	else if (dis > _width) {
		int end_x = get_text_end_x(get_point_line_text(spin_point));
		if (text_rect_.width - scroll_xy.x > end_x) {
			spin_point.x = off_margin_.x;
		}
		else {
			scroll_xy.x -= _font_width;
			drag_start_point_.x -= _font_width;
		}
	}
	else
		spin_point.x = dis;
}

void cedit::scroll_y(int dis) {
	scroll_xy.y += dis;
	if (scroll_xy.y >= 0)
		scroll_xy.y = 0;
	if (start_line_ != 0) {
		drag_end_point_.y += dis;
		drag_start_point_.y += dis;
	}
	change_start_line((-dis) / (_font_height + line_off));
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
	string &text = get_point_line_text(p);
	int x = get_text_end_x(text);
	if (p.x < x)
		x = p.x - (p.x - off_margin_.x) % _font_width;
	int y = get_end_line_y();
	if (p.y < y) 
		y = p.y - (p.y - off_margin_.y) % (_font_height + line_off) + line_off + off_margin_.y;
	return { x,y };
}

uint16_t cedit::get_text_end_x(string &text) {
	return text.size()*_font_width + off_margin_.x;
}

uint16_t cedit::get_end_line_y() {
	return (v_text_.size() - start_line_)*(_font_height + line_off) + line_off + off_margin_.y;
}

uint16_t cedit::get_point_line_index(c_point p) {
	uint16_t index= start_line_ + (p.y - off_margin_.y) / (_font_height + line_off);
	if (index > v_text_.size() - 1)
		return v_text_.size() - 1;
	return index;
}

string &cedit::get_point_line_text(c_point p) {
	uint16_t index = get_point_line_index(p);
	if (index > v_text_.size() - 1)
		index = v_text_.size() - 1;
	return v_text_[index];
}

string &cedit::get_index_line_text(uint16_t index) {
	if (index < 0)
		index = 0;
	if (index > v_text_.size())
		index = v_text_.size() - 1;
	return v_text_[index];
}

string cedit::get_point_ch(c_point p) {
	/*string &text = get_point_line_text(p);
	int x_ch = p.x / _font_width;
	for(auto &ch:text)*/
	return "";
}

c_point cedit::get_end_line_spin_xy() {
	int x = (*--v_text_.end()).size()*_font_width + off_margin_.x;
	int y = (v_text_.size() - start_line_)*(_font_height + line_off) + line_off + off_margin_.x;
	return { x,y };
}

string &cedit::get_end_line_text() {
	return *--v_text_.end();
}

void cedit::insert_ch(char ch) {
	string &text = get_point_line_text(spin_point);
	int index_x = abs(scroll_xy.x)/_font_width+(spin_point.x - off_margin_.x) / _font_width;
	if (index_x > text.size())
		index_x = text.size() - 1;
	text.insert(text.begin() + index_x , ch);
	spin_point.x += _font_width;
}

void cedit::set_spin_x(int p) {
	if (p < 0) {//spin
		if (scroll_xy.x >= 0) { //to up line
			int index = get_point_line_index(spin_point);
			if (index == 0)     //first line
				return;
			int end_x = get_text_end_x(get_index_line_text(index - 1));
			spin_point.y -= (_font_height + line_off);
			if (end_x > text_rect_.width) {  //up line over right
				scroll_xy.x = -(end_x - text_rect_.width);
				spin_point.x = get_point_spin_xy({ text_rect_.width,0 }).x;
			}
			else
				spin_point.x = end_x;
		}
		else
			scroll_xy.x += _font_width;
	}
	else if (p > _width) {
		int end_x = get_text_end_x(get_point_line_text(spin_point));
		if (text_rect_.width - scroll_xy.x > end_x) {
			scroll_xy.x = 0;
			spin_point.y += (_font_height + line_off);
			spin_point.x = off_margin_.x;
		}
		else
			scroll_xy.x -= _font_width;
	}
	else
		spin_point.x = p;
}

void cedit::set_spin_y(int p) {
	int x = _width - scroll_xy.x;
	int end_x = get_text_end_x(get_point_line_text(spin_point));
	if (x > end_x) {
		scroll_xy.x = 0;
		spin_point.y += (_font_height + line_off);
		spin_point.x = off_margin_.x;
	}

	if (p < _width)
		spin_point.x = p;
	else if (x < end_x) {
		scroll_xy.x -= _font_width;
	}
}

void cedit::drag(c_point p) {
	//drag_end_point_ += p;
	scroll_x(spin_point.x + p.x);
	if (drag_end_point_.y > _height)
		scroll_y(-(_font_height + line_off));
	else if (drag_end_point_.y < 0) {
		scroll_y(_font_height + line_off);
	}
	else
		drag_end_point_ += p;
	update();
}

void cedit::mouse_move(c_point p) {
	//drag_end_point_ = p;
}

void cedit::select_all() {
	drag_start_point_ = { 0,0 };
	drag_end_point_ = { _width,_height };
	update();
}

uint16_t cedit:: get_max_line() {
	return _height / (_font_height + line_off);
}

void cedit::change_font_height(int height) {
	set_font_height(height);
	max_line_ = get_max_line();
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
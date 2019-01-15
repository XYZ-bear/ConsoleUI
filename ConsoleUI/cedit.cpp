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
	if (is_select())
		draw_select();
	if (style_ == T_singleline_edit)
		_gdi.draw_text(text_, off_margin_, _font_height);
	else {
		int len = start_line_ + max_line_;
		if (v_text_.size() - start_line_ < max_line_)
			len = v_text_.size() - start_line_;

		for (int i = start_line_; i < len; i++) {
			draw_line(i);
		}
	}
	
	return cwbase::update(redraw);
}

void cedit::draw_line(int line) {
	int ypos = (line-start_line_)*line_height_;
	if (line == spin_point.y&&!is_select())
		_gdi.draw_frame_rect({ text_rect_.p.x ,ypos }, { text_rect_.p.x + text_rect_.width,ypos + line_height_ }, 2, RGB(70, 70, 70));
	_gdi.draw_text_ex(v_text_[line], { text_rect_.p.x,ypos }, _font_height);
	old_spin_point = spin_point;
}

void cedit::draw_select() {
	c_point start_point = drag_start_point_;
	c_point end_point = drag_end_point_;

	int begin_index = start_point.y;
	int end_index = end_point.y;

	start_point = get_spin_real_point(start_point);
	end_point = get_spin_real_point(end_point);

	if (start_point.y > end_point.y)
		swap(start_point, end_point);

	if (begin_index > end_index)
		swap(begin_index, end_index);

	//只显示可见区域
	if (begin_index == end_index)
		_gdi.fill_rect(start_point, { end_point.x,end_point.y + line_height_ }, select_color_);
	else {
		for (int index = begin_index; index <= end_index; index++) {
			//if (index > start_line_ + max_line_)
			//	break;
			//if (index < start_line_)
			//	continue;
			if (index == end_index)
				_gdi.fill_rect(start_point, { end_point.x,start_point.y + line_height_ }, select_color_);
			else {
				int end_x = get_line_width(index)*_font_width;
				_gdi.fill_rect(start_point, { end_x +text_rect_.p.x,start_point.y + line_height_ }, select_color_);
			}
			start_point.x = text_rect_.p.x;
			start_point.y += line_height_;
		}
	}
}

void cedit::click_in(c_point p) {
	spin_point = get_point_spin_xy(p);
	drag_start_point_ = spin_point;
	drag_end_point_ = spin_point;
	set_timer(this, 500, &cedit::test);
	update();
}

void cedit::click_out(c_point p) {
	kill_timer(this, &cedit::test);
	is_spin = false;
}

bool cedit::init() {
	if (style_ == T_singleline_edit) {
		_font_height = _height - 4;
	}

	int max_line_x = 0;
	ifstream myfile("def.h");
	string temp;
	myfile.is_open();
	while (getline(myfile, temp))
	{
		v_text_.push_back(temp);
		if (temp.length() > max_line_x)
			max_line_x = temp.length();
	}
	myfile.close();

	line_height_ = _font_height + line_off;
	
	scroll_ = new cscroll();
	scroll_->create({ _width - 12,0 }, 12, _height -12, this);
	scroll_->set_bk_color(RGB(62, 62, 62));
	scroll_->set_bar_color(RGB(104, 104, 104));
	scroll_->set_bar_drag_color(RGB(200, 200, 200));
	scroll_->set_range(0, v_text_.size()*line_height_);
	scroll_->add_cmd(this, T_scroll_event, &cedit::on_h_scroll);

	v_scroll_ = new cscroll();
	v_scroll_->create({ 0 ,_height - 12 }, _width, 12, this);
	v_scroll_->set_style(T_v_scroll);
	v_scroll_->set_bk_color(RGB(62, 62, 62));
	v_scroll_->set_bar_color(RGB(104, 104, 104));
	v_scroll_->set_bar_drag_color(RGB(200, 200, 200));
	v_scroll_->set_range(0, max_line_x*_font_width);
	v_scroll_->add_cmd(this, T_scroll_event, &cedit::on_v_scroll);


	text_rect_ = { off_margin_,_width - scroll_->get_width(),_height-v_scroll_->get_height() };
	max_line_ = text_rect_.height / line_height_;

	return cwbase::init();
}

void cedit::double_click(c_point p) {
	auto info = get_point_spin_info(p);
	int prei = info.in_line_index;
	int nexti = info.in_line_index;
	string &text = v_text_[info.line];
	int line_len = text.size();
	while (1) {
		if (text[prei] != ' '&&prei > 0)
			prei--;
		if (text[nexti] != ' '&&nexti < line_len )
			nexti++;
		if (prei == 0 && nexti == line_len)
			break;
		else if (text[prei] == ' '&&text[nexti] == ' ') {
			prei++;
			//nexti--;
			break;
		}
		else if (prei == 0 && text[nexti] == ' ') {
			//nexti--;
			break;
		}
		else if (text[prei] == ' '&&nexti == line_len) {
			prei++;
			break;
		}
	}
	drag_start_point_.x = index_to_pos(prei);
	drag_end_point_.x = index_to_pos(nexti);
	update();
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
		if (is_select())
			delete_select();
		else
			delete_ch();
		goto update;
	}
	else if (key.wVirtualKeyCode == VK_RIGHT) {
		spin_right();
	}
	else if (key.wVirtualKeyCode == VK_LEFT) {
		spin_left();
	}
	else if (key.wVirtualKeyCode == VK_UP) {
		spin_up();
	}
	else if (key.wVirtualKeyCode == VK_DOWN) {
		spin_down();
	}
	else if (key.wVirtualKeyCode == VK_RETURN) {
		add_new_line();
		goto update;
	}
	else {
		if (key.uChar.AsciiChar != 0) {
			insert_ch(key.uChar.AsciiChar);
		}
	}

update:
	update();
}

void cedit::on_h_scroll(const void *data) {
	scroll_info &info = *(scroll_info*)data;
	change_start_line(info.pos / line_height_ - start_line_);
	drag_end_point_.y += ((info.pos / line_height_ - start_line_)*line_height_);
	drag_start_point_.y += ((info.pos / line_height_ - start_line_)*line_height_);
	update();
}

void cedit::on_v_scroll(const void *data) {
	scroll_info &info = *(scroll_info*)data;
	text_rect_.p.x -= info.move;
	update();
}

void cedit::mouse_wheeled(bool up) {
	if (up) {
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000)) 
			change_font_height(get_font_size() + 2);
		else
			scroll_->scroll(-1);
	}
	else {
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
			change_font_height(get_font_size() - 2);
		else
			scroll_->scroll(1);
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
	text.insert(text.begin() + get_point_spin_info(spin_point,false).in_line_index, ch);
	if (ch == '\t')
		spin_right(TAB_WIDTH);
	else
		spin_right();
}

void cedit::delete_ch() {
	string &text = get_line_text(spin_point.y);
	if (spin_point.x == 0) {
		int line = spin_point.y;
		spin_up_end();
		if (text.size() != 0 && line >= 1)
			get_line_text(line - 1) += text;
		v_text_.erase(v_text_.begin() + line);
	}
	else {
		spin_info info = get_point_spin_info(spin_point, false);
		if (info.in_line_ch == '\t')
			spin_left(TAB_WIDTH);
		else
		    spin_left();
		text.erase(text.begin() + info.in_line_index - 1);
	}
}

void cedit::delete_select() {
	c_point start_point = drag_start_point_;
	c_point end_point = drag_end_point_;

	if (start_point.y > end_point.y)
		swap(start_point, end_point);

	spin_info binfo = get_point_spin_info(start_point, false);
	spin_info einfo = get_point_spin_info(end_point, false);

	if (start_point.y == end_point.y) {
		string &text = v_text_[start_point.y];
		text.erase(text.begin() + binfo.in_line_index, text.begin() + einfo.in_line_index);
	}
	else {
		string &btext = v_text_[start_point.y];
		btext.erase(btext.begin() + binfo.in_line_index, btext.end());
		string &etext = v_text_[end_point.y];
		etext.erase(etext.begin(), etext.begin() + einfo.in_line_index);
		btext += etext;
		v_text_.erase(v_text_.begin() + start_point.y + 1, v_text_.begin() + end_point.y + 1);
	}
	spin_point = start_point;
	drag_end_point_ = spin_point;
	drag_start_point_ = spin_point;
}

void cedit::add_new_line() {
	spin_info binfo = get_point_spin_info(spin_point, false);
	string &text=get_line_text(binfo.line);
	string spit=text.substr(binfo.in_line_index, text.size() - binfo.in_line_index );
	text.erase(text.begin()+binfo.in_line_index,text.end());
	v_text_.insert(v_text_.begin() + binfo.line + 1, spit);
	spin_down_begin();
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

/*get the real pos from the index*/
int cedit::index_to_pos(int index) {
	vector<string> v_str;
	string &text = get_line_text(spin_point.y);
	int start = 0;
	int tindex = 0;
	for (auto &s : text) {
		if (index == tindex)
			return start;
		if (s == '\t')
			start += TAB_WIDTH;
		else
			start += 1;
		tindex++;
	}
	return start;
}

void cedit::drag(drag_info p) {
	drag_end_point_= get_point_spin_xy(p.cur_point);
	spin_point = drag_end_point_;
	if (p.cur_point.y<text_rect_.p.y) {
		scroll_->scroll(-1);
	}
	else if (p.cur_point.y > text_rect_.height) {
		scroll_->scroll(1);
	}
	update();
}

void cedit::mouse_move(c_point p) {
}

void cedit::select_all() {
	drag_start_point_ = { 0,0 };
	drag_end_point_ = { get_line_width(v_text_.size() - 1),(int)v_text_.size() - 1 };
	update();
}

uint16_t cedit::get_max_line() {
	return text_rect_.height / line_height_;
}

void cedit::change_font_height(int height) {
	set_font_height(height);
	line_height_ = height + line_off;
	max_line_ = get_max_line();
	scroll_->set_range(0, v_text_.size()*line_height_);
	scroll_->update();
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
		spin_point.x = get_line_width(spin_point.y);
	}
}

void cedit::spin_down_begin() {
	if (spin_point.y < v_text_.size()) {
		spin_point.y += 1;
		spin_point.x = 0;
	}
}

void cedit::spin_left(int step /*= 1*/) {
	if (spin_point.x > 0) {
		int t = get_next_spin_step_x(true);
		if((spin_point.x - t)*_font_width + text_rect_.p.x < 0)
			v_scroll_->scroll(-_font_width);
		spin_point.x -= t;
	}
	else
		spin_up_end();
	is_spin = true;
}

void cedit::spin_right(int step /*= 1*/) {
	if (spin_point.x < get_line_width(spin_point.y)) {
		int t = get_next_spin_step_x();
		if ((spin_point.x + t)*_font_width + text_rect_.p.x > text_rect_.width)
			v_scroll_->scroll(_font_width);
		spin_point.x += t;
	}
	else
		spin_down_begin();
	is_spin = true;
}

c_point cedit::get_spin_real_point(c_point p) {
	int x = p.x*_font_width + text_rect_.p.x;
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

bool cedit::is_spin_bottom_line() {
	if (max_line_ == spin_point.y - start_line_)
		return true;
	return false;
}

bool cedit::is_select() {
	return drag_start_point_ != drag_end_point_;
}

int cedit::get_next_spin_step_x(bool is_left) {
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

/*y->line*/
int cedit::get_point_line(int y) {
	return start_line_ + (y - off_margin_.y) / line_height_;
}

/*x->index*/
int cedit::get_point_line_pos(int x) {
	return (x - off_margin_.x) / _font_width;
}

/*mirror the screen point to the spin point*/
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
			if (start+ TAB_WIDTH > x) {
				info.in_line_pos = start;
				info.in_line_index = index;
				if (info.in_line_index >= 0)
					info.in_line_ch = text[info.in_line_index];
				if (info.in_line_index >= 1)
					info.pre_ch = text[info.in_line_index - 1];
				return info;
			}
			start += TAB_WIDTH;
			index++;
		}
		else {
			if (start + s.size() >= x) {
				info.in_line_index = index + x - start;
				info.in_line_pos = x;
				if (info.in_line_index >= 0)
					info.in_line_ch = text[info.in_line_index];
				if (info.in_line_index >= 1)
					info.pre_ch = text[info.in_line_index - 1];
				//info.pre_ch = text[info.in_line_index + 1];
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

/*get the index of the chinese(2 byte)*/
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
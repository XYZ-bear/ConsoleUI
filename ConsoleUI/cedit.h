#pragma once
#include "cwbase.h"
#include "cscroll.h"
#include <vector>

enum T_edit_style
{
	T_multiline_edit,
	T_singleline_edit
};

class cedit :
	public cwbase
{
public:
	cedit();
	~cedit();

	bool update(bool redraw=true);
	bool init();
	void click_in(c_point p);
	void click_out(c_point p);
	void double_click(c_point p);
	void input_key(c_key key);
	void test();
	void set_style(T_edit_style style) { style_ = style; };
	void set_spin_color(COLORREF color) { spin_color_ = color; };
	T_edit_style get_style() { return style_; };

	void on_h_scroll(const void *data);
	void on_v_scroll(const void *data);
	void draw_line(int line);
	void parase_text(string &text);
	c_point get_point_spin_xy(c_point p);
	string &get_end_line_text();
	string &get_line_text(uint16_t index);
	int get_spin_ch_pos();
	void insert_ch(char ch);
	void delete_ch();
	void delete_select();
	void add_new_line();

	void draw_select();
	void select_all();

	uint16_t get_max_line();
	void change_font_height(int height);
	void change_start_line(int move);
	void spin_up_end();
	void spin_down_begin();
	void spin_left(int step = 1);
	void spin_right(int step = 1);
	void spin_up(int step = 1);
	void spin_down(int step = 1);
	int get_next_spin_step_x(bool is_left = false);
	int get_line_width(int line);
	int index_to_pos(int index);
	bool is_spin_bottom_line();
	bool is_spin_top_line();
	bool is_select();
	c_point get_spin_real_point(c_point p);
	int get_point_line(int y);
	int get_point_line_pos(int x);
	spin_info get_point_spin_info(c_point p, bool is_screen_point = true);
	string get_GB(string &str,int index);
private:
	bool is_spin;
	int spin_x_;
	int spin_y_;
	int spin_x_off_;
	int spin_y_off_;
	c_point scroll_xy{ 0,0 };
	c_color spin_color_ = RGB(255, 255, 255);
	c_color select_color_ = RGB(0, 144, 244);
	string text_;
	T_edit_style style_= T_singleline_edit;
	cscroll *scroll_ = nullptr;
	cscroll *v_scroll_ = nullptr;
	vector<string> v_text_;
	uint32_t start_line_ = 0;
	uint16_t max_line_;
	uint8_t line_off = 1;
	uint8_t line_height_;
	c_point spin_point{ 0,0 };
	c_point old_spin_point{ 0,0 };
	c_point off_margin_{ 2,2 };
	c_rect text_rect_{ 0,0 };

	c_point drag_start_point_{ 0,0 };
	c_point drag_end_point_{ 0,0 };
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
	void focus();
	void mouse_wheeled(bool up);
	void drag(drag_info p);
	void mouse_move(c_point p);
};


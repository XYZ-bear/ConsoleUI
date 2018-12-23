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

	bool update(bool redraw=false);
	bool init();
	void click_in(c_point p);
	void click_out(c_point p);
	void double_click(c_point p);
	void input_key(c_key key);
	void test();
	void set_style(T_edit_style style) { style_ = style; };
	void set_spin_color(COLORREF color) { spin_color_ = color; };
	T_edit_style get_style() { return style_; };

	void on_scroll(const void *data);

	void parase_text(string &text);
	void parase_tab(string &text);
	c_point get_point_spin_xy(c_point p);
	c_point get_end_line_spin_xy();
	string &get_end_line_text();
	uint16_t get_point_line_index(c_point p);
	string &get_point_line_text(c_point p);
	string &get_index_line_text(uint16_t index);
	string get_point_ch(c_point p);
	uint16_t get_text_end_x(string &text);
	uint16_t get_end_line_y();
	void insert_ch(char ch);
	void set_spin_x(int p);
	void set_spin_y(int p);
	void scroll_x(int dis);
	void scroll_y(int dis);

	void draw_select();
	void select_all();
private:
	bool is_spin;
	int spin_x_;
	int spin_y_;
	int spin_x_off_;
	int spin_y_off_;
	c_point scroll_xy{ 0,0 };
	COLORREF spin_color_ = RGB(255, 255, 255);
	string text_;
	T_edit_style style_= T_singleline_edit;
	cscroll *scroll_ = nullptr;
	vector<string> v_text_;
	uint16_t start_line_ = 0;
	uint16_t max_line_;
	uint8_t line_off = 1;
	c_point spin_point{ 0,0 };
	c_point old_spin_point;
	c_point off_margin_{ 2,2 };
	c_rect text_rect_{ 0,0 };

	c_point drag_start_point_{ 0,0 };
	c_point drag_end_point_{ 0,0 };
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
	void focus();
	void mouse_wheeled(bool up);
	void drag(c_point p);
	void mouse_move(c_point p);
};


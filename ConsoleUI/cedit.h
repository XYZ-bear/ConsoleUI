#pragma once
#include "cwbase.h"

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

	bool update();
	bool init();
	void click_in(c_point p);
	void click_out(c_point p);
	void double_click(c_point p);
	void input_key(c_key key);
	void test();
	void set_style(T_edit_style style) { style_ = style; };
	void set_spin_color(COLORREF color) { spin_color_ = color; };
	T_edit_style get_style() { return style_; };
private:
	bool is_spin;
	int spin_x_;
	int spin_y_;
	int spin_x_off_;
	int spin_y_off_;
	COLORREF spin_color_ = RGB(255, 255, 255);
	string text_;
	T_edit_style style_= T_singleline_edit;
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
};


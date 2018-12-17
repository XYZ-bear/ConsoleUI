#pragma once
#include "cwbase.h"

enum T_scroll_style
{
	T_h_scroll,
	T_v_scroll
};

class cscroll :
	public cwbase
{
public:
	cscroll();
	~cscroll();

	bool update();
	bool init();
	void click_in(c_point p);
	void click_out(c_point p);
	void double_click(c_point p);
	void set_style(T_scroll_style style) { style_ = style; };
	void set_spin_color(COLORREF color) { spin_color_ = color; };
	T_scroll_style get_style() { return style_; };
private:
	COLORREF spin_color_ = RGB(255, 255, 255);
	T_scroll_style style_= T_h_scroll;
	c_rect scroll_bar_;
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
};


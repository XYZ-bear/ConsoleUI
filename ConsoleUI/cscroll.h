#pragma once
#include "cwbase.h"
#include "cevent.h"

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

	bool update(bool redraw = false);
	bool init();
	void click_in(c_point p);
	void click_out(c_point p);
	void double_click(c_point p);
	void set_style(T_scroll_style style) { style_ = style; };
	void set_bar_color(COLORREF color) { bar_color_ = color; };
	void set_bar_drag_color(COLORREF color) { bar_drag_color_ = color; };
	T_scroll_style get_style() { return style_; };
	void set_one_step(float step) { one_step_ = step; };
	/*move distance*/
	void scroll(int xy);
	void set_range(int min, int max);
private:
	void scroll_to_(int xy);
private:
	T_scroll_style style_= T_h_scroll;
	c_rect scroll_bar_;
	COLORREF bar_color_;
	COLORREF bar_drag_color_;
	COLORREF bar_active_color_;
	int scroll_bar_offset = 3;
	c_point old_point_;
	float one_step_ = 0.3f;
	int max_pos_ = 100;
	int min_pos_ = 0;
public:
	void mouse_move_in(c_point p);
	void mouse_move_out(c_point p);
	void drag(c_point p);
	void mouse_wheeled(bool up);
};


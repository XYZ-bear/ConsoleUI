#pragma once

/*windows event*/
#define D_mouse_up_wheel 7864320
#define D_mouse_down_wheel 4287102976

/*default init*/
#define D_default_font_height 16       //width=height/2

/*event define*/
enum T_ctr_event {
	T_click_in_event,
	T_click_out_event,
	T_double_click_event,
	T_mouse_move_event,
	T_mouse_move_in_event,
	T_mouse_move_out_event,
	T_mouse_wheeled_event,
	T_input_key,
	T_focus,
	T_drag_event,
	T_scroll_event
};

/*hint_type*/
enum T_hint
{
	T_hint_nono,
	T_hint_right,
	T_hint_bottom,
	T_hint_right_bottom
};

/*control type*/
enum T_ctr_type
{
	T_frame,
	T_window,
	T_button,
	T_edit,
	T_tips,
	T_scroll
};

/*align*/
enum T_align
{
	T_v_align_left = 0x1,
	T_v_align_right = 0x2,
	T_v_align_center = 0x4,
	T_h_align_left = 0x8,
	T_h_align_right = 0x10,
	T_h_align_center = 0x20,
};

typedef _KEY_EVENT_RECORD c_key;


struct c_point {
	long x;
	long y;

	bool operator==(const c_point p) const
	{
		return p.x == x && p.y == y;
	}
	bool operator!=(const c_point p) const
	{
		return p.x != x && p.y != y;
	}
	void operator+=(const c_point p)
	{
		x += p.x;
		y += p.y;
	};
	bool operator>(const c_point p) {
		return x > p.x&&y > p.y;
	}
	bool operator<(const c_point p) {
		return x < p.x&&y < p.y;
	}
	void operator-=(const c_point p)
	{
		x -= p.x;
		y -= p.y;
	}
	c_point operator+(const c_point p) const
	{
		return { x + p.x,y + p.y };
	}
	c_point operator-(const c_point p) const
	{
		return { x - p.x,y - p.y };
	}
};

struct c_rect {
	c_point p;
	int width;
	int height;
	bool is_point_in(c_point pi) {
		if (pi > p&&pi < c_point{p.x + width, p.y + height})
			return true;
		return false;
	}
};

struct spin_info
{
	int line = 0;
	int in_line_pos = 0;
	int in_line_index = 0;
	char in_line_ch = 0;
	char pre_ch = 0;
	char next_ch = 0;
};
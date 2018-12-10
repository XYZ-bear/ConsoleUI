#pragma once

/*mouse_event*/
#define D_mouse_click_event 1

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
	T_window,
	T_button,
	T_edit
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
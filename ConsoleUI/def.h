#pragma once


#define MOUSE_EVENT_CLICK 1

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
#pragma once

#include "cwbase.h"
#include <list>
#include <functional>
#include "ctips.h"

class cwindow :public cwbase
{
public:
	cwindow();
	~cwindow();
	bool create(string title,c_point op, int width, int height);
	bool create(string title, c_point op, int width, int height,cwbase* parent);
	string get_title() { return title_; }
	void set_title(string title) { title_ = title; }
	template<class _Fn>
	void regist_control_call(int id, cwbase *base, _Fn func) {
		add_cmd(this,id,base,func);
	}
	bool is_close() { return is_close_; }
	T_hint hint_t() { return hint_t_; }
	T_align prejudge_align_v(c_point p);
	T_align prejudge_align_h(c_point p);
private:
	list<cwbase *> chidren_list;
	string title_;
	c_point old_init_point;
	c_point pre_point;
	c_point cur_point;
	c_point hint_point;
	c_rect old_rect;
	int header_height;
	bool is_mouse_in_header;
	T_hint hint_t_ = T_hint_nono;
	bool is_drag_ = false;
	bool is_close_ = false;
	bool is_max_ = false;
	cwbase *point_ctr=nullptr;
	cwbase *focus_ctr = nullptr;
	ctips tips;
public:
	bool init();
	bool update(bool redraw);
	void click_in(c_point p);
	void click_out(c_point p);
	void mouse_move(c_point p);
	void double_click(c_point p);
	void hint(c_point p);
	void drag(drag_info p);
	void size_change(c_rect rect);
	void input_key(c_key key);
public:
	void close_click( const void* p);
	void max_click( const void* p);
};


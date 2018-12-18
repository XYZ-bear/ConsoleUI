#pragma once

#include "cwbase.h"
#include <list>
#include <functional>
#include "ctips.h"

class base_brigde {
public:
	virtual void do_event(int id, cwbase *base, void *data) {};
};

template<typename T>
class brigde:public base_brigde {
	typedef void (T::*callback)(cwbase *base,void* data);
	typedef map<cwbase*, callback> obj_func_bridge;
private:
	T * ob_;
	map<int, obj_func_bridge> event_func_;
public:
	static brigde& instance() {
		static brigde bg;
		return bg;
	}
	void add_event(T *ob,int id,cwbase *base,callback func) {
		ob_ = ob;
		event_func_[id][base] = func;
	}
	void do_event(int id, cwbase *base,void *data) {
		if (event_func_[id][base])
			(ob_->*event_func_[id][base])(base, data);
	}
};

class cwindow :public cwbase
{
public:
	cwindow();
	~cwindow();
	bool add_child(cwbase *child);
	bool create(string title,c_point op, int width, int height);
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
	void update_window(bool redraw=false);
private:
	template<class T, class _Fn>
	void add_cmd(T *con, int id, cwbase *base, _Fn func) {
		if (!brigde_) {
			brigde_ = new brigde<T>();
		}
		((brigde<T>*)brigde_)->add_event(con, id, base, func);
		call_func_ = [this](int id, cwbase *base,void *data) {
			brigde_->do_event(id, base, data);
		};
	}
	cwbase* point_in_ctr(c_point p);
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
	function<void (int id, cwbase *base,void* data)> call_func_;
	base_brigde *brigde_ = nullptr;
	cwbase *point_ctr=nullptr;
	cwbase *focus_ctr = nullptr;
	ctips tips;
public:
	bool init();
	bool update();
	void click_in(c_point p);
	void click_out(c_point p);
	void mouse_move(c_point p);
	void double_click(c_point p);
	void hint(c_point p);
	void drag(c_point p);
	void size_change(c_rect rect);
	void input_key(c_key key);
public:
	void close_click(cwbase *base, void* p);
	void max_click(cwbase *base, void* p);
};


#pragma once

#include "cwbase.h"
#include <list>
#include <functional>


template<typename T>
class brigde {
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
	void de_event(int id, cwbase *base,void *data) {
		(ob_->*event_func_[id][base])(base,data);
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
	void close_click(cwbase *base, void* p);
	template<class _Fn>
	void regist_control_call(int id, cwbase *base, _Fn func) {
		add_cmd(this,id,base,func);
	}
	bool is_close() { return is_close_; }
private:
	template<class T, class _Fn>
	void add_cmd(T *con, int id, cwbase *base, _Fn func) {
		brigde<T>::instance().add_event(con, id, base, func);
		call_func_ = [this](int id, cwbase *base,void *data) {
			brigde<T>::instance().de_event(id, base, data);
		};
	}
private:
	list<cwbase *> chidren_list;
	string title_;
	c_point old_init_point;
	c_point pre_point;
	c_rect old_rect;
	int header_height;
	bool is_mouse_in_header;
	bool is_close_ = false;
	bool is_max_ = false;
	function<void (int id, cwbase *base,void* data)> call_func_;
public:
	bool init();
	bool update();
	void click_in(c_point p);
	void click_out(c_point p);
	void mouse_move(c_point p);
	void double_click(c_point p);
};


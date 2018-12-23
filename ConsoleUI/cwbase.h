#pragma once

#include "cgdi.h"
#include "cevent.h"
#include <map>
#include <list>

class cwbase:public cevent
{
private:
	HANDLE m_console_hand_;
	HDC buffer_hdc_;
	T_ctr_type ctr_type_;

protected:
	c_point _left_top;
	c_point _right_bottom;
	int _width =0;
	int _height =0;
	cgdi _gdi;
	T_align _align;
	/*color*/
	COLORREF _bk_color=RGB(0,0,0);
	COLORREF _active_color;
	COLORREF _mouse_in_color;

	bool _enable_focus = true;
	bool _is_focus;
	bool _is_show = true;
	int _font_height = D_default_font_height;
	int _font_width = D_default_font_height / 2;

	cwbase *_parent = nullptr;
	list<cwbase*> _childrend;
	string _tip_str;

public:
	cwbase();
	virtual ~cwbase();
	int get_width();
	int get_height();
	c_point &get_left_top();
	c_point &get_right_bottom();
	int get_console_width();
	int get_console_height();
	bool is_point_in(c_point p);
	cgdi& get_gdi();
	HDC& get_hdc();
	HWND get_hand();
	c_point get_point();
	void set_point(c_point point);
	void set_size(c_point op, int width, int height);
	void set_align(T_align align) { _align = align; }
	T_align get_align() { return _align; }

	void set_ctr_type(T_ctr_type ctr_type) { ctr_type_ = ctr_type; }
	T_ctr_type get_ctr_type() { return ctr_type_ ; }

	void set_bk_color(COLORREF color) { _active_color = color; _bk_color = color; };
	void set_mouse_in_color(COLORREF color) { _mouse_in_color = color; };

	bool enable_focus() { return _enable_focus; }
	void set_enable_focus(bool is) { _enable_focus = is; }

	bool is_focus() { return _is_focus; }
	void set_is_focus(bool is) { _is_focus = is; }

	bool is_show() { return _is_show; }
	void set_is_show(bool is) { _is_show = is; }

	void set_font_height(int height) { _font_height = height; _font_width = height / 2; }
	int get_font_size() { return _font_height; }

	c_point get_client_point(c_point p) {
		return p - _left_top;
	}

	c_point get_parent_point(c_point p) {
		if (_parent)
			return _parent->get_left_top() + p;
	}

	void erase_bk();

	template<class T,class _Fn>
	void set_timer(T *ob,int interval, _Fn func_) {
		ctimer::instance().add_timer(ob, interval, func_);
	}

	template<class T, class _Fn>
	void kill_timer(T *ob, _Fn func_) {
		ctimer::instance().kill_timer(ob, func_);
	}

	cwbase *get_parent() { return _parent; }

	list<cwbase*> &get_children() { return _childrend; }

	void move_child_to_end(cwbase* ctr);

	void remove_self();

	void remove_child(cwbase *ctr);

	void update_parent();

	void do_event(T_ctr_event id,const void *data);

	void set_tip(string str) { _tip_str = str; }

	string get_tip() { return _tip_str; }
public:
	virtual bool init();
	virtual bool update(bool redraw = false);
	virtual bool create( c_point op, int width, int height, cwbase *parent=nullptr, COLORREF _bk_color = RGB(0, 0, 0));
	virtual void click_in(c_point p) { };
	virtual void click_out(c_point p) { };
	virtual void double_click(c_point p) { };
	virtual void mouse_move(c_point p){ };
	virtual void mouse_move_in(c_point p) { };
	virtual void mouse_move_out(c_point p) { };
	virtual void mouse_wheeled(bool up) {};
	virtual void input_key(c_key key) { };
	virtual void focus() { };
	virtual void drag(c_point p) { };
	virtual bool pre_event(T_ctr_event id, const void *data) { return true; };
};


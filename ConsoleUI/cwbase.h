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

	cwbase *_parent = nullptr;
	list<cwbase*> _childrend;
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

	void set_font_height(int height) { _font_height = height; }
	int get_font_size() { return _font_height; }

	c_point get_client_point(c_point p) {
		return p - _left_top;
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

	list<cwbase*> &get_children() { return _childrend; }

	void update_parent();
public:
	virtual bool init() = 0;
	virtual bool update() { update_parent(); return true; };
	virtual bool create( c_point op, int width, int height, cwbase *parent=nullptr, COLORREF _bk_color = RGB(255, 255, 255));
	virtual void click_in(c_point p) { call_func_(T_click_in_event,&p); };
	virtual void click_out(c_point p) { call_func_(T_click_out_event, &p); };
	virtual void double_click(c_point p) { call_func_(T_double_click_event, &p); };
	virtual void mouse_move(c_point p){ call_func_(T_mouse_move_event, &p); };
	virtual void mouse_move_in(c_point p) { call_func_(T_mouse_move_in_event, &p); };
	virtual void mouse_move_out(c_point p) { call_func_(T_mouse_move_out_event, &p); };
	virtual void input_key(c_key key) { call_func_(T_input_key, &key); };
	virtual void foucs() { call_func_(T_focus, nullptr); };
	virtual void drag(c_point p) { call_func_(T_drag_event, &p); };
};


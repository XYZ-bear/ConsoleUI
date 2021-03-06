#include "stdafx.h"
#include "cwbase.h"


cwbase::cwbase()
{
	m_console_hand_= CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}

cwbase::~cwbase()
{
}

int cwbase::get_width() { return _width; }

int cwbase::get_height() { return _height; }

int cwbase::get_console_width() {
	RECT r;
	GetClientRect(GetConsoleWindow(),&r);
	return r.right;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(m_console_hand_, &bInfo);
	return bInfo.srWindow.Right - bInfo.srWindow.Left;
}

int cwbase::get_console_height() {
	RECT r;
	GetClientRect(GetConsoleWindow(), &r);
	return r.bottom;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(m_console_hand_, &bInfo);
	return bInfo.srWindow.Bottom - bInfo.srWindow.Top;
}

cgdi& cwbase::get_gdi() { return _gdi; }

bool cwbase::init() {
	for (auto child : _childrend) {
		child->init();
	}
	return true;
}

bool cwbase::update(bool redraw) {
	//if (_is_show) {
	//	for (auto child : _childrend) {
	//		if (child->is_show()) {
	//			if (redraw)
	//				child->update(redraw);
	//			else
	//				child->update_parent();
	//		}
	//	}
	//	if (redraw) {
	//		update_parent();
	//	}
	//	else {

	//		auto parent = this;
	//	
	//		while (parent&&parent->get_ctr_type() != T_window) {
	//			parent->update_parent();
	//			parent = parent->_parent;
	//		}
	//	}
	//}

	if (_is_show) {
		if (redraw) {
			//for (auto child : _childrend) {
			//	if (child->is_show()) {
			//		child->update(redraw);
			//	}
			//}
			update_parent();
		}
		else {
			for (auto child : _childrend) {
				if (child->is_show()) {
					//
					child->update_parent();
					//child->update(redraw);
				}
			}
		}
	}
	return true;
}

void cwbase::to_top(cwbase *ctr) {
	if (!ctr)
		return;
	auto parent = ctr->get_parent();
	if (!parent)
		return;
	auto it = parent->_childrend.end();
	while (parent->_childrend.begin() != it) {
		auto child = *(--it);
		if (ctr == child) {
			parent->_childrend.erase(it);
			parent->_childrend.push_back(ctr);
			break;
		}
	}
}

bool cwbase::is_child(cwbase *ctr) {
	if (!ctr)
		return false;
	if (ctr == this)
		return true;
	for (auto child : ctr->_childrend) {
		child->is_child(ctr);
	}
	return false;
}

bool cwbase::create(c_point op, int width, int height, cwbase *parent, COLORREF bk_color) {
	_left_top = op;
	_right_bottom = { op.x + width,op.y + height };
	_width = width;
	_height = height;

	_active_color = bk_color;
	_bk_color = bk_color;
	_mouse_in_color = bk_color;

	_align = T_v_align_left;
	_gdi.set_rng(width, height);
	_gdi.set_refer_point(_left_top);
	_gdi.init();

	_parent = parent;

	if (_parent) {
		_parent->_childrend.push_back(this);
	}
	return true;
}

HDC& cwbase::get_hdc() {
	return buffer_hdc_;
}

bool cwbase::is_point_in(c_point p) {
	if (p > _left_top&&p < _right_bottom)
		return true;
	return false;
}

HWND cwbase::get_hand() {
	return GetConsoleWindow();
}

void cwbase::set_point(c_point point){
	_left_top = point;
	_right_bottom = { point.x + _width,point.y + _height };
	_gdi.set_refer_point(_left_top);
}

void cwbase::set_size(c_point op, int width, int height) {
	_left_top = op;
	_right_bottom = { op.x + width,op.y + height };
	_width = width;
	_height = height;
	_gdi.set_rng(width, height);
	_gdi.set_refer_point(_left_top);
}

c_point cwbase::get_point() {
	return _left_top;
}

c_point &cwbase::get_left_top() {
	return _left_top;
}

c_point &cwbase::get_right_bottom() {
	return _right_bottom;
}

void cwbase::erase_bk() {
	_gdi.fill_rect({ 0,0 }, { _width ,_height },_bk_color);
}

void cwbase::update_parent() {
	if (_parent) {
		BitBlt(_parent->get_gdi().buffer_hdc_, _gdi.refer_c_point_.x, _gdi.refer_c_point_.y, _gdi.width_, _gdi.height_, _gdi.buffer_hdc_, 0, 0, SRCCOPY);
	}
}

void cwbase::move_child_to_end(cwbase* ctr) {
	auto it = _childrend.end();
	while (_childrend.begin() != it) {
		auto child = *(--it);
		if (ctr == child) {
			_childrend.erase(it);
			_childrend.push_back(ctr);
			break;
		}
	}
}

void cwbase::remove_self() {
	if (_parent)
		_parent->remove_child(this);
}

void cwbase::remove_child(cwbase *ctr) {
	auto it = _childrend.end();
	while (_childrend.begin() != it) {
		if (ctr == *(--it)) {
			_childrend.erase(it);
			break;
		}
	}
}

cwbase *cwbase::get_root() {
	auto ctr = this;
	while (ctr&&ctr->get_ctr_type() != T_window) {
		ctr->update_parent();
		ctr = ctr->_parent;
	}
	return ctr;
}

void cwbase::do_event(T_ctr_event id, const void *data) {
	if (!pre_event(id, data))
		return;
	switch (id)
	{
	case T_click_in_event:click_in(*(c_point*)data);
		break;
	case T_click_out_event:click_out(*(c_point*)data);
		break;
	case T_double_click_event:double_click(*(c_point*)data);
		break;
	case T_mouse_move_event:mouse_move(*(c_point*)data);
		break;
	case T_mouse_move_in_event:mouse_move_in(*(c_point*)data);
		break;
	case T_mouse_move_out_event:mouse_move_out(*(c_point*)data);
		break;
	case T_mouse_wheeled_event:mouse_wheeled(*(bool*)data);
		break;
	case T_input_key:input_key(*(c_key*)data);
		break;
	case T_focus:focus();
		break;
	case T_drag_event:drag(*(drag_info*)data);
		break;
	case T_scroll_event:
		break;
	default:
		break;
	}
	call_func_(id,data);
}
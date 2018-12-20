#pragma once

#include "cwindow.h"
#include <list>
#include <thread>
#include <iostream>
#include "ctimer.h"
using namespace std;

class cframe:public cwbase
{
public:
	cframe();
	~cframe();

	bool init() {
		tips.create({ 0,0 }, 100, 20, this);
		return cwbase::init();
	}
	static cframe& instance() {
		static cframe frame;
		return frame;
	}

	int get_console_width() {
		RECT r;
		GetClientRect(GetConsoleWindow(), &r);
		return r.right;
	}

	int get_console_height() {
		RECT r;
		GetClientRect(GetConsoleWindow(), &r);
		return r.bottom;
	}

	void run() {

		HDC hdc = GetDC(GetConsoleWindow());
		HBITMAP bmp = CreateCompatibleBitmap(hdc, get_console_width(), get_console_height());
		HDC buffer_hdc = CreateCompatibleDC(NULL);
		SelectObject(buffer_hdc, bmp);
		c_point old;
		if (_childrend.size() > 0)
			active_ctr = *(--_childrend.end());
		
		c_point data;
		c_point root_point;
		c_point old_root_point;
		while (1) {

			RECT rect{ 0,0,get_console_width(),get_console_height() };
			HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
			FillRect(buffer_hdc, &rect,br);
			DeleteObject(br);

			INPUT_RECORD keyRec;
			DWORD state = 0, res;
			HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
			int num;
			LPDWORD f=(LPDWORD)&num;
			GetNumberOfConsoleInputEvents(hIn, f);
			num = *f;

			if (num > 0) {
				ReadConsoleInput(hIn, &keyRec, 1, &res);
				erase_bk();
				//update();
				if (keyRec.EventType == MOUSE_EVENT) {
					POINT p;
					GetCursorPos(&p);
					ScreenToClient(GetConsoleWindow(), &p);
					
					if (keyRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
						data = { p.x ,p.y };
						root_point = data;
						active_ctr = get_point_ctr(this, data);
						do_somthing(active_ctr, T_mouse_move_event, data);

						if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
							OutputDebugString("a");
							do_somthing(old_move_ctr, T_drag_event, root_point - old_root_point);
							old_root_point = root_point;
							continue;
						}

						if (old_move_ctr != active_ctr) {
							do_somthing(active_ctr, T_mouse_move_in_event, data);
							do_somthing(old_move_ctr, T_mouse_move_out_event, data);
							tips.show_tip(active_ctr, root_point);
						}
						old_move_ctr = active_ctr;
						old_root_point = root_point;
					}
					if (keyRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK) {
						if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
							do_somthing(active_ctr, T_double_click_event, data);
						}
					}
					if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						if (keyRec.Event.MouseEvent.dwEventFlags == 0) {
							do_somthing(active_ctr, T_click_in_event, data);
							if (old_click_ctr != active_ctr)
								do_somthing(old_click_ctr, T_click_out_event, data);
							active_window = get_ctr_root_window(active_ctr);
							if (active_window) {
								if (!active_window->hint_t()) {
									switch_window((cwbase*)active_window);
								}
							}
							old_click_ctr = active_ctr;
						}
					}
				}

				if (keyRec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
					DeleteObject(bmp);
					bmp = CreateCompatibleBitmap(hdc, get_console_width(), get_console_height());
					SelectObject(buffer_hdc, bmp);
				}

				if (keyRec.EventType == KEY_EVENT) {
					if (keyRec.Event.KeyEvent.bKeyDown)
					{
						if(active_ctr)
							active_ctr->do_event(T_input_key, &keyRec.Event.KeyEvent);
					}
				}
			}
			else
				Sleep(1);

			ctimer::instance().check_timer();
			update();
			BitBlt(hdc, 0, 0, get_console_width(), get_console_height(), get_gdi().buffer_hdc_, 0, 0, SRCCOPY);
		}
	}

	void input_event() {

	}

	bool add(cwindow* window) {
		if (window) {
			window_list.push_back(window);
			return true;
		}
		return false;
	}

	void remove(cwindow* window) {
		for (auto it = window_list.begin(); it != window_list.end(); ++it) {
			if ((*it) == window && window != nullptr) {
				window_list.erase(it);
				break;
			}
		}
	}

	cwbase* get_point_ctr(cwbase *parent,c_point &p) {
		if (parent) {
			if (parent->is_point_in(p)) {
				p=parent->get_client_point(p);
				auto &children = parent->get_children();
				if (children.size() == 0) {
					return parent;
				}
				else {
					auto it = children.end();
					
					while (children.begin() != it) {
						auto child = *(--it);
						auto res=get_point_ctr(child, p);
						if (res) {
							return res;
						}
						else if(children.begin() == it){
							return parent;
						}
					}
				}
			}
			else
				return nullptr;
		}
	}

	cwindow *get_ctr_root_window(cwbase *ctr) {
		if (!ctr)
			return nullptr;
		while (ctr->get_ctr_type() != T_window) {
			ctr = ctr->get_parent();
			if (ctr == nullptr)
				return nullptr;
		}
		return (cwindow*)ctr;
	}

	bool do_somthing(cwbase* ctr, T_ctr_event id, c_point data ) {
		if (!ctr)
			return false;
		ctr->do_event(id, &data);
	}

	void switch_window(cwbase* ctr) {
		auto it = _childrend.end();
		while (_childrend.begin() != it) {
			auto child = *(--it);
			if (ctr == child) {
				_childrend.erase(it);
				_childrend.pop_back();
				_childrend.push_back(ctr);
				_childrend.push_back(&tips);
				break;
			}
		}
	}
private:
	list<cwindow*> window_list;
	cwindow *active_window;
	cwbase *old_click_ctr;
	cwbase *old_move_ctr;
	cwbase *active_ctr;
	ctips tips;
};


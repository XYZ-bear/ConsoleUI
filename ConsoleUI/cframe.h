#pragma once  //sf	fsdf			dsd

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
		cwindow *window=new cwindow();
		window->create("windows1", { 10,20 }, 600, 400);

		//cwindow *window2 = new cwindow();
		//window2->create("windows2", { 100,60 }, 300, 200);

		//int k = 40;
		//for (int i = 0; i < 10; i++) {
		//	cwindow *window4 = new cwindow();
		//	window4->create(to_string(i), { k + i,60 }, 600, 400);
		//}
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
		c_point old;
		if (_childrend.size() > 0)
			active_ctr = *(--_childrend.end());
		
		c_point data{ 0,0 };
		c_point root_point{ 0,0 };
		c_point old_root_point = { 0,0 };

		while (1) {

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
			
				if (keyRec.EventType == MOUSE_EVENT) {
					POINT p;
					GetCursorPos(&p);
					ScreenToClient(GetConsoleWindow(), &p);

					if (keyRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
						data = { p.x ,p.y };
						root_point = data;
						active_ctr = get_point_ctr(this, data);
						
						if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
							do_somthing(old_move_ctr, T_drag_event, root_point - old_root_point);
							old_root_point = root_point;
							goto update;
						}
						do_somthing(active_ctr, T_mouse_move_event, data);

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
							do_somthing(active_ctr, T_focus, true);
							tips.set_is_show(false);
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
					else if (keyRec.Event.MouseEvent.dwButtonState == 0) {
						if (keyRec.Event.MouseEvent.dwEventFlags == 0) {
						}

					}

					if (keyRec.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED) {
						if (keyRec.Event.MouseEvent.dwButtonState == D_mouse_up_wheel)
							do_somthing(active_ctr, T_mouse_wheeled_event, true);
						else
							do_somthing(active_ctr, T_mouse_wheeled_event, false);
					}
				}

				if (keyRec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
		/*			DeleteObject(bmp);
					bmp = CreateCompatibleBitmap(hdc, get_console_width(), get_console_height());
					SelectObject(buffer_hdc, bmp);*/
				}

				if (keyRec.EventType == KEY_EVENT) {
					if (keyRec.Event.KeyEvent.bKeyDown)
					{
						if(old_click_ctr)
							old_click_ctr->do_event(T_input_key, &keyRec.Event.KeyEvent);
					}
				}
			}
			else
				Sleep(1);

			update:
			ctimer::instance().check_timer();
			redraw_windows();
			_gdi.update();
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

	bool do_somthing(cwbase* ctr, T_ctr_event id, bool data) {
		if (!ctr)
			return false;
		ctr->set_is_focus(data);
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

	void redraw_windows() {
		for (auto child : _childrend) {
			if (child->is_show())
				child->update_parent();
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


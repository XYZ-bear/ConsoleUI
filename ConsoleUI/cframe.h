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
	bool init() { return true; };

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
		//for (auto &cwindow : window_list) {
		//	if (cwindow) {
		//		cwindow->init();
		//		cwindow->update_window(true);
		//	}
		//}

		for (auto &window : _childrend) {
			if (window) {
				window->init();
				((cwindow*)window)->update_window(true);
			}
		}

		HDC hdc = GetDC(GetConsoleWindow());
		HBITMAP bmp = CreateCompatibleBitmap(hdc, get_console_width(), get_console_height());
		HDC buffer_hdc = CreateCompatibleDC(NULL);
		SelectObject(buffer_hdc, bmp);
		c_point old;
		if (_childrend.size() > 0)
			active_ctr = *(--_childrend.end());
		//if (window_list.size() > 0) {
		//	active_window = *(--window_list.end());
		//}
		//for (auto &cwindow : window_list) {
		//	if (cwindow) {
		//		cwindow->update_window();
		//	}
		//}
		
		while (1) {

			RECT rect{ 0,0,get_console_width(),get_console_height() };
			HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
			FillRect(buffer_hdc, &rect,br);
			DeleteObject(br);
			//for (auto &cwindow : window_list) {
			//	if (cwindow) {
			//		cwindow->update();
			//	}
			//}
			//active_window->update();

			INPUT_RECORD keyRec;
			DWORD state = 0, res;
			HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
			int num;
			LPDWORD f=(LPDWORD)&num;
			GetNumberOfConsoleInputEvents(hIn, f);
			num = *f;
			if (num > 0) {
				ReadConsoleInput(hIn, &keyRec, 1, &res);

				if (keyRec.EventType == MOUSE_EVENT) {
					POINT p;
					GetCursorPos(&p);
					ScreenToClient(GetConsoleWindow(), &p);
					if (keyRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
						if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
							if (active_ctr) {
								active_ctr->drag({ p.x ,p.y });
								continue;
							}
						}
						if (active_ctr) {
							active_ctr->mouse_move({ p.x ,p.y });
						}
					}
					if (keyRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK) {
						if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
							if (active_ctr)
								active_ctr->double_click({ p.x ,p.y });
						}
					}
					if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						if (keyRec.Event.MouseEvent.dwEventFlags == 0) {
							old = { p.x ,p.y };
							auto old_ctr = active_ctr;
							auto active_ctr = get_point_ctr(this, old);
							if (old_ctr)
								old_ctr->click_out(old);
							if (active_ctr)
								active_ctr->click_in(old);
							
							if (active_ctr->get_ctr_type() == T_window) {
								if (!((cwindow*)active_ctr)->hint_t()) {
									auto it = _childrend.end();
									while (_childrend.begin() != it) {
										if (active_ctr == *(--it)) {
											_childrend.erase(it);
											_childrend.push_back(active_ctr);
											break;
										}
									}
								}
							}
							//auto it = window_list.end();
							//auto active_it = window_list.end();
							//while (window_list.begin() != it) {
							//	auto window = *(--it);
							//	if (window) {
							//		if (window->hint_t())
							//			break;
							//		if (window->is_point_in({ p.x ,p.y }) && active_it == window_list.end()) {
							//			//window->click_in(old);
							//			cwbase *df = get_point_ctr(window, { p.x,p.y });
							//			if (df)
							//				OutputDebugString(to_string(df->get_ctr_type()).c_str());

							//			if (window->is_close()) {
							//				window_list.erase(it);
							//				if (window_list.size() > 0) {
							//					active_window = *(--window_list.end());
							//					active_it = window_list.end();
							//				}
							//				break;
							//			}
							//			active_window = window;
							//			active_it = it;
							//		}
							//		else
							//			window->click_out(old);
							//	}
							//}
							//if (active_it != window_list.end()) {
							//	window_list.erase(active_it);
							//	window_list.push_back(active_window);
							//}
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
							active_ctr->input_key(keyRec.Event.KeyEvent);
					}
				}

				//active_window->update_window();
			}
			else
				Sleep(1);

			ctimer::instance().check_timer();

			for (auto &window : _childrend) {
				if(window&&window->get_gdi().get_change()) {
					BitBlt(buffer_hdc, window->get_gdi().refer_c_point_.x, window->get_gdi().refer_c_point_.y, window->get_gdi().width_, window->get_gdi().height_, window->get_gdi().buffer_hdc_, 0, 0, SRCCOPY);
				}
			}
			BitBlt(hdc, 0, 0, get_console_width(), get_console_height(), buffer_hdc, 0, 0, SRCCOPY);
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

	cwbase* get_point_ctr(cwbase *parent,c_point p) {
		if (parent) {
			if (parent->is_point_in(p)) {
				auto &children = parent->get_children();
				if (children.size() == 0) {
					return parent;
				}
				else {
					auto it = children.end();
					
					while (children.begin() != it) {
						auto child = *(--it);
						auto res=get_point_ctr(child, parent->get_client_point(p));
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
private:
	list<cwindow*> window_list;
	cwindow *active_window;
	cwbase *active_ctr;
};


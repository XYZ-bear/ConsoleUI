#pragma once

#include "cwindow.h"
#include <list>
#include <thread>
#include <iostream>
#include "ctimer.h"
using namespace std;

class cframe
{
public:
	cframe();
	~cframe();
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
		for (auto &cwindow : window_list) {
			if (cwindow) {
				cwindow->init();
			}
		}

		HDC hdc = GetDC(GetConsoleWindow());
		HBITMAP bmp = CreateCompatibleBitmap(hdc, get_console_width(), get_console_height());
		HDC buffer_hdc = CreateCompatibleDC(NULL);
		SelectObject(buffer_hdc, bmp);
		c_point old;
		if (window_list.size() > 0) {
			active_window = *(--window_list.end());
		}
		
		while (1) {

			RECT rect{ 0,0,get_console_width(),get_console_height() };
			HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
			FillRect(buffer_hdc, &rect,br);
			DeleteObject(br);
			for (auto &cwindow : window_list) {
				if (cwindow) {
					cwindow->update();
				}
			}


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
							if (active_window) {
								active_window->drag({ p.x ,p.y });
							}
						}
						if (active_window) {
							active_window->mouse_move({ p.x ,p.y });
						}
					}
					if (keyRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK) {
						if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
							if (active_window) {
								active_window->double_click({ p.x ,p.y });
							}
						}
					}
					if (keyRec.Event.MouseEvent.dwEventFlags == 0) {
						old = { p.x ,p.y };

						auto it = window_list.end();
						auto active_it = window_list.end();
						while (window_list.begin() != it) {
							auto window = *(--it);
							if (window) {
								if (window->hint_t())
									break;
								if (window->is_point_in({ p.x ,p.y }) && active_it == window_list.end()) {
									window->click_in(old);
									if (window->is_close()) {
										window_list.erase(it);
										if (window_list.size() > 0) {
											active_window = *(--window_list.end());
											active_it = window_list.end();
										}
										break;
									}
									active_window = window;
									active_it = it;
								}
								else
									window->click_out(old);
							}
						}
						if (active_it != window_list.end()) {
							window_list.erase(active_it);
							window_list.push_back(active_window);
						}
					}
					if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						if (active_window) {
							active_window->click_in({ p.x ,p.y });
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
						active_window->input_key(keyRec.Event.KeyEvent.uChar.AsciiChar);
					}
				}
			}
			else
				Sleep(1);

			ctimer::instance().check_timer();

			for (auto &cwindow : window_list) {
				if (cwindow) {
					BitBlt(buffer_hdc, cwindow->get_gdi().refer_c_point_.x, cwindow->get_gdi().refer_c_point_.y, cwindow->get_gdi().width_, cwindow->get_gdi().height_, cwindow->get_gdi().buffer_hdc_, 0, 0, SRCCOPY);
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
private:
	list<cwindow*> window_list;
	cwindow *active_window;
};


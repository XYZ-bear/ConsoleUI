#pragma once

#include "cwindow.h"
#include <list>
#include <thread>
#include <iostream>
using namespace std;

class cframe
{
public:
	cframe();
	~cframe();
	static cframe instance() {
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
		//thread t1(&cframe::input_event, this);
		//t1.detach();

		HDC hdc = GetDC(GetConsoleWindow());
		HBITMAP bmp = CreateCompatibleBitmap(hdc, get_console_width(), get_console_height());
		HDC buffer_hdc = CreateCompatibleDC(NULL);
		SelectObject(buffer_hdc, bmp);
		while (1) {
			INPUT_RECORD keyRec;
			DWORD state = 0, res;
			HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
			ReadConsoleInput(hIn, &keyRec, 1, &res);

			if (keyRec.EventType == MOUSE_EVENT) {
				if (keyRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
					if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						POINT p;
						GetCursorPos(&p);
						ScreenToClient(GetConsoleWindow(), &p);
						auto it = window_list.end();
						while (window_list.begin() != it) {
							auto window = *(--it);
							if (window&&window->is_point_in({ p.x ,p.y })) {
								auto old_window_p=window->get_point();
								auto move = c_point{ p.x ,p.y } - old;
								window->set_point(move+ old_window_p);
								break;
							}
						}
					}
				}
				if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					POINT p;
					GetCursorPos(&p);
					ScreenToClient(GetConsoleWindow(), &p);

					old = { p.x ,p.y };

					auto it = window_list.end();
					while (window_list.begin() != it) {
						auto window = *(--it);
						if (window&&window->is_point_in({ p.x ,p.y })) {
							window_list.erase(it);
							window_list.push_back(window);
							window->click();
							break;
						}
					}
				}
			}

			if (keyRec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
				DeleteObject(bmp);
				bmp = CreateCompatibleBitmap(hdc, get_console_width(), get_console_height());
				SelectObject(buffer_hdc, bmp);
			}



			RECT rect{ 0,0,get_console_width(),get_console_height()};
			FillRect(buffer_hdc, &rect, CreateSolidBrush(RGB(0,0,0)));

			for (auto &cwindow : window_list) {
				if (cwindow) {
					cwindow->update();
					BitBlt(buffer_hdc, cwindow->get_cgdi().refer_c_point_.x, cwindow->get_cgdi().refer_c_point_.y, cwindow->get_cgdi().width_, cwindow->get_cgdi().height_, cwindow->get_cgdi().buffer_hdc_, 0, 0, SRCCOPY);
				}
			}
			BitBlt(hdc, 0, 0, get_console_width(), get_console_height(), buffer_hdc, 0, 0, SRCCOPY);
			//Sleep(10);
		}
	}

	void input_event() {
		while (1) {
			INPUT_RECORD keyRec;
			DWORD state = 0, res;
			HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
			ReadConsoleInput(hIn, &keyRec, 1, &res);

			if (keyRec.EventType == KEY_EVENT)
			{
				if (keyRec.Event.KeyEvent.bKeyDown == 1)
				{
					if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
					{

					}

					if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_UP)
					{
					}

					if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
					{
					}
				}

				if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
				{
				}

				if (keyRec.Event.KeyEvent.bKeyDown)
				{
					//m_cmd += keyRec.Event.KeyEvent.uChar.AsciiChar;
					//MessageBox(GetConsoleWindow(), "sfdsfs", "", 1);
				}
			}
			if (keyRec.EventType == MOUSE_EVENT) {
				if (keyRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
					if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						POINT p;
						GetCursorPos(&p);
						ScreenToClient(GetConsoleWindow(), &p);
						auto it = window_list.end();
						while (window_list.begin() != it) {
							auto window = *(--it);
							if (window&&window->is_point_in({ p.x ,p.y })) {
								window->set_point({ p.x ,p.y });
								break;
							}
						}
					}
				}
				if (keyRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					POINT p;
					GetCursorPos(&p);
					ScreenToClient(GetConsoleWindow(), &p);

					old = { p.x ,p.y };

					auto it = window_list.end();
					while (window_list.begin() != it) {
						auto window = *(--it);
						if (window&&window->is_point_in({ p.x ,p.y })) {
							window_list.erase(it);
							window_list.push_back(window);
							window->click();
							break;
						}
					}
				}
			}
		}
	}

	bool add(cwindow* window) {
		if (window) {
			window_list.push_back(window);
			return true;
		}
		return false;
	}
private:
	list<cwindow*> window_list;
	c_point old;
};


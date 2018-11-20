#include "stdafx.h"
#include "cmouse.h"


cmouse::cmouse()
{
}


cmouse::~cmouse()
{
}

void cmouse::input_mouse_event(INPUT_RECORD &mouse_event) {
	//if (mouse_event.EventType == KEY_EVENT)
	//{
	//	if (mouse_event.Event.KeyEvent.bKeyDown == 1)
	//	{
	//		if (mouse_event.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
	//		{

	//		}

	//		if (mouse_event.Event.KeyEvent.wVirtualKeyCode == VK_UP)
	//		{
	//		}

	//		if (mouse_event.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
	//		{
	//		}
	//	}

	//	if (mouse_event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
	//	{
	//	}

	//	if (mouse_event.Event.KeyEvent.bKeyDown)
	//	{
	//		//m_cmd += mouse_event.Event.KeyEvent.uChar.AsciiChar;
	//		//MessageBox(GetConsoleWindow(), "sfdsfs", "", 1);
	//	}
	//}
	//if (mouse_event.EventType == MOUSE_EVENT) {
	//	if (mouse_event.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
	//		if (mouse_event.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
	//			POINT p;
	//			GetCursorPos(&p);
	//			ScreenToClient(GetConsoleWindow(), &p);
	//			auto it = window_list.end();
	//			while (window_list.begin() != it) {
	//				auto window = *(--it);
	//				if (window&&window->is_point_in({ p.x ,p.y })) {
	//					window_list.erase(it);
	//					window_list.push_back(window);
	//					window->click();
	//					break;
	//				}
	//			}
	//		}
	//	}
	//	if (mouse_event.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
	//		POINT p;
	//		GetCursorPos(&p);
	//		ScreenToClient(GetConsoleWindow(), &p);
	//		auto it = window_list.end();
	//		while (window_list.begin() != it) {
	//			auto window = *(--it);
	//			if (window&&window->is_point_in({ p.x ,p.y })) {
	//				window_list.erase(it);
	//				window_list.push_back(window);
	//				window->click();
	//				break;
	//			}
	//		}
	//	}
	//}
}
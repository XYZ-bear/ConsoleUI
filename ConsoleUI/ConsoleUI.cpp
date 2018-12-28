// ConsoleUI.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cframe.h"
#include "cwindow.h"
#include "ccombox.h"
#include "ctimer.h"


int get_spin_ch_pos_in_text() {
	vector<string> v_str;
	string text = "abc\te\ta";
	cout << text << endl;
	split_with_ch(text, '\t', v_str);
	int start = 0;
	int index = 0;
	int x = 12;
	struct spin_info
	{
		int index;
		int pos;
		char ch;
	};
	spin_info info;
	for (auto &s : v_str) {
		if (s == "\t") {
			start += 3;
			if (start >= x) {
				info.index = index;
				info.pos = start;
				info.ch = text[info.index];
				return index;
			}
			index++;
		}
		else {

			if (start + s.size() >= x) {
				info.index = index + x - start - 1;
				info.pos = x;
				info.ch = text[info.index];
				return index + x - start - 1;
			}
			start += s.size();
			index += s.size();
		}
	}
	return index;
}

int main()
{
	//cout << get_spin_ch_pos_in_text() << endl;
	//getchar();
	//1 1 1 -1 -1 -1 -1 1 1
	//string s = "你好gdsfs";
	//int index = 0;
	//if (index <= s.size() / 2) {
	//	for (int i = index; i >= 0; i--) {
	//		if (s[i] > 0) {
	//			if ((index - i) % 2 == 0)
	//				index = index - 1;
	//			else
	//				cout << s.substr(index , 2);
	//			break;
	//		}
	//	}
	//}
	//else {
	//	for (int i = index; i <s.size(); i++) {
	//		if (s[i] > 0) {
	//			if ((index - i) % 2 == 0)
	//				cout << s.substr(index - 1, 2);
	//			else
	//				cout << s.substr(index, 2);
	//			break;
	//		}
	//	}
	//}
	////for (auto k : s)
	////	cout << (int)k << " ";
	//getchar();



	HANDLE hStdin;
	DWORD fdwSaveOldMode;

	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter = 0;

	// Get the standard input handle. 

	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	// Save the current input mode, to be restored on exit. 

	GetConsoleMode(hStdin, &fdwSaveOldMode);


	// Enable the window and mouse input events. 

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	SetConsoleMode(hStdin, fdwMode);
	//cgdi tt;// = nullptr;
	//tt.set_rng(800, 800);
	//tt.set_refer_point({ 0,0 });
	//tt.init();
	//for (int i = 0; i < 5000; i++) {
	//	cgdi sf;// = new cgdi();
	//	sf.set_rng(800, 800);
	//	sf.set_refer_point({ 0,0 });
	//	sf.init();
	//	sf.draw_frame_rect({ 20,20 }, { 500,600 }, 1, RGB(144, 144, 255));
	//	sf.draw_line({ 10,10 }, { 10,100 }, 1, RGB(255, 255, 255));
	//	BitBlt(tt.buffer_hdc_, sf.refer_c_point_.x, sf.refer_c_point_.y, sf.width_, sf.height_, sf.buffer_hdc_, 0, 0, SRCCOPY);
	//	sf.release();
	//}
	//tt.update();
	//getchar();
	//ccombox cbx;
	//cbx.get_root().text = "root";;
	//cbx.get_root()[0].text = "r1";
	//cbx.get_root()[0][0].text = "rr0";
	//cbx.get_root()[0][1].text = "rr1";

	//cout << cbx.get_root().text << endl;
	//cout << cbx.get_root()[0].text << endl;
	
	//cframe frame;

	//
	//auto windows = cframe::instance().get_children();
	//for (auto window : windows) {
	//	window->update();
	//	window->update_parent();
	//}
	//cframe::instance().get_gdi().update();
	//getchar();
	cframe::instance().init();
	cframe::instance().update(true);
	cframe::instance().run();
	return 0;
}
// ConsoleUI.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cframe.h"
#include "cwindow.h"
#include "ccombox.h"
#include "ctimer.h"

int main()
{
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
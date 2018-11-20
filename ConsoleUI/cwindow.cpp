#include "stdafx.h"
#include "cwindow.h"


cwindow::cwindow()
{
}


cwindow::~cwindow()
{
}

bool cwindow::update() {
	_gdi.draw_frame_rect({ 0,0 }, { _width ,_height }, 10, RGB(255, 0, 0));
	_gdi.draw_line({ 0,30 }, { _width,30}, 2, RGB(255, 144, 0));
	//_gdi.draw_text();
	return true;
}

bool cwindow::add_child(cwbase *child) {
	if (child) {
		chidren_list.push_back(child);
		return true;
	}
	return false;
}

bool cwindow::create(string title, c_point op, int width, int height) {
	title_ = title;
	return cwbase::create(op,width,height);
}

void cwindow::click() {
	//MessageBox(get_hand(), get_title().c_str(), "", 1);
}
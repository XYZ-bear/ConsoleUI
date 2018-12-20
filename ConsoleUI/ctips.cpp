#include "stdafx.h"
#include "ctips.h"

ctips::ctips()
{
	set_ctr_type(T_tips);
	set_enable_focus(false);
	set_is_show(false);
}


ctips::~ctips()
{
}

bool ctips::update()
{
	//if (!_is_show)
	//	return false;
	//erase_bk();
	//_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	//_gdi.draw_text(text_, { 2,2 }, _height - 4);
	return cwbase::update();
}

void ctips::show_tip(cwbase *bind_ob, c_point p) {
	if (bind_ob) {
		string str = bind_ob->get_tip();
		if (str.length() == 0) {
			set_is_show(false);
			return;
		}
		set_is_show(true);
		erase_bk();
		c_point point{ p.x,p.y+20 };
		set_size(point,str.length()*D_default_font_height/2+4, D_default_font_height+4);
		if (_right_bottom.x > _parent->get_width()) {
			point.x = _parent->get_width() - _width;
			set_point(point);
		}
		_gdi.fill_rect({ 0,0 }, { _width ,_height },RGB(210,210,210));
		_gdi.draw_text(str, { 4,2 }, D_default_font_height,RGB(100,100,100));
		cwbase::update();
	}
}

void ctips::click_in(c_point p) {
}

void ctips::click_out(c_point p) {
}

bool ctips::init() {
	set_is_show(false);
	return cwbase::init();
}

void ctips::double_click(c_point p) {

}

void ctips::mouse_move_in(c_point p) {
	set_point(p);
}

void ctips::mouse_move_out(c_point p) {
}
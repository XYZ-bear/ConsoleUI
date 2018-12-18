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
	//erase_bk();
	//_gdi.draw_frame_rect({ 0,0 }, { _width ,_height });
	//_gdi.draw_text(text_, { 2,2 }, _height - 4);
	return cwbase::update();
}

void ctips::show_tip(cwbase *bind_ob, c_point p) {
	if (is_show())
		return;
	if (bind_ob&&text_.find(bind_ob) != text_.end()) {
		set_is_show(true);
		erase_bk();
		c_point point{ p.x,p.y+20 };
		string str = text_[bind_ob];
		set_size(point,str.length()*D_default_font_height/2+4, D_default_font_height+4);
		if (_right_bottom.x > _parent->get_width()) {
			point.x = _parent->get_width() - _width;
			set_point(point);
		}
		_gdi.fill_rect({ 0,0 }, { _width ,_height },RGB(210,210,210));
		_gdi.draw_text(text_[bind_ob], { 4,2 }, D_default_font_height,RGB(100,100,100));
		cwbase::update();
	}
}

void ctips::add_tip(cwbase *bind_ob, string str) {
	text_[bind_ob] = str; 
}

void ctips::click_in(c_point p) {
}

void ctips::click_out(c_point p) {
}

bool ctips::init() {
	set_is_show(false);
	return true;
}

void ctips::double_click(c_point p) {

}

void ctips::mouse_move_in(c_point p) {
	set_point(p);
}

void ctips::mouse_move_out(c_point p) {
}
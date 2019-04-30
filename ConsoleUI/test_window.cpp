#include "stdafx.h"
#include "test_window.h"
#include "cbutton.h"
#include "cedit.h"
#include "ccombox.h"

testwindow::testwindow()
{
	set_ctr_type(T_window);
}


testwindow::~testwindow()
{

}

bool testwindow::init() {
	cbutton *close_button4 = new cbutton();
	close_button4->create({ 40,40 }, 15, 15, this, RGB(255, 215, 0));
	close_button4->set_type(T_rect_button);
	close_button4->add_cmd(this, T_click_in_event, &testwindow::message_click);
	close_button4->set_tip("tttt");

	mb = new cmessage_box();
	mb->create("afasf",{ 50,50 }, 200, 100,this);


	//ccombox *comb = new ccombox(this);
	//comb->get_root().text = "com1";
	//comb->get_root()[0].text = "root1";
	//comb->get_root()[1].text = "root2";
	//comb->get_root()[2].text = "root3";
	//comb->get_root()[3].text = "root4";
	//comb->create({ 65,65 }, 70,90, this,RGB(255, 0, 0));

	cedit *edit = new cedit();
	edit->set_font_height(16);
	edit->create({ 60,50 }, 500, 300, this, RGB(30, 30, 30));
	edit->set_style(T_multiline_edit);
	edit->set_tip("¹Øsdddddddddddddddddddddddddddddddddddddddd±Õ");

	//cscroll *scroll = new cscroll();
	//scroll->create({ 310,30 }, 12, 200, this);
	//scroll->set_bk_color(RGB(62,62,62));
	//scroll->set_bar_color(RGB(104,104,104));
	//scroll->set_bar_drag_color(RGB(200,200,200));
	//scroll->set_tip("¹ö¹ö¶´");

	//cscroll *scrollv = new cscroll();
	//scrollv->create({ 100,300 }, 200, 12, this);
	//scrollv->set_style(T_v_scroll);

	//tips.create({ 0,0 }, 100, 20, this);

	return cwindow::init();
}

bool testwindow::update(bool redraw) {
	return cwindow::update(redraw);
}

void testwindow::click_in(c_point p) {
	cwindow::click_in(p);
}

void testwindow::click_out(c_point p) {
	cwindow::click_out(p);
}

void testwindow::mouse_move(c_point p) {
	cwindow::mouse_move(p);
}

void testwindow::close_click(const void* p) {
	c_point *res = (c_point*)p;
	//MessageBox(GetConsoleWindow(), "close", "", 1);
	remove_self();
}

void testwindow::message_click(const void *p) {
	//mb->do_mode();
	set_timer(this, 17, &testwindow::draw);
}

void testwindow::double_click(c_point p) {
	cwindow::double_click(p);
}

void testwindow::draw() {
	update(true);
	_gdi.draw_frame_rect({ 40,40 }, { dl ,dl },1, c_color(RGB(255,0,0)));
	dl+=2;
	update_parent();
}

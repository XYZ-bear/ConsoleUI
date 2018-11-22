#include "stdafx.h"
#include "cgdi.h"

cgdi::cgdi()
{
}

cgdi::~cgdi()
{
}

void cgdi::init() {
	buffer_hdc_ = CreateCompatibleDC(NULL);
	hdc_ = GetDC(GetConsoleWindow());
	bmp_ = CreateCompatibleBitmap(hdc_, width_, height_);
	SelectObject(buffer_hdc_, bmp_);
}

void cgdi::draw_line(c_point p1, c_point p2, int width, COLORREF color, int style) {
	hpen_ = CreatePen(style, width, color);//添加画笔属性。d代表宽度,a,b,c用于调颜色
	SelectObject(buffer_hdc_, hpen_);//画笔绑定到句柄
	MoveToEx(buffer_hdc_, p1.x, p1.y, NULL);
	LineTo(buffer_hdc_, p2.x, p2.y);
	DeleteObject(hpen_);
}

void cgdi::draw_retangle(c_point p1, c_point p2, int width, COLORREF color, int style) {
	hpen_ = CreatePen(style, width, color);//添加画笔属性。d代表宽度,a,b,c用于调颜色
	SelectObject(buffer_hdc_, hpen_);//画笔绑定到句柄
	Rectangle(buffer_hdc_, p1.x, p1.y, p2.x, p2.y);
	DeleteObject(hpen_);
}

void cgdi::draw_frame_rect(c_point p1, c_point p2, int width, COLORREF color, int style) {
	hpen_ = CreatePen(style, width, color);//添加画笔属性。d代表宽度,a,b,c用于调颜色
	open_ = (HPEN)SelectObject(hdc_, hpen_);
	RECT rect{ p1.x,p1 .y,p2.x,p2.y};
	FrameRect(buffer_hdc_, &rect,CreateSolidBrush(color));
	SelectObject(hdc_, open_);
	DeleteObject(hpen_);
}

void cgdi::fill_rect(c_point p1, c_point p2, int width, COLORREF color) {
	RECT rect{ p1.x,p1.y,p2.x,p2.y };
	FillRect(buffer_hdc_,&rect, CreateSolidBrush(color));
}

void cgdi::draw_text() {
	//DrawText()
	RECT rect{ 100, 100, 200, 300 };


	HFONT hFont;

	hFont = CreateFont(10, 10, 0, 0, FW_THIN, true, false, false,
		CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
		FF_MODERN, "宋体");

	SelectObject(buffer_hdc_, hFont);

	DrawText(buffer_hdc_, "测试", 10, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	DeleteObject(hFont);
}

void cgdi::set_refer_point(c_point rp) { refer_c_point_ = rp; }

void cgdi::set_rng(int width, int height) {
	width_ = width;
	height_ = height;
}

void cgdi::update() {
	BitBlt(hdc_, refer_c_point_.x, refer_c_point_.y, width_, height_,buffer_hdc_, 0, 0, SRCCOPY);
}
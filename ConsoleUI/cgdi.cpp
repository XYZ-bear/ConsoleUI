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
	open_ = (HPEN)SelectObject(buffer_hdc_, hpen_);
	MoveToEx(buffer_hdc_, p1.x, p1.y, NULL);
	LineTo(buffer_hdc_, p2.x, p2.y);
	SelectObject(hdc_, open_);
	DeleteObject(hpen_);
}

void cgdi::draw_retangle(c_point p1, c_point p2, int width, COLORREF color, int style) {
	hpen_ = CreatePen(style, width, color);//添加画笔属性。d代表宽度,a,b,c用于调颜色
	open_ = (HPEN)SelectObject(buffer_hdc_, hpen_);
	Rectangle(buffer_hdc_, p1.x, p1.y, p2.x, p2.y);
	SelectObject(hdc_, open_);
	DeleteObject(hpen_);
}

void cgdi::draw_frame_rect(c_point p1, c_point p2, int width, COLORREF color, int style) {
	hpen_ = CreatePen(style, width, color);//添加画笔属性。d代表宽度,a,b,c用于调颜色
	open_ = (HPEN)SelectObject(buffer_hdc_, hpen_);
	RECT rect{ p1.x,p1 .y,p2.x,p2.y};
	HBRUSH br = CreateSolidBrush(color);
	FrameRect(buffer_hdc_, &rect,br);
	SelectObject(hdc_, open_);
	DeleteObject(hpen_);
	DeleteObject(br);
}

void cgdi::fill_rect(c_point p1, c_point p2, int width, COLORREF color) {
	RECT rect{ p1.x,p1.y,p2.x,p2.y };
	HBRUSH br = CreateSolidBrush(color);
	SelectObject(buffer_hdc_,br);
	FillRect(buffer_hdc_,&rect, br);
	DeleteObject(br);
}

void cgdi::draw_ellipse(c_point p, int len, COLORREF color) {
	HBRUSH hBrush = CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(buffer_hdc_, hBrush);

	HPEN hPen = CreatePen(PS_SOLID, 0, color);
	HPEN hOldPen = (HPEN)SelectObject(buffer_hdc_, hPen);

	Ellipse(buffer_hdc_, p.x, p.y, p.x + len , p.y + len);

	SelectObject(buffer_hdc_, hOldBrush);
	DeleteObject(hBrush);

	SelectObject(buffer_hdc_, hOldPen);
	DeleteObject(hPen);
}

void cgdi::draw_text(string str, c_point p, int height, COLORREF color) {
	RECT rect{ p.x, p.y, p.x + 8 * str.length(), p.y + height };

	SetBkMode(buffer_hdc_, TRANSPARENT);
	SetTextColor(buffer_hdc_, color);
	HFONT hFont;

	hFont = CreateFont(height, 0, 0, 0, FW_DONTCARE, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FF_DONTCARE, "新宋体");//Microsoft Yahei UI

	SelectObject(buffer_hdc_, hFont); 
	DrawText(buffer_hdc_, str.c_str(), str.length(), &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	DeleteObject(hFont);
}

void cgdi::set_refer_point(c_point rp) { refer_c_point_ = rp; }

void cgdi::set_rng(int width, int height) {
	width_ = width;
	height_ = height;
	bmp_ = CreateCompatibleBitmap(hdc_, width_, height_);
	SelectObject(buffer_hdc_, bmp_);
	
}

void cgdi::update() {
	BitBlt(hdc_, refer_c_point_.x, refer_c_point_.y, width_, height_,buffer_hdc_, 0, 0, SRCCOPY);
}
#include "stdafx.h"
#include "cgdi.h"

cgdi::cgdi()
{
}

cgdi::~cgdi()
{
}
HDC cgdi::hdc_ = GetDC(GetConsoleWindow());
void cgdi::init() {
	buffer_hdc_ = CreateCompatibleDC(NULL);
	bmp_ = CreateCompatibleBitmap(hdc_, width_, height_); 
	SelectObject(buffer_hdc_, bmp_);
}

void cgdi::draw_line(c_point p1, c_point p2, int width, COLORREF color, int style) {
	HPEN hpen_ = CreatePen(style, width, color);//添加画笔属性。d代表宽度,a,b,c用于调颜色
	HPEN open_ = (HPEN)SelectObject(buffer_hdc_, hpen_);
	MoveToEx(buffer_hdc_, p1.x, p1.y, NULL);
	LineTo(buffer_hdc_, p2.x, p2.y);
	SelectObject(buffer_hdc_, open_);
	DeleteObject(hpen_);
}

void cgdi::draw_retangle(c_point p1, c_point p2, int width, COLORREF color, int style) {
	HPEN hpen_ = CreatePen(style, width, color);//添加画笔属性。d代表宽度,a,b,c用于调颜色
	HPEN open_ = (HPEN)SelectObject(buffer_hdc_, hpen_);
	Rectangle(buffer_hdc_, p1.x, p1.y, p2.x, p2.y);
	SelectObject(buffer_hdc_, open_);
	DeleteObject(hpen_);
}

void cgdi::draw_frame_rect(c_point p1, c_point p2, int width, COLORREF color, int style) {
	HPEN hpen_ = CreatePen(style, width, color);//添加画笔属性。d代表宽度,a,b,c用于调颜色
	HPEN open_ = (HPEN)SelectObject(buffer_hdc_, hpen_);
	RECT rect{ p1.x,p1 .y,p2.x,p2.y};
	HBRUSH br = CreateSolidBrush(color);
	FrameRect(buffer_hdc_, &rect,br);
	SelectObject(buffer_hdc_, open_);
	DeleteObject(hpen_);
	DeleteObject(br);
}

void cgdi::fill_rect(c_point p1, c_point p2, COLORREF color) {
	RECT rect{ p1.x,p1.y,p2.x,p2.y };
	HBRUSH br = CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(buffer_hdc_, br);
	FillRect(buffer_hdc_,&rect, br);
	SelectObject(buffer_hdc_, hOldBrush);
	DeleteObject(hOldBrush);
	DeleteObject(br);
}

void cgdi::fill_rect(c_rect r1, COLORREF color) {
	RECT rect{ r1.p.x,r1.p.y,r1.p.x + r1.width,r1.p.y + r1.height };
	HBRUSH br = CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(buffer_hdc_, br);
	FillRect(buffer_hdc_, &rect, br);
	SelectObject(buffer_hdc_, hOldBrush);
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
	RECT rect{ p.x, p.y, p.x + height/2 * str.length(), p.y + height };

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
	set_change(true);
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

void cgdi::set_change(bool is) {
	is_change_ = is;
}

bool cgdi::get_change() {
	return is_change_;
}

void cgdi::release() {
	//DeleteObject(bmp_);
	//DeleteObject(hdc_);
}
#pragma once
class cgdi
{
public:
	cgdi();
	~cgdi();
	static cgdi& instance() {
		static cgdi gdi;
		return gdi;
	}
	void draw_line(c_point p1, c_point p2, int width = 1, COLORREF color = RGB(255,255,255),int style = PS_SOLID);
	void draw_retangle(c_point p1, c_point p2, int width = 1, COLORREF color = RGB(255, 255, 255), int style = PS_SOLID);
	void draw_frame_rect(c_point p1, c_point p2, int width = 1, COLORREF color = RGB(255, 255, 255), int style = PS_SOLID);
	void draw_text(string str, c_point p, int height = D_default_font_height, COLORREF color = RGB(255, 255, 255));
	void fill_rect(c_point p1, c_point p2, COLORREF color = RGB(255, 255, 255));
	void fill_rect(c_rect r1, COLORREF color = RGB(255, 255, 255));
	void draw_ellipse(c_point p1, int len, COLORREF color = RGB(255, 255, 255));
	void set_refer_point(c_point rp);
	void update();
	void init();
	void set_rng(int width, int height);
	void set_change(bool is);
	bool get_change();
	HDC buffer_hdc_;
	c_point refer_c_point_;
	int width_;
	int height_;
private:
	HDC hdc_;
	
	HBITMAP bmp_;
	HPEN hpen_;
	HPEN open_;

	bool is_change_;
};


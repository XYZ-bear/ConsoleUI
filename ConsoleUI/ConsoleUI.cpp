// ConsoleUI.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cframe.h"
#include "cwindow.h"
#include "ccombox.h"

int main()
{
	//ccombox cbx;
	//cbx.get_root().text = "root";;
	//cbx.get_root()[0].text = "r1";
	//cbx.get_root()[0][0].text = "rr0";
	//cbx.get_root()[0][1].text = "rr1";

	//cout << cbx.get_root().text << endl;
	//cout << cbx.get_root()[0].text << endl;
	
	//cframe frame;
	cwindow window;
	window.create("windows1",{10,20},600,300);

	cwindow window2;
	window2.create("windows2",{ 100,60 }, 300, 200);

	cwindow window3;
	window3.create("windows3", { 400,60 }, 300, 200);

	cframe::instance().add(&window);
	cframe::instance().add(&window2);
	cframe::instance().add(&window3);
	cframe::instance().run();
	return 0;
}
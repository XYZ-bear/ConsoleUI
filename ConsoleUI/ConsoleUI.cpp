// ConsoleUI.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cframe.h"
#include "cwindow.h"

int main()
{
	//cframe frame;
	cwindow window;
	window.create("windows1",{10,20},600,300);

	cwindow window2;
	window2.create("windows2",{ 100,60 }, 300, 200);

	cframe::instance().add(&window);
	cframe::instance().add(&window2);
	cframe::instance().run();
	return 0;
}
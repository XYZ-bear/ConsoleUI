// ConsoleUI.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cframe.h"
#include "cwindow.h"

int main()
{
	cframe frame;
	cwindow window;
	window.create("a",{10,20},600,300);

	cwindow window2;
	window2.create("b",{ 100,60 }, 300, 200);

	frame.add(&window);
	frame.add(&window2);
	frame.run();
	return 0;
}
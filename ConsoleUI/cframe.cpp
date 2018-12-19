#include "stdafx.h"
#include "cframe.h"


cframe::cframe()
{
	set_ctr_type(T_frame);
	create({ 0,0 }, get_console_width(), get_console_height());
}


cframe::~cframe()
{
}

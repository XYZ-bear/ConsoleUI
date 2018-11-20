#pragma once
class cmouse
{
public:
	cmouse();
	~cmouse();
	void input_mouse_event(INPUT_RECORD &keyRec);
};


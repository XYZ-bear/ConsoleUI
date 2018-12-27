#pragma once
#include "stdafx.h"
#include "common.h"

void split_with_ch(string &str, char ch, vector<string> &vc) {
	string res;

	int index = 0;
	for (auto &s : str) {
		if (s == ch) {
			if (res != "") {
				vc.push_back(res);
				res = "";
			}
			vc.push_back(string{ ch });
		}
		else
			res += s;
		if (index == str.size() - 1)
			vc.push_back(res);
		index++;
	}
}

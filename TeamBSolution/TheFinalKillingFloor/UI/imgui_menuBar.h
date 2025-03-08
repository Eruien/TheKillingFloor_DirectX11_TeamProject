#pragma once
#include "LStd.h"

class imgui_menuBar
{
public:
	void Init();
	void Frame();
	void Render();
	void Test();

private:
	wstring saveFileName;
	bool show_demo_window = true;
	bool show_another_window = false;
	TVector4 clear_color = TVector4(0.f);
	bool _isSaveWindow = false;

};


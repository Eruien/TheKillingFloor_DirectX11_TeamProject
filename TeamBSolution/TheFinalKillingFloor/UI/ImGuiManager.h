#pragma once
#include "LStd.h"

#ifdef _DEBUG
#pragma comment(lib, "Imgui_d.lib")
#else
#pragma comment(lib, "Imgui.lib")
#endif // _DEBUG

class ImGuiManager
{
public:
	void Init();
	void Frame();
	void Render();

};


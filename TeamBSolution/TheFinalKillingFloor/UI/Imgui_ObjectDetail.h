#pragma once
#include "LStd.h"
class Imgui_ObjectDetail
{
public:
	void Init();
	void Frame();
	void Render();
	void Test();
	static bool _isDialogWindow;
private:
	wstring _filePathName;
	ID3D11ShaderResourceView* my_texture = nullptr;
	bool _isTexAcceptWindow = false;
	
};


#pragma once
#include "LStd.h"
#include "KObject.h"
//
#include "DigitDisplay.h"
#include "PickingUI.h"
#include "DragUI.h"
#include "Resize2D.h"
#include "Animator.h"
#include "ChangeTexture.h"
#include "ExitWindow.h"
#include "TextureList.h"
#include "BillBoard.h"
//
#include "imgui_menuBar.h"
#include "ImGuiManager.h"
#include "Imgui_ObjectDetail.h"

class UIManager
{
public:
    static UIManager& GetInstance()
    {
        static UIManager instance;
        return instance;
    }

    // 프라이빗 생성자

    
  
    void Init();
    void Frame();
    void Render();

    void Save(const wstring filePath);
    void Load(const wstring filePath);

    void AddUIObject(shared_ptr<KObject> obj) { _objs.push_back(obj); };
    shared_ptr<KObject> GetUIObject(wstring name);

    	static bool s_isMouseInImGuiWindow;
        static shared_ptr<KObject> s_selectedObject;

private:
    vector<shared_ptr<KObject>> _objs;
    UIManager();
    ~UIManager();
    shared_ptr< Imgui_ObjectDetail> _imGuiObjDetail;
    shared_ptr< imgui_menuBar> _imgui_menuBar;
    shared_ptr<ImGuiManager> _imGuiManager;
   
};


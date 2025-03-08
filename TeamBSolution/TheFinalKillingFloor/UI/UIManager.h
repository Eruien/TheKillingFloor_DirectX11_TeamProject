#pragma once
#include "LStd.h"
#include "KObject.h"
#include "LGlobal.h"
#include "../LPlayer.h"
#include "../LScene.h"
#include "LInput.h"
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
#include "ButtonAction.h"
#include "Text.h"
#include "HpBar.h"
#include "UIEvent.h"
#include "TextToTexture.h"
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
    void Init(ComPtr<ID3D11DepthStencilState> Depth, ComPtr<ID3D11DepthStencilState> Disable);
    void Frame();
    void Render();

    void Save(const wstring filePath);
    void Load(const wstring filePath);

    void AddUIObject(shared_ptr<KObject> obj) { _objs.push_back(obj); };
    shared_ptr<KObject> GetUIObject(wstring name);
    vector<shared_ptr<KObject>>& GetUIObjects() { return _objs; };
    vector<shared_ptr<KObject>> GetGroup(wstring groupName);
    vector<shared_ptr<KObject>> GetSceneObj(wstring sceneName);
    void ChangeScene(Event Scene);
    void RemoveObject(wstring name);
    void UpdateResolution(int width, int height);
    	static bool s_isMouseInImGuiWindow;
        static shared_ptr<KObject> s_selectedObject;
        bool _editMode = false;
        int _beforeHeight= LGlobal::g_WindowHeight;
        int _beforeWidth= LGlobal::g_WindowWidth;
        
private:
    vector<shared_ptr<KObject>> _objs;
    vector<shared_ptr<KObject>> _objsTemp;
    UIManager();
    ~UIManager();
    shared_ptr< Imgui_ObjectDetail> _imGuiObjDetail;
    shared_ptr< imgui_menuBar> _imgui_menuBar;
    shared_ptr<ImGuiManager> _imGuiManager;
    ComPtr<ID3D11DepthStencilState> _DepthStencilState;
    ComPtr<ID3D11DepthStencilState> _DepthStencilStateDisable;
 

};


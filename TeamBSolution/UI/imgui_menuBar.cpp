
#include "imgui_menuBar.h"
#include "LGlobal.h"
#include "UIManager.h"
void imgui_menuBar::Init()
{

}

void imgui_menuBar::Frame()
{
  


    //
    Test();
}

void imgui_menuBar::Render()
{

    if (_isSaveWindow)
    {
        ImGui::OpenPopup("Save?");
        if (ImGui::BeginPopupModal("Save?",0, ImGuiWindowFlags_AlwaysAutoResize))
        {

            if (ImGui::Button("OK", ImVec2(100.f, 25.f)))
            {
                _isSaveWindow = false;
                UIManager::GetInstance().Save(L"testScene.xml");
                ImGui::CloseCurrentPopup();
            }


            ImGui::SameLine(0,30);
           
            if (ImGui::Button("Cancle", ImVec2(100.f, 25.f)))
            {
                _isSaveWindow = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }


    // 파일 다이얼로그 창
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }
    // 파일 다이얼로그 창
}

void imgui_menuBar::Test()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
     //if (show_demo_window)
     //   ImGui::ShowDemoWindow(&show_demo_window);

    if (ImGui::BeginMainMenuBar())
    {
  
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("Open File Dialog"))
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".h,.XML", ".");


            if (ImGui::MenuItem("Save"))
            {
                _isSaveWindow = true;
            }


             ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();

        }
        if (ImGui::BeginMenu("New"))
        {
            if (ImGui::MenuItem("NewObject", "CTRL+N"))
            {
                shared_ptr<KObject> obj = make_shared<KObject>();
                obj->Init();
                obj->AddScripts(make_shared<PickingUI>());
                obj->AddScripts(make_shared<Resize2D>());
                obj->AddScripts(make_shared<DragUI>());
                obj->AddScripts(make_shared<ChangeTexture>());
                obj->GetScript<DragUI>(L"DragUI")->Init();
                obj->SetPos({ 0,0,0 });
                obj->SetScale({ 200,200,0 });
                obj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/1.png");
                UIManager::GetInstance().AddUIObject(obj);
            }
          
            ImGui::EndMenu();

        }
    }
    ImGui::EndMainMenuBar();

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;

    //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //    ImGui::Checkbox("Another Window", &show_another_window);

    //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);


    //   
    //  //  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //    ImGui::End();
    //}

    // 3. Show another simple window.
    //if (show_another_window)
    //{
    //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //    ImGui::Text("Hello from another window!");
    //    if (ImGui::Button("Close Me"))
    //        show_another_window = false;
    //    ImGui::End();
    //}
}

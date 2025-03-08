
#include "ImGuiManager.h"
#include "LGlobal.h"

void ImGuiManager::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // 
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(LGlobal::g_hWnd);
    ImGui_ImplDX11_Init(LGlobal::g_pDevice.Get(), LGlobal::g_pImmediateContext.Get());
}

void ImGuiManager::Frame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Render()
{

    ImGui::Render();
    /*const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    LGlobal::g_pImmediateContext.Get()->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    LGlobal::g_pImmediateContext.Get()->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);*/
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

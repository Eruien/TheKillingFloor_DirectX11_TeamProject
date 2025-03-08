//#include "KObject.h"
//#include "ImguiDetail.h"
//#include "PickingUI.h"
//#include "ChangeTexture.h"
//#include "LGlobal.h"
//#include <WICTextureLoader.h>
//#pragma comment(lib, "DirectXTK_D.lib")
//ImguiDetail::ImguiDetail() : MonoBehaviour(L"ImguiDetail")
//{
//}
//
//ImguiDetail::~ImguiDetail()
//{
//}
//
//void ImguiDetail::Init()
//{
//}
//
//void ImguiDetail::Frame()
//{
//
//	// 오브젝트가 선택되었을 때
//	if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetIsSelected())
//	{
//
//		ImGui::Begin("Object Details");
//		char buffer[256] = "";  // 문자열을 저장할 버퍼
//
//		// 원래 가지고 있던 이름 표시
//		ImGui::Text("Name: %s", GetGameObject()->GetName().c_str());
//		ImGui::Text("Change Name: ");
//		ImGui::SameLine(0, -1);
//		ImGui::SetNextItemWidth(100);
//		if (ImGui::InputText("##ObjectName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
//		{
//			// 엔터 키가 눌렸을 때 실행되는 코드
//			// buffer에 사용자의 입력이 저장됨
//			// 변경된 값을 사용하여 원하는 동작을 수행하세요
//			std::string userInput = buffer;
//			GetGameObject()->SetName(userInput.c_str());  // SetName 함수에 사용자의 입력을 전달
//		}
//		ImGui::Separator();
//
//		ImGui::Text("Position");
//
//		ImGui::Text("X");
//		ImGui::SameLine();
//		ImGui::SetNextItemWidth(100);
//		ImGui::InputFloat("##PositionX", &GetGameObject()->m_vPosition.x);
//
//		ImGui::SameLine();
//		ImGui::Text("Y");
//		ImGui::SameLine();
//		ImGui::SetNextItemWidth(100);
//		ImGui::InputFloat("##PositionY", &GetGameObject()->m_vPosition.y);
//
//		ImGui::SameLine();
//		ImGui::Text("Z");
//		ImGui::SameLine();
//		ImGui::SetNextItemWidth(100);
//		ImGui::InputFloat("##PositionZ", &GetGameObject()->m_vPosition.z);
//
//		ImGui::Separator();
//
//		ImGui::Text("Scale");
//
//		ImGui::Text("Width");
//		ImGui::SameLine();
//		ImGui::SetNextItemWidth(100);
//		ImGui::InputFloat("##ScaleX", &GetGameObject()->m_vScale.x);
//
//		ImGui::SameLine();
//		ImGui::Text("Height");
//		ImGui::SameLine();
//		ImGui::SetNextItemWidth(100);
//		ImGui::InputFloat("##ScaleY", &GetGameObject()->m_vScale.y);
//
//		ImGui::Separator();
//
//		//ImGui::Text();
//		if (ImGui::Button("Load Texture From File"))
//		{
//			ImGuiFileDialog::Instance()->OpenDialog("ChooseTexture", "Choose Texture File", ".png", ".");
//
//		}
//
//
//
//		ImGui::End();
//
//		if (ImGuiFileDialog::Instance()->Display("ChooseTexture"))
//		{
//			// action if OK
//			if (ImGuiFileDialog::Instance()->IsOk())
//			{
//				
//				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
//				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
//				// action
//				std::wstring str = wstring().assign(filePathName.begin(), filePathName.end());
//
//				_filePathName = str;
//
//
//
//
//				HRESULT hr = CreateWICTextureFromFile(LGlobal::g_pDevice.Get(), LGlobal::g_pImmediateContext.Get(), _filePathName.c_str(), nullptr, &my_texture);
//				_isTexAcceptWindow = true;
//				if (FAILED(hr))
//				{
//					MessageBoxA(NULL, "CreateWICTextureFromFile Error", "Error Box", MB_OK);
//				}
//
//			}
//
//			// close
//		
//			ImGuiFileDialog::Instance()->Close();
//		}
//
//		if (my_texture && _isTexAcceptWindow)
//		{
//			ImGui::Image((void*)my_texture, ImVec2(256, 256));
//			if (ImGui::Button("Accept"))
//			{
//				GetGameObject()->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(_filePathName);
//				_isTexAcceptWindow = false;
//			}
//			if (ImGui::Button("Cancle"))
//			{
//				_isTexAcceptWindow = false;
//
//				//창닫기
//			}
//		}
//	}
//
//
//
//
//	
//}
//
//

#include "ModalWindow.h"
#include "KObject.h"
ModalWindow::ModalWindow() : MonoBehaviour(L"ModalWindow")
{
}

ModalWindow::~ModalWindow()
{
}

void ModalWindow::Init()
{
}

void ModalWindow::Frame()
{
	for (auto child : _child)
	{
		
		child->m_matWorld = child->m_matWorld * GetGameObject()->m_matWorld;
	}
}

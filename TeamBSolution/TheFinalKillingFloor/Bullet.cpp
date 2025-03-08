#include "Bullet.h"


bool Bullet::Frame() 
{
	LModel::Frame();
	if (bTarget)
	{
		TVector3 targetDir = target->GetPosition();
		targetDir.y += target->m_OBBBox.fTall * 0.95f;
		TVector3 addDir = targetDir - this->GetPosition();
		float dist = 50 / addDir.Length();
		addDir.Normalize();
		addDir *= dist;
		m_Forward += addDir;
		m_Forward.Normalize();
		m_matControl.Forward(m_Forward * 0.05f);
	}
	//if (LGlobal::g_PlayerModel->IsZedTime)
	//{
	//	m_matControl._41 += m_Forward.x * 1.f;
	//	m_matControl._42 += m_Forward.y * 1.f;// -LGlobal::g_fSPF * 30.f;
	//	m_matControl._43 += m_Forward.z * 1.f;
	//	return true;
	//}
	m_matControl._41 += m_Forward.x * 20.f;
	m_matControl._42 += m_Forward.y * 20.f;// -LGlobal::g_fSPF * 30.f;
	m_matControl._43 += m_Forward.z * 20.f;
	return true;
}
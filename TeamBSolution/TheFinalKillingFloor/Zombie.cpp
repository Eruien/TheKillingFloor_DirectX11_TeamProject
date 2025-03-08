#include "Zombie.h"
#include "string"
#include "LWrite.h"
#include "LGlobal.h"
#include "EnemyPatrol.h"
#include "EnemyTrace.h"
#include "EnemyDeath.h"
#include "EnemyTakeDamage.h"
#include "EnemyAttack.h"
#include "KObject.h"
#include "UIManager.h"

void Zombie::FSM(FSMType fsmType)
{
	auto iter = LFSMMgr::GetInstance().m_map.find(fsmType);

	if (LFSMMgr::GetInstance().m_map.end() == iter)
	{
		MessageBoxA(NULL, "FSMType Error", "Error Box", MB_OK);
		return;
	}

	m_pFsm = iter->second.get();

	m_pActionList.insert(std::make_pair(State::ENEMYPATROL, std::make_unique<EnemyPatrol>(this)));
	m_pActionList.insert(std::make_pair(State::ENEMYTRACE, std::make_unique<EnemyTrace>(this)));
	m_pActionList.insert(std::make_pair(State::ENEMYTAKEDAMAGE, std::make_unique<EnemyTakeDamage>(this)));
	m_pActionList.insert(std::make_pair(State::ENEMYDEATH, std::make_unique<EnemyDeath>(this)));
	m_pActionList.insert(std::make_pair(State::ENEMYATTACK, std::make_unique<EnemyAttack>(this)));

	m_pAction = m_pActionList.find(State::ENEMYPATROL)->second.get();
	m_CurrentState = State::ENEMYPATROL;
}

void Zombie::SetTransition(Event inputEvent)
{
	m_CurrentState = m_pFsm->StateTransition(m_CurrentState, inputEvent);
	m_pAction = m_pActionList.find(m_CurrentState)->second.get();
}

State Zombie::GetState()
{
	return m_CurrentState;
}

void Zombie::Process()
{
	m_pAction->Process();
}

void Zombie::Move(TVector3 target)
{
	if (IsOnAir)
	{
		return;
	}
	m_Dir = target - TVector3(m_matControl._41, m_matControl._42, m_matControl._43);
	m_Dir.Normalize();
	TVector3 forward = m_matControl.Forward();

	float dirX = m_Dir.x;
	float dirZ = m_Dir.z;

	DirectX::XMVECTOR gRotation;
	DirectX::XMMATRIX matRotation;

	float yawRadians = atan2(dirZ, dirX);
	gRotation = DirectX::XMQuaternionRotationRollPitchYaw(0, -yawRadians - 1.5708, 0);
	DirectX::XMVECTOR xmPos = DirectX::XMVectorSet(m_matControl._41, m_matControl._42, m_matControl._43, 1.0f);
	matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmPos);

	TMatrix zombieScale;
	D3DXMatrixScaling(&zombieScale, 0.2f, 0.2f, 0.2f);
	TMatrix zombiePos = zombieScale * matRotation;

	m_matControl = zombiePos;
	m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_Dir.x;
	m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_Dir.z;
}

void Zombie::MoveInstancing(TVector3 target)
{
	auto fbxMeshList = m_pModel->m_DrawList;
	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		for (int i = 0; i < fbxMeshList[iSub]->m_InstanceSize; i++)
		{
			TVector3 pos = { fbxMeshList[iSub]->m_matInstanceList.mat[i]._41,
			fbxMeshList[iSub]->m_matInstanceList.mat[i]._42, fbxMeshList[iSub]->m_matInstanceList.mat[i]._43 };
			m_Dir = target - pos;
			m_Dir.Normalize();
			TVector3 forward = fbxMeshList[iSub]->m_matInstanceList.mat[i].Forward();

			float dirX = m_Dir.x;
			float dirZ = m_Dir.z;

			DirectX::XMVECTOR gRotation;
			DirectX::XMMATRIX matRotation;

			float yawRadians = atan2(dirZ, dirX);
			gRotation = DirectX::XMQuaternionRotationRollPitchYaw(0, -yawRadians - 1.5708, 0);
			DirectX::XMVECTOR xmPos = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
			matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmPos);

			fbxMeshList[iSub]->m_matInstanceList.mat[i] = matRotation;
			fbxMeshList[iSub]->m_matInstanceList.mat[i]._41 += m_Speed * LGlobal::g_fSPF * m_Dir.x;
			fbxMeshList[iSub]->m_matInstanceList.mat[i]._43 += m_Speed * LGlobal::g_fSPF * m_Dir.z;
		}
	}
}

int Zombie::GetRandomNumber()
{
	return m_Distribution(m_Generator);
}
bool Zombie::Frame()
{
	//if (LGlobal::g_PlayerModel->IsSteamPack)
	//{
	//	m_AnimationRate = 0.5f;
	//	m_Speed = 25.0f;
	//}
	//else
	//{
	//	m_AnimationRate = 1.0f;
	//	m_Speed = 50.0f;
	//}

	if (LGlobal::g_PlayerModel->IsZedTime)
	{
		m_AnimationRate = 1.f;
		m_Speed = 30.0f;
	}
	else
	{
		m_AnimationRate = 2.0f;
		m_Speed = 60.0f;
	}

	m_ZombieSound->Play();
	// ºôº¸µå

	if (m_HP != 100)
	{
		m_enemyHp->SetPos({ m_matControl._41,m_matControl._42 + 40, m_matControl._43 });

		/*  LWRITE.AddText(to_wstring(LGlobal::g_PlayerModel->m_matControl._41), 100, 400);
		 LWRITE.AddText(to_wstring(LGlobal::g_PlayerModel->m_matControl._41), 100, 500);*/
		TMatrix matRotation, matTrans, matScale, worldMat;
		D3DXMatrixInverse(&matRotation, nullptr, &LGlobal::g_pMainCamera->m_matView);
		matRotation._41 = 0.0f;
		matRotation._42 = 0.0f;
		matRotation._43 = 0.0f;
		matRotation._44 = 1.0f;

		D3DXMatrixTranslation(&matTrans, m_enemyHp->m_vPosition.x,
			m_enemyHp->m_vPosition.y,
			m_enemyHp->m_vPosition.z
		);

		D3DXMatrixScaling(&matScale, m_enemyHp->m_vScale.x,
			m_enemyHp->m_vScale.y,
			m_enemyHp->m_vScale.z
		);
		worldMat = matScale * matRotation * matTrans;
		m_enemyHp->SetMatrix(&worldMat, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
		m_enemyHp->Frame();
		// ºôº¸µå ³¡
	}


	if (m_HP <= 0)
	{
		LSoundMgr::GetInstance().GetPtr(L"killsound.mp3")->PlayEffect();
		LGlobal::g_PlayerModel->m_Money += 100;
		UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
		IsDead = true;
	}

	LSkinningModel::AniFrame();

	m_PlayerPos = { m_Player->m_matControl._41, m_Player->m_matControl._42, m_Player->m_matControl._43 };
	m_NPCPos = { m_matControl._41, m_matControl._42, m_matControl._43 };

	if (((m_NPCPos.x - m_AttackRange) < m_PlayerPos.x) && (m_PlayerPos.x < (m_NPCPos.x + m_AttackRange))
		&& ((m_NPCPos.z - m_AttackRange) < m_PlayerPos.z) && (m_PlayerPos.z < (m_NPCPos.z + m_AttackRange)))
	{
		IsAttackRange = true;
	}
	else
	{
		IsAttackRange = false;
	}

	if (((m_NPCPos.x - m_PatrolRange) < m_PlayerPos.x) && (m_PlayerPos.x < (m_NPCPos.x + m_PatrolRange))
		&& ((m_NPCPos.z - m_PatrolRange) < m_PlayerPos.z) && (m_PlayerPos.z < (m_NPCPos.z + m_PatrolRange)))
	{
		IsFindPlayer = true;
	}

	if (((m_RandomPos.x - 50) < m_NPCPos.x) && (m_NPCPos.x < (m_RandomPos.x + 50)) &&
		((m_RandomPos.z - 50) < m_NPCPos.z) && (m_NPCPos.z < (m_RandomPos.z + 50)))
	{
		IsEndPatrol = true;
	}

	if (IsEndPatrol)
	{
		m_RandomPos = { float(GetRandomNumber()), 0.0f, float(GetRandomNumber()) };
		IsEndPatrol = false;
	}

	return true;
}

bool Zombie::FrameInstancing()
{
	auto fbxMeshList = m_pModel->m_DrawList;
	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		for (int i = 0; i < fbxMeshList[iSub]->m_InstanceSize; i++)
		{
			m_PlayerPos = { m_Player->m_matControl._41, m_Player->m_matControl._42, m_Player->m_matControl._43 };
			m_NPCPos = { fbxMeshList[iSub]->m_matInstanceList.mat[i]._41,
			fbxMeshList[iSub]->m_matInstanceList.mat[i]._42, fbxMeshList[iSub]->m_matInstanceList.mat[i]._43 };
		}
	}

	if (((m_NPCPos.x - m_PatrolRange) < m_PlayerPos.x) && (m_PlayerPos.x < (m_NPCPos.x + m_PatrolRange))
		&& ((m_NPCPos.z - m_PatrolRange) < m_PlayerPos.z) && (m_PlayerPos.z < (m_NPCPos.z + m_PatrolRange)))
	{
		IsFindPlayer = true;
	}

	if (((m_RandomPos.x - 50) < m_NPCPos.x) && (m_NPCPos.x < (m_RandomPos.x + 50)) &&
		((m_RandomPos.z - 50) < m_NPCPos.z) && (m_NPCPos.z < (m_RandomPos.z + 50)))
	{
		IsEndPatrol = true;
	}

	if (IsEndPatrol)
	{
		m_RandomPos = { float(GetRandomNumber()), 0.0f, float(GetRandomNumber()) };
		IsEndPatrol = false;
	}

	LModel::FrameInstancing();

	return true;
}

bool Zombie::Render()
{
	if(m_HP != 100)
	m_enemyHp->Render();
	return true;
}

bool Zombie::RenderMark()
{
	m_minimapMarker->Render();
	return false;
}

Zombie::Zombie(LPlayer* player)
{
	m_Player = player;
	m_ZombieSound = LSoundMgr::GetInstance().Load(L"../../res/sound/zombieSound.mp3");
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	m_Generator.seed(seed);

	m_RandomPos = { float(GetRandomNumber()), 0.0f, float(GetRandomNumber()) };

	m_enemyHp = make_shared<KObject>();
	m_enemyHp->Init();
	m_enemyHp->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/hp_bar.png");
	m_enemyHp->SetPos({ 0, 0, 0 });
	m_enemyHp->SetScale({ 12,2,1 });

	m_minimapMarker = make_shared<KObject>();
	m_minimapMarker->Init();
	m_minimapMarker->SetPos({ 0, 0, -1 });
	m_minimapMarker->SetScale({ 10,10, 1 });
	m_minimapMarker->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/firemode_dot.png");
}
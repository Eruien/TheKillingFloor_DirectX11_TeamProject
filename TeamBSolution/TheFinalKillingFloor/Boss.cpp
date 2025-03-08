#include "Boss.h"
#include "string"
#include "LWrite.h"
#include "LGlobal.h"
#include "KObject.h"
#include "UIManager.h"
#include "BossTrace.h"
#include "BossSwiping.h"
#include "BossJumpAttack.h"
#include "BossTakeDamage.h"
#include "BossDeath.h"

void Boss::FSM(FSMType fsmType)
{
	auto iter = LFSMMgr::GetInstance().m_map.find(fsmType);

	if (LFSMMgr::GetInstance().m_map.end() == iter)
	{
		MessageBoxA(NULL, "FSMType Error", "Error Box", MB_OK);
		return;
	}

	m_pFsm = iter->second.get();

	m_pActionList.insert(std::make_pair(State::BOSSTRACE, std::make_unique<BossTrace>(this)));
	m_pActionList.insert(std::make_pair(State::BOSSSWIPING, std::make_unique<BossSwiping>(this)));
	m_pActionList.insert(std::make_pair(State::BOSSJUMPATTACK, std::make_unique<BossJumpAttack>(this)));
	m_pActionList.insert(std::make_pair(State::BOSSTAKEDAMAGE, std::make_unique<BossTakeDamage>(this)));
	m_pActionList.insert(std::make_pair(State::BOSSDEATH, std::make_unique<BossDeath>(this)));

	m_pAction = m_pActionList.find(State::BOSSTRACE)->second.get();
	m_CurrentState = State::BOSSTRACE;
}

void Boss::SetTransition(Event inputEvent)
{
	m_CurrentState = m_pFsm->StateTransition(m_CurrentState, inputEvent);
	m_pAction = m_pActionList.find(m_CurrentState)->second.get();
}

State Boss::GetState()
{
	return m_CurrentState;
}

void Boss::Process()
{
	m_pAction->Process();
}

void Boss::Move(TVector3 target)
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
	D3DXMatrixScaling(&zombieScale, 0.4f, 0.4f, 0.4f);
	TMatrix zombiePos = zombieScale * matRotation;

	m_matControl = zombiePos;
	m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_Dir.x;
	m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_Dir.z;
}

void Boss::JumpAttackMove(TVector3 target)
{
	
}

int Boss::GetRandomNumber()
{
	return m_Distribution(m_Generator);
}

bool Boss::Frame()
{
	if (LGlobal::g_PlayerModel->IsZedTime)
	{
		m_AnimationRate = 0.5f;
		m_Speed = 75.0f;
	}
	else
	{
		m_AnimationRate = 1.0f;
		m_Speed = 150.0f;
	}

	m_ZombieSound->Play();
	// ºôº¸µå

	m_enemyHp->SetPos({ m_matControl._41, m_matControl._42 + 140, m_matControl._43 });

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

	if (m_HP <= 0)
	{
		LSoundMgr::GetInstance().GetPtr(L"killsound.mp3")->PlayEffect();
		LGlobal::g_PlayerModel->m_Money += 300;
		UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
		IsDead = true;
	}

	LSkinningModel::Frame();

	m_PlayerPos = { m_Player->m_matControl._41, m_Player->m_matControl._42, m_Player->m_matControl._43 };
	m_NPCPos = { m_matControl._41, m_matControl._42, m_matControl._43 };

	if (!IsUseRush)
	{
		m_RushCoolTimeStart += LGlobal::g_fSPF;
	}

	if (abs(m_PlayerPos.x - m_NPCPos.x) > m_RushRange
		&& abs(m_PlayerPos.z - m_NPCPos.z) > m_RushRange)
	{
		m_RushPos = m_PlayerPos;
		IsRush = true;
		IsRushDir = false;
	}

	if ((m_RushCoolTimeStart > m_RushCoolTimeEnd) && IsRush)
	{
		m_RushCoolTimeStart = 0.0f;
		IsUseRush = true;
		IsOnAir = true;
		IsRush = false;
		TVector3 offset = m_PlayerPos - m_NPCPos;
		offset *= 0.9f;
		offset.y = 550.f;

		m_Velocity = offset;
	}

	if (((m_NPCPos.x - m_AttackRange) < m_PlayerPos.x) && (m_PlayerPos.x < (m_NPCPos.x + m_AttackRange))
		&& ((m_NPCPos.z - m_AttackRange) < m_PlayerPos.z) && (m_PlayerPos.z < (m_NPCPos.z + m_AttackRange)))
	{
		IsAttackRange = true;
	}
	else
	{
		IsAttackRange = false;
	}

	return true;
}

bool Boss::Render()
{
	LSkinningModel::Render();
	m_enemyHp->Render();
	return true;
}

bool Boss::RenderMark()
{
	m_minimapMarker->Render();
	return false;
}

Boss::Boss(LPlayer* player)
{
	m_Player = player;
	m_ZombieSound = LSoundMgr::GetInstance().Load(L"../../res/sound/zombieSound.mp3");
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	m_Generator.seed(seed);

	m_RandomPos = { float(GetRandomNumber()), 0.0f, float(GetRandomNumber()) };
	m_AttackRange = 60.0f;
	m_RushRange = 400.0f;
	m_RushCoolTimeStart = 0.0f;
	m_RushCoolTimeEnd = 10.0f;
	m_enemyHp = make_shared<KObject>();
	m_enemyHp->Init();
	m_enemyHp->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/hp_bar.png");
	m_enemyHp->SetPos({ 0, 0, 0 });
	m_enemyHp->SetScale({ 18,3,1 });

	m_minimapMarker = make_shared<KObject>();
	m_minimapMarker->Init();
	m_minimapMarker->SetPos({ 0, 0, -1 });
	m_minimapMarker->SetScale({ 20,20, 1 });
	m_minimapMarker->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/firemode_dot.png");
}
#pragma once
#include <numeric> // for accumulate
#include "LScene.h"
#include "LMap.h"
#include "LHeightMap.h"
#include "LPlayer.h"
#include "LFbxMgr.h"
#include "LDebugCamera.h"
#include "LModelCamera.h"
#include "LBackView.h"
#include "LDxRT.h"
#include "LPlaneShape.h"
#include "Zombie.h"
#include "Tank.h"
#include "Boss.h"
#include "LSkyBox.h"
#include "LSelect.h"
#include "LBox.h"
#include "KObject.h"
#include "ZombieWave.h"
#include "LSoundMgr.h"
#include "Light.h" // light
#include <random>
#include <chrono>
#include "Bullet.h"
#include "LTrail.h"


#define GPLAYER LGlobal::g_PlayerModel
const int g_iMaxLight = 1;

struct SHADOW_CONSTANT_BUFFER
{
	TMatrix	g_matShadow;
};

class InGameScene : public SceneState
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LModelCamera> m_ModelCamera = nullptr;
	std::shared_ptr<LBackView> m_BackViewCamera = nullptr;
	std::shared_ptr<LCamera> m_MinimapCamera = nullptr;
	std::shared_ptr<LCamera> m_MinimapPosCamera = nullptr;
	std::shared_ptr<LSkyBox> m_SkyBox = nullptr;
	vector<shared_ptr<KObject>> _EnemyMarks;
	shared_ptr<KObject> m_muzzleFlash = nullptr;
	vector<shared_ptr<KObject>> m_bloodSplatter;

	std::default_random_engine m_Generator;							// 난수 생성
	std::uniform_real_distribution<float> m_Distribution{ -0.03f, 0.03f };


	std::deque<float> fpsValues;
	int m_crrBlood = 0;
public:
	std::shared_ptr<LHeightMap> m_CustomMap = nullptr;
	std::vector<shared_ptr<Bullet>> m_RifleBulletList;

	vector<vector<shared_ptr<Bullet>>> m_ShotgunBulletListArray;

	int BulletIndex = 0;
	std::shared_ptr<LModel> m_Tree;
	std::shared_ptr<LModel> m_MapModel = nullptr;
	
	vector<shared_ptr<LModel>> m_WallList;
	vector<shared_ptr<LModel>> m_TreeList;
	vector<shared_ptr<LModel>> m_GrassList;
	vector<shared_ptr<LModel>> m_KitList;
	vector<shared_ptr<LModel>> m_AmmoList;

	LFbxObj* mapObj = nullptr;
	LFbxObj* treeObj = nullptr;
	LFbxObj* bulletObj;
	LFbxObj* wallObj = nullptr;
	LFbxObj* grassObj = nullptr;
	LFbxObj* kitObj = nullptr;
	LFbxObj* ammoObj = nullptr;

	std::shared_ptr<ZombieWave> m_ZombieWave = nullptr;
public:
	// pos
	TVector3 m_PlayerFirstSpawnPos;
public: // light
	LIGHT_CONSTANT_BUFFER1	m_cbLight1;
	LIGHT_CONSTANT_BUFFER2	m_cbLight2;
	ComPtr<ID3D11Buffer>	m_pConstantBufferLight[2];
	Light					m_PointLight[1];
	float fLightStart = 1.0f;
	float fLightEnd = 0.15f;
public:
	// Shadow
	LDxRT m_RT;
	LDxRT m_rtMinimap;
	LPlaneShape m_pQuad;
	LPlaneShape m_ShapeMinimap;
	TMatrix	m_matWorld;
	TMatrix	m_matShadow;
	TMatrix	m_matViewLight;
	TMatrix	m_matProjLight;
	TMatrix	m_matTexture;
	SHADOW_CONSTANT_BUFFER m_CBmatShadow;
	ComPtr<ID3D11Buffer> m_pCBShadow;
public:
	//Sword Trail
	LTrail* m_SwordTrail = nullptr;
	TVector3 m_OneHandSwordLow = { 0.0f, 0.0f, 0.0f };
	TVector3 m_OneHandSwordHigh = { 0.0f, 150.0f, 0.0f };
	TVector3 m_TwoHandSwordLow = { 0.0f, 0.0f, 0.0f };
	TVector3 m_TwoHandSwordHigh = { 0.0f, 0.0f, 255.0f };
public:
	// Collision
	LSelect* m_Select = nullptr;
public:
	shared_ptr<KObject> m_playerIcon;
public:
	bool IsNextWave = false;
	bool IsEndGame = false;
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Retry() override;
	void DeleteCurrentObject();
	void Release();
public:
	void SoundInit();
	void CameraInit();
	void PlayerInit(PlayerType playerType);
	void CharacterInit();
	void ResetWeapon();
public:
	//Initialize Functions
	void InitializeWeapon();
	void InitializeObjects();
	void InitializeSkyBox();
	void InitializePlayerIcon();
	void InitializeBloodSplatters();
	void InitializeTrees();
	void InitializeWalls();
	void InitializeBullets();
	void InitializeLighting();
	void InitializeShadow();
	void InitializeMap();
	void InitializeGrasses();
	void InitializeMinimap();
	void InitializeMuzzleFlash();
	void InitializeTreePosition(std::shared_ptr<LModel>& tree);
	void InitializeWallPosition(std::shared_ptr<LModel>& wall, int i, int j);
	void InitializeOBBBox();
	void InitializeItem();
	void InitializeTrail();

	//Process Functions
	
	void ZedAutoAim();

	void ProcessMuzzleFlash();
	void ProcessBloodSplatter();
	void CheckPlayerDeath();
	void PlayInGameSound();
	void UpdateUI();
	void ProcessWaveTransition();
	void UpdateMapObjects();
	void UpdateWallModels();
	void UpdateTreeModels();
	void UpdateBulletModels();
	void ProcessItem();
	void GetItem();
	void GenerateItem();
	void ShootBullet();
	void ShootRifle();
	void ShootShotgun();
	void AutoTargetBullet(TVector3 forward, int index, TVector3 thisPosition);
	
	void SwitchCameraView();
	void UpdateCameraTargetPosition();
	void FramePlayerModel();
	void UpdateZombieAndTankModels();
	void HandlePlayerCollisions();
	void LimitPlayerMovement();
	void LimitNpcMovement();
	void UpdateGunModelPosition();
	void UpdateOBB();
	void FrameUI();
	void FramePointLight();
	void UpdatePlayerPhysics();
	void UpdateNpcPhysics();
	void AdjustPlayerHeight();
	void AdjustNpcHeight();
	void RenderItem();
	void RenderBullets();
public:
	void NextWave();
	void RenderObject();
	void RenderShadow(TMatrix* matWorld, TMatrix* matShadow,
		TMatrix* matView, TMatrix* matProj);
	void CreateShadowConstantBuffer();
	void RenderTrail();
	void InterpolRenderTrail();
public:
	InGameScene(LScene* parent);
	virtual ~InGameScene();
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic = false);
};
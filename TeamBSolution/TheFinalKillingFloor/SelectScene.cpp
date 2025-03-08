#include "SelectScene.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "LExportIO.h"
#include "LInput.h"
#include "UIManager.h"
bool SelectScene::Init()
{
    InitializeDebugCamera();
    InitializeSkyBox();
    InitializeMap();
    InitializeModel();
    InitializeWeapon();
    InitializeLighting();
    InitializeSwordTrail();
    return false;
}

void SelectScene::Process()
{
    m_DebugCamera->m_vCameraPos = m_BindCameraPos;
    m_DebugCamera->m_fCameraYaw = m_BindCameraYaw;
    m_DebugCamera->m_fCameraPitch = m_BindCameraPitch;
    m_DebugCamera->m_fCameraRoll = m_BindCameraRoll;

    m_CustomMap->Frame();
    m_GunMan->Frame();
    m_SwordMan->Frame();
    UpdateWeaponPosition();

    FrameLight();

    UpdateOBB();

    if (LInput::GetInstance().m_MouseState[0])
    {
        if (m_Select.ChkOBBToRay(&m_GunMan->m_OBBBox.m_Box))
        {
            m_GunMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Fire_Rifle_Ironsights.fbx");
            m_SwordMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"TwoHand_Idle_Anim.fbx");
            m_playerType = PlayerType::GUN;
        }

        if (m_Select.ChkOBBToRay(&m_SwordMan->m_OBBBox.m_Box))
        {
            m_SwordMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"TwoHand_FrontSlash.fbx");
            m_GunMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Idle_Rifle_Ironsights.fbx");
            m_playerType = PlayerType::SWORD;
        }
    }

    UIManager::GetInstance().Frame();

    // adjust player's height
    float fHeight = m_CustomMap->GetHeight(m_GunMan->m_matControl._41, m_GunMan->m_matControl._43);
    m_GunMan->m_matControl._42 = fHeight + 1.0f;
    fHeight = m_CustomMap->GetHeight(m_SwordMan->m_matControl._41, m_SwordMan->m_matControl._43);
    m_SwordMan->m_matControl._42 = fHeight + 1.0f;

    if (m_playerType == PlayerType::SWORD)
    {
        SelectBladeMan();
    }
    else if (m_playerType == PlayerType::GUN)
    {
        SelectGunMan();
    }
}

void SelectScene::FrameLight()
{
    if (m_playerType == PlayerType::GUN)
    {
        m_PointLight[0].Frame(m_GunMan->GetPosition());
        m_PointLight[0].m_vDirection = { 0, -1, -1 };
	}
    else
    {
        m_PointLight[0].Frame(m_SwordMan->GetPosition());
        m_PointLight[0].m_vDirection = { 0, -1, -1 };
	}
    
    m_PointLight[0].m_vDirection.Normalize();
}

void SelectScene::Render()
{
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilStateDisable.Get(), 1);
    m_SkyBox->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_SkyBox->Render();
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilState.Get(), 1);

    m_CustomMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_CustomMap->Render();

    if (!m_GunDeathAniEnd)
    {
        m_GunMan->Render();
    }

    if (!m_SwordDeathAniEnd)
    {
        m_SwordMan->Render();
    }
    
    TMatrix playerTranslation;
    playerTranslation.Translation(TVector3(m_GunMan->m_matControl._41, m_GunMan->m_matControl._42 + m_GunMan->m_SettingBox.vCenter.y, m_GunMan->m_matControl._43));
    m_GunMan->m_OBBBox.SetMatrix(&playerTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    //m_GunMan->m_OBBBox.Render();

    if (!m_GunDeathAniEnd)
    {
        m_Rifle->m_WeaponModel->Render();
    }

    playerTranslation.Translation(TVector3(m_SwordMan->m_matControl._41, m_SwordMan->m_matControl._42 + m_SwordMan->m_SettingBox.vCenter.y, m_SwordMan->m_matControl._43));
    m_SwordMan->m_OBBBox.SetMatrix(&playerTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    //m_SwordMan->m_OBBBox.Render();
   
    if (!m_SwordDeathAniEnd)
    {
        m_OneHandSword->m_WeaponModel->Render();
    }

    //m_PointLight[0].m_vPosition = m_GunMan->GetPosition();

    m_cbLight1.g_cAmbientMaterial[0] = TVector4(0.1f, 0.1f, 0.1f, 1);
    m_cbLight1.g_cDiffuseMaterial[0] = TVector4(0.3f);
    m_cbLight1.g_cSpecularMaterial[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cEmissionMaterial[0] = TVector4(0, 0, 0, 1);

    m_cbLight1.g_cAmbientLightColor[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cSpecularLightColor[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cDiffuseLightColor[0] = TVector4(m_PointLight[0].m_DiffuseColor.x,
        m_PointLight[0].m_DiffuseColor.y,
        m_PointLight[0].m_DiffuseColor.z, 1.0f);

    m_cbLight2.g_vLightPos[0] = TVector4(m_PointLight[0].m_vPosition.x,
        m_PointLight[0].m_vPosition.y,
        m_PointLight[0].m_vPosition.z,
        m_PointLight[0].m_fRange);

    m_cbLight2.g_vLightDir[0] = TVector4(m_PointLight[0].m_vDirection.x,
        m_PointLight[0].m_vDirection.y,
        m_PointLight[0].m_vDirection.z, 1.0f);
   
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[0].Get(), 0, NULL, &m_cbLight1, 0, 0);
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[1].Get(), 0, NULL, &m_cbLight2, 0, 0);

    ID3D11Buffer* pBuffers[2];
    pBuffers[0] = m_pConstantBufferLight[0].Get();
    pBuffers[1] = m_pConstantBufferLight[1].Get();
    LGlobal::g_pImmediateContext->PSSetConstantBuffers(3, 2, pBuffers);

    if (!m_SwordDeathAniEnd)
    {
        InterpolRenderTrail();
    }
    
    UIManager::GetInstance().Render();
}

void SelectScene::Release()
{
}

void SelectScene::InitializeDebugCamera()
{
    m_DebugCamera = std::make_shared<LDebugCamera>();
    m_DebugCamera->CreateLookAt({ 0.0f, 0.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
    m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
    m_DebugCamera->m_vCameraPos = m_BindCameraPos;
    m_DebugCamera->m_fCameraYaw = m_BindCameraYaw;
    m_DebugCamera->m_fCameraPitch = m_BindCameraPitch;
    m_DebugCamera->m_fCameraRoll = m_BindCameraRoll;
    LGlobal::g_pMainCamera = m_DebugCamera.get();
}

void SelectScene::InitializeSkyBox()
{
    m_SkyBox = std::make_shared<LSkyBox>();
    m_SkyBox->Set();
    m_SkyBox->Create(L"../../res/hlsl/SkyBox.hlsl", L"../../res/sky/grassenvmap1024.dds");
}

void SelectScene::InitializeMap()
{
    m_CustomMap = std::make_shared<LHeightMap>();
    m_CustomMap->Set();
    m_CustomMap->CreateHeightMap(L"../../res/Heightmap/heightMap513.bmp");
    //map
    LMapDesc MapDesc = {};
    MapDesc.iNumCols = m_CustomMap->m_iNumCols;
    MapDesc.iNumRows = m_CustomMap->m_iNumRows;
    MapDesc.fCellDistance = 1.0f;
    MapDesc.fScaleHeight = 0.1f;
    MapDesc.ShaderFilePath = L"../../res/hlsl/LightMapForSelect.hlsl";
    MapDesc.TextureFilePath = L"../../res/map/snow_01_diff_8k.jpg";
    m_CustomMap->Load(MapDesc);
}

void SelectScene::InitializeModel()
{
    // GunManSetting
    m_GunMan = std::make_shared<LSkinningModel>();
    m_GunMan->m_pModel = LFbxMgr::GetInstance().GetPtr(L"army3.fbx");
    m_GunMan->CreateBoneBuffer();
    m_GunMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Idle_Rifle_Ironsights.fbx");
    TMatrix matScale;
    TMatrix matRot;
    D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);
    m_GunMan->m_matControl *= matScale;
    D3DXMatrixRotationY(&matRot, 3.14159);
    m_GunMan->m_matControl *= matRot;
    m_GunMan->m_matControl._41 += m_GunManPos.x;
    m_GunMan->m_matControl._43 += m_GunManPos.z;
    m_GunMan->m_matControl._42 = m_CustomMap->GetHeight(m_GunMan->m_matControl._41, m_GunMan->m_matControl._43);


    std::wstring head = L"Head";
    std::wstring root = L"root";
    std::wstring neck = L"Neck";

    TMatrix Head = m_GunMan->m_pModel->m_NameMatrixMap[0][head];
    TMatrix Root = m_GunMan->m_pModel->m_NameMatrixMap[0][root];
    TMatrix Neck;

    m_GunMan->SetOBBBox({ -30.0f, Root._42, -20.0f }, { 30.0f, Head._42, 30.0f }, 0.2f);

    m_GunMan->m_OBBBox.Frame();
    m_GunMan->m_OBBBox.CreateOBBBox(
       m_GunMan->m_SettingBox.fExtent[0],
       m_GunMan->m_SettingBox.fExtent[1],
       m_GunMan->m_SettingBox.fExtent[2],
        {m_GunMan->m_OBBBox.m_matWorld._41,
           m_GunMan->m_OBBBox.m_matWorld._42,
           m_GunMan->m_OBBBox.m_matWorld._43 },
       m_GunMan->m_SettingBox.vAxis[0],
       m_GunMan->m_SettingBox.vAxis[1],
       m_GunMan->m_SettingBox.vAxis[2]);

    // SwordManSetting
    m_SwordMan = std::make_shared<LSkinningModel>();
    m_SwordMan->m_pModel = LFbxMgr::GetInstance().GetPtr(L"BladeMan.fbx");
    m_SwordMan->CreateBoneBuffer();
    m_SwordMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"TwoHand_Select_Idle_Anim.fbx");

    D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);
    m_SwordMan->m_matControl *= matScale;
    D3DXMatrixRotationY(&matRot, 3.14159);
    m_SwordMan->m_matControl *= matRot;
    m_SwordMan->m_matControl._41 += m_SwordManPos.x;
    m_SwordMan->m_matControl._43 += m_SwordManPos.z;
    m_SwordMan->m_matControl._42 = m_CustomMap->GetHeight(m_SwordMan->m_matControl._41, m_SwordMan->m_matControl._43);
    
    Neck = m_SwordMan->m_pModel->m_NameMatrixMap[0][neck];
    Root = m_SwordMan->m_pModel->m_NameMatrixMap[0][root];

    m_SwordMan->SetOBBBox({ -30.0f, Root._42, -20.0f }, { 30.0f, Neck._42, 30.0f }, 0.2f);

    m_SwordMan->m_OBBBox.Frame();
    m_SwordMan->m_OBBBox.CreateOBBBox(
        m_SwordMan->m_SettingBox.fExtent[0],
        m_SwordMan->m_SettingBox.fExtent[1],
        m_SwordMan->m_SettingBox.fExtent[2],
        { m_SwordMan->m_OBBBox.m_matWorld._41,
           m_SwordMan->m_OBBBox.m_matWorld._42,
           m_SwordMan->m_OBBBox.m_matWorld._43 },
        m_SwordMan->m_SettingBox.vAxis[0],
        m_SwordMan->m_SettingBox.vAxis[1],
        m_SwordMan->m_SettingBox.vAxis[2]);
}

void SelectScene::InitializeWeapon()
{
    m_Rifle = std::make_shared<LWeapon>();
    m_Rifle->m_WeaponModel = std::make_shared<LModel>();
    m_Rifle->m_WeaponModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Assault_Rifle_A.fbx");

    m_Rifle->m_WeaponModel->m_ParentBoneName = LExportIO::GetInstance().m_ItemParentName[0];
    m_Rifle->m_WeaponModel->m_pModel->m_matScale = LExportIO::GetInstance().m_ItemScale[0];
    m_Rifle->m_WeaponModel->m_pModel->m_matRotation = LExportIO::GetInstance().m_ItemRotation[0];
    m_Rifle->m_WeaponModel->m_pModel->m_matTranslation = LExportIO::GetInstance().m_ItemTranslation[0];

    m_OneHandSword = std::make_shared<LWeapon>();
    m_OneHandSword->m_WeaponModel = std::make_shared<LModel>();
    m_OneHandSword->m_WeaponModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"HeroBlade.fbx");

    m_OneHandSword->m_WeaponModel->m_ParentBoneName = LExportIO::GetInstance().m_ItemParentName[3];
    m_OneHandSword->m_WeaponModel->m_pModel->m_matScale = LExportIO::GetInstance().m_ItemScale[3];
    m_OneHandSword->m_WeaponModel->m_pModel->m_matRotation = LExportIO::GetInstance().m_ItemRotation[3];
    m_OneHandSword->m_WeaponModel->m_pModel->m_matTranslation = LExportIO::GetInstance().m_ItemTranslation[3];
}

void SelectScene::UpdateWeaponPosition()
{
    if (m_Rifle->m_WeaponModel->m_pModel != nullptr && m_GunMan->m_pActionModel != nullptr)
    {
        if (m_GunMan->m_pActionModel->m_iEndFrame <= int(m_GunMan->m_fCurrentAnimTime)) return;

        m_Rifle->m_WeaponModel->m_pModel->m_matSocket =
        m_GunMan->m_pActionModel->m_NameMatrixMap[int(m_GunMan->m_fCurrentAnimTime)][m_Rifle->m_WeaponModel->m_ParentBoneName];

        m_Rifle->m_WeaponModel->m_matControl = 
            m_Rifle->m_WeaponModel->m_pModel->m_matScale * m_Rifle->m_WeaponModel->m_pModel->m_matRotation * m_Rifle->m_WeaponModel->m_pModel->m_matTranslation 
            * m_Rifle->m_WeaponModel->m_pModel->m_matSocket
            * m_GunMan->m_matControl;
    }
    
    if (m_OneHandSword->m_WeaponModel->m_pModel != nullptr && m_SwordMan->m_pActionModel != nullptr)
    {
        if (m_SwordMan->m_pActionModel->m_iEndFrame <= int(m_SwordMan->m_fCurrentAnimTime)) return;

        m_OneHandSword->m_WeaponModel->m_pModel->m_matSocket =
            m_SwordMan->m_pActionModel->m_NameMatrixMap[int(m_SwordMan->m_fCurrentAnimTime)][m_OneHandSword->m_WeaponModel->m_ParentBoneName];

        m_OneHandSword->m_WeaponModel->m_matControl =
            m_OneHandSword->m_WeaponModel->m_pModel->m_matScale * m_OneHandSword->m_WeaponModel->m_pModel->m_matRotation * m_OneHandSword->m_WeaponModel->m_pModel->m_matTranslation 
            * m_OneHandSword->m_WeaponModel->m_pModel->m_matSocket
            * m_SwordMan->m_matControl;
    }
}

void SelectScene::InitializeLighting()
{
    m_pConstantBufferLight[0].Attach(CreateConstantBuffer(
        LGlobal::g_pDevice.Get(), &m_cbLight1, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
    m_pConstantBufferLight[1].Attach(CreateConstantBuffer(
        LGlobal::g_pDevice.Get(), &m_cbLight2, 1, sizeof(LIGHT_CONSTANT_BUFFER2)));
    float fLightRange = 20.0f;
    TVector3 vRotation = TVector3(0.f, 0.0f, 0.0f); //TVector3(-(XM_PI * 0.2f), 0.0f, 0.0f);
    TVector3 vDir = TVector3(0.0f, -1.0f, 0.0f);
    TVector3 v0 = TVector3(0.0f, 10.0f, 0.0f);
    TVector4 v1 = TVector4(1.0f, 1.0f, 1.0f, 1.0f); // color
    TVector3 v2 = TVector3(10.0f, 10.0f, 10.0f);    // scale
    m_PointLight[0].SetValue(&v0,
        &vDir,
        &fLightRange,
        &v1,
        &v2,
        &vRotation,
        90.0f, // 내부
        120.0f);//외부
}

void SelectScene::InitializeSwordTrail()
{
    m_SwordTrail = new LTrail;
    m_SwordTrail->Set();
    // Texture버전
    m_SwordTrail->Create(L"../../res/hlsl/SwordTrailTexture.hlsl", L"../../res/Trail/T_SwipeTrail.png");
    // VertexColor버전
    //m_SwordTrail->Create(L"../../res/hlsl/SwordTrailVertexColor.hlsl", L"../../res/map/topdownmap.jpg");
}

ID3D11Buffer* SelectScene::CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic)
{
    HRESULT hr = S_OK;
    ID3D11Buffer* pBuffer = nullptr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    if (bDynamic)
    {
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0;
    }
    bd.ByteWidth = iSize * iNumIndex;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    InitData.pSysMem = data;
    if (data != NULL)
    {
        if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
        {
            //H(hr);
            return nullptr;
        }
    }
    else
    {
        if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
        {
            //H(hr);
            return nullptr;
        }
    }
    return pBuffer;
}

void SelectScene::UpdateOBB()
{
    m_Select.SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_Select.Update();
    m_GunMan->m_OBBBox.UpdateOBBBoxPosition(
        { m_GunMan->m_OBBBox.m_matWorld._41,
            m_GunMan->m_OBBBox.m_matWorld._42,
            m_GunMan->m_OBBBox.m_matWorld._43 });
    m_SwordMan->m_OBBBox.UpdateOBBBoxPosition(
        { m_SwordMan->m_OBBBox.m_matWorld._41,
            m_SwordMan->m_OBBBox.m_matWorld._42,
            m_SwordMan->m_OBBBox.m_matWorld._43 });
}

void SelectScene::InterpolRenderTrail()
{
    m_SwordTrail->InterpolRenderTrail(&LocalSwordLow,
        &LocalSwordHigh, 
        &m_OneHandSword->m_WeaponModel->m_matControl);
}

void SelectScene::SelectBladeMan()
{
    if (UIManager::GetInstance().GetUIObject(L"B_Play")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {
        m_GunChangeDeathAni = true;
        m_GunMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Player_Death.fbx");
        m_SwordMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"TwoHand_Idle_Anim.fbx");
        LSoundMgr::GetInstance().GetPtr(L"PlayerHitSound.WAV")->PlayEffect();
    }

    if (m_GunChangeDeathAni)
    {
        if (m_GunMan->m_TimerEnd)
        {
            m_GunDeathAniEnd = true;
            m_SwordMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"TwoHand_Walk.fbx");
        }
    }

    if (m_GunDeathAniEnd)
    {
        IsUseTimer = true;
        TVector3 swordManForWard = m_SwordMan->m_matControl.Forward();
        swordManForWard.Normalize();
        m_SwordMan->m_matControl._41 += swordManForWard.x * LGlobal::g_fSPF * 100;
        m_SwordMan->m_matControl._43 += swordManForWard.z * LGlobal::g_fSPF * 100;
        LSoundMgr::GetInstance().GetPtr(L"PlayerStep.wav")->Play(false);
    }

    if (IsUseTimer)
    {
        m_TimerStart += LGlobal::g_fSPF;
    }

    if (m_TimerStart > m_TimerEnd)
    {
        m_TimerStart = 0.0f;
        m_GunDeathAniEnd = false;
        IsUseTimer = false;
        m_GunChangeDeathAni = false;
        D3DXMatrixTranslation(&m_SwordMan->m_matControl, m_SwordManPos.x, m_SwordManPos.y, m_SwordManPos.z);
        LSoundMgr::GetInstance().GetPtr(L"PlayerStep.wav")->Stop();
        UIManager::GetInstance().ChangeScene(Event::GOINGAMESCENE);
    }
}

void SelectScene::SelectGunMan()
{
    if (UIManager::GetInstance().GetUIObject(L"B_Play")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {
        m_SwordChangeDeathAni = true;
        m_GunMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Idle_Rifle_Ironsights.fbx");
        m_SwordMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"TwoHand_Death.fbx");
        LSoundMgr::GetInstance().GetPtr(L"PlayerHitSound.WAV")->PlayEffect();
    }

    if (m_SwordChangeDeathAni)
    {
        if (m_SwordMan->m_TimerEnd)
        {
            m_SwordDeathAniEnd = true;
            m_GunMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Walk_Fwd_Rifle_Ironsights.fbx");
        }
    }

    if (m_SwordDeathAniEnd)
    {
        IsUseTimer = true;
        TVector3 gunManForWard = m_GunMan->m_matControl.Forward();
        gunManForWard.Normalize();
        m_GunMan->m_matControl._41 += gunManForWard.x * LGlobal::g_fSPF * 100;
        m_GunMan->m_matControl._43 += gunManForWard.z * LGlobal::g_fSPF * 100;
        LSoundMgr::GetInstance().GetPtr(L"PlayerStep.wav")->Play(false);
    }

    if (IsUseTimer)
    {
        m_TimerStart += LGlobal::g_fSPF;
    }

    if (m_TimerStart > m_TimerEnd)
    {
        m_TimerStart = 0.0f;
        m_SwordDeathAniEnd = false;
        IsUseTimer = false;
        m_SwordChangeDeathAni = false;
        D3DXMatrixTranslation(&m_GunMan->m_matControl, m_GunManPos.x, m_GunManPos.y, m_GunManPos.z);
        LSoundMgr::GetInstance().GetPtr(L"PlayerStep.wav")->Stop();
        UIManager::GetInstance().ChangeScene(Event::GOINGAMESCENE);
    }
}

SelectScene::SelectScene(LScene* parent) : SceneState(parent)
{
    //Init();
}

SelectScene::~SelectScene()
{

}

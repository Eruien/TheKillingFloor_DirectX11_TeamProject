#include "InGameScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "UIManager.h"
#include "LCharacterIO.h"
#include "LAnimationIO.h"
#include "LExportIO.h"
#include "LWeaponMgr.h"
#include "SelectScene.h"

static bool Init_2 = true;
bool InGameScene::Init()
{
    
    SoundInit();
    CameraInit();
    CharacterInit();
    CreateShadowConstantBuffer();

    InitializeSkyBox();
    InitializePlayerIcon();
    InitializeBloodSplatters();
    InitializeMap();
    InitializeWalls();
    InitializeBullets();
    InitializeLighting();
    InitializeShadow();
    InitializeTrees();
    InitializeGrasses();
    InitializeMinimap();
    InitializeMuzzleFlash();
    InitializeOBBBox();
    InitializeItem();
    InitializeTrail();

    m_PlayerFirstSpawnPos = { LGlobal::g_PlayerModel->m_matControl._41, LGlobal::g_PlayerModel->m_matControl._42, LGlobal::g_PlayerModel->m_matControl._43 };

   

    return true;
}
void InGameScene::Process()
{
    if (m_BackViewCamera.get() != LGlobal::g_pMainCamera)
    {
        LGlobal::g_pMainCamera = m_BackViewCamera.get();
    }
    //상점키
    if (LINPUT.m_KeyStateOld[DIK_B] == KEY_UP)
    {
    
            UIManager::GetInstance().ChangeScene(Event::GOSHOPSCENE);
        
    }

    if (LGlobal::g_PlayerModel->IsZedTime)
        ZedAutoAim();

    ProcessBloodSplatter();
    CheckPlayerDeath();
    PlayInGameSound();
    UpdateUI();
    ProcessWaveTransition();
    UpdateMapObjects();
    UpdateWallModels();
    UpdateTreeModels();
    SwitchCameraView();
    UpdateCameraTargetPosition();
    FramePlayerModel();
    UpdateBulletModels(); // 총알 위치 업데이트
    GenerateItem();

    UpdateZombieAndTankModels();
    HandlePlayerCollisions(); // 플레이어 충돌처리
    LimitPlayerMovement();
    LimitNpcMovement();
    UpdateGunModelPosition();
    UpdateOBB();
    FrameUI();
    FramePointLight();
    UpdatePlayerPhysics();
    AdjustPlayerHeight();
    UpdateNpcPhysics();
    AdjustNpcHeight();
    

    ProcessItem();
}

void InGameScene::Render()
{
    // 새로운 FPS 값을 큐에 추가
    fpsValues.push_back(1.0f / LGlobal::g_fSPF);
    // 큐의 크기가 일정 크기를 넘으면 가장 오래된 값을 제거
    if (fpsValues.size() > 100) {
        fpsValues.pop_front();
    }
    // FPS 값을 평균내어 텍스트로 변환
    float averageFPS = std::accumulate(fpsValues.begin(), fpsValues.end(), 0.0f) / fpsValues.size();
    std::wstring fpsText = std::to_wstring(int(averageFPS));
    LWrite::GetInstance().AddText(fpsText, 10.0f, 10.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    //
    
    fLightStart += LGlobal::g_fSPF;
    if (fLightStart > fLightEnd)
    {
        m_PointLight[0].m_vPosition.y = -1000.f;
    }
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilStateDisable.Get(), 1);
    m_SkyBox->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_SkyBox->Render();
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilState.Get(), 1);
    //LGlobal::g_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
    // Light Render
    m_cbLight1.g_cAmbientMaterial[0] = TVector4(0.1f, 0.1f, 0.1f, 1);
    m_cbLight1.g_cDiffuseMaterial[0] = TVector4(0.6f);
    m_cbLight1.g_cSpecularMaterial[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cEmissionMaterial[0] = TVector4(0, 0, 0, 1);

    m_cbLight1.g_cAmbientLightColor[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cSpecularLightColor[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cDiffuseLightColor[0] = TVector4(m_PointLight[0].m_DiffuseColor.x,
        m_PointLight[0].m_DiffuseColor.y,
        m_PointLight[0].m_DiffuseColor.z, 1.0f);
    /*TMatrix matInvWorld;
    D3DXMatrixInverse(&matInvWorld, NULL, &m_PointLight[0].m_matWorld);
    D3DXMatrixTranspose(&matInvWorld, &matInvWorld);
    D3DXMatrixTranspose(&m_cbLight2.g_matInvWorld[0], &matInvWorld);*/

   /* m_cbLight2.g_vEyeDir[0].x = LGlobal::g_pMainCamera->m_vLook.x; 
    m_cbLight2.g_vEyeDir[0].y = LGlobal::g_pMainCamera->m_vLook.y;
    m_cbLight2.g_vEyeDir[0].z = LGlobal::g_pMainCamera->m_vLook.z;*/
    //m_cbLight2.g_vEyeDir[0].w = 100.0f; // 강도

    m_cbLight2.g_vLightPos[0] = TVector4(m_PointLight[0].m_vPosition.x,
        m_PointLight[0].m_vPosition.y,
        m_PointLight[0].m_vPosition.z,
        m_PointLight[0].m_fRange);

    m_cbLight2.g_vLightDir[0] = TVector4(m_PointLight[0].m_vDirection.x,
        m_PointLight[0].m_vDirection.y,
        m_PointLight[0].m_vDirection.z, 1.0f);
    m_cbLight1.g_bIsZedTime = LGlobal::g_PlayerModel->IsZedTime ? 1.0f : 0.0f;
    //if (LGlobal::g_PlayerModel->IsZedTime == true)
    //{
    //    m_cbLight1.g_bIsZedTime = LGlobal::g_PlayerModel->IsZedTime;
    //}
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[0].Get(), 0, NULL, &m_cbLight1, 0, 0);
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[1].Get(), 0, NULL, &m_cbLight2, 0, 0);
    
    ID3D11Buffer* pBuffers[2];
    pBuffers[0] = m_pConstantBufferLight[0].Get();
    pBuffers[1] = m_pConstantBufferLight[1].Get();
    LGlobal::g_pImmediateContext->PSSetConstantBuffers(3, 2, pBuffers);

    //
    TVector4 vClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (m_RT.Begin(vClearColor))
    {
        TVector3 vLightPos = TVector3(-10000, 10000, -10000);
        //-----------------------------------------------------
        // 1패스:그림자맵 생성
        //-----------------------------------------------------
        TVector3 vEye = vLightPos;
        TVector3 vLookat = TVector3(0, 0, 0);
        TVector3 vUp = TVector3(0.0f, 1.0f, 0.0f);
        D3DXMatrixLookAtLH(&m_matViewLight, &vEye, &vLookat, &vUp);
        D3DXMatrixOrthoOffCenterLH(&m_matProjLight, -512 / 2, 512 / 2, -512 / 2, 512 / 2, 0.0f, 100.0f);
        RenderShadow(&LGlobal::g_PlayerModel->m_matControl, &m_matShadow, &m_matViewLight, &m_matProjLight);
        m_RT.End();
    }

    RenderObject();
    LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->Render();
    
    m_ZombieWave->Render();

    // map
    //m_MapModel->m_pModel->m_DrawList[0]->SetMatrix(&m_MapModel->m_matControl, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    //m_CBmatShadow.g_matShadow = m_MapModel->m_matControl * m_matViewLight * m_matProjLight * m_matTexture;
    //D3DXMatrixTranspose(&m_CBmatShadow.g_matShadow, &m_CBmatShadow.g_matShadow);
    //LGlobal::g_pImmediateContext->UpdateSubresource(m_pCBShadow.Get(), 0, NULL, &m_CBmatShadow, 0, 0);
    //LGlobal::g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pCBShadow.GetAddressOf());
    //m_MapModel->m_pModel->m_DrawList[0]->PreRender();
    //ID3D11ShaderResourceView* pSRV[] = { m_RT.m_pSRV.Get() };
    //LGlobal::g_pImmediateContext->PSSetShaderResources(1, 1, pSRV);
    //m_MapModel->m_pModel->m_DrawList[0]->PostRender();
  
    m_CustomMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_CustomMap->Render();
    static float sTime;
    sTime += LGlobal::g_fSPF;


    for (auto& wall : m_WallList)
    {
        wall->Render();
    }

    for (auto& tree : m_TreeList)
    {
        tree->Render();
    }
    // grass
    RenderItem();
    RenderBullets();
    for (auto& grass : m_GrassList)
    {
        grass->Render();
    }

    // bullet
    
    

    InterpolRenderTrail();

    

    //muzzleFlash
    if (LGlobal::g_PlayerModel->IsShoot)
    {
        if (sTime >= LGlobal::g_PlayerModel->m_Gun->m_GunSpec.ShootDelay)
        {
            m_muzzleFlash->SetIsRender(true);

            sTime = 0;
        }
        else if (sTime + 0.05f >= LGlobal::g_PlayerModel->m_Gun->m_GunSpec.ShootDelay)
        {
            m_muzzleFlash->SetIsRender(false);
        }
        m_muzzleFlash->Render();
    }

    //blood
    for (auto& obj : m_bloodSplatter)
    {
        obj->Render();
    }

    

    // Shadow
   /* m_pQuad.SetMatrix(NULL, NULL, NULL);
    m_pQuad.PreRender();
    {
        LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, m_RT.m_pSRV.GetAddressOf());
    }
    m_pQuad.PostRender();*/

    // Collision
    
    TMatrix playerTranslation;
    playerTranslation.Translation(TVector3(LGlobal::g_PlayerModel->m_matControl._41, LGlobal::g_PlayerModel->m_matControl._42 + LGlobal::g_PlayerModel->m_SettingBox.vCenter.y, LGlobal::g_PlayerModel->m_matControl._43));
    LGlobal::g_PlayerModel->m_OBBBox.SetMatrix(&playerTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    //LGlobal::g_PlayerModel->m_OBBBox.Render();

    TMatrix weaponPos = LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl;
    
    if (LGlobal::g_PlayerModel->m_Type == PlayerType::SWORD)
    {
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.WeaponSetMatrix(&weaponPos, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.Render();
    }
 
    m_ZombieWave->CollisionBoxRender();

    //if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
    //{
    //    if (LInput::GetInstance().m_MouseState[0])
    //    {
    //        if (m_ZombieWave->m_EnemyMap["Zombie"].size() > 0)
    //        {
    //            for (auto& zombie : m_ZombieWave->m_EnemyMap["Zombie"])
    //            {
    //                if (m_Select->ChkOBBToRay(&zombie->m_OBBBox.m_Box) &&
    //                    LGlobal::g_PlayerModel->m_CurrentGun != WeaponState::SHOTGUN)
    //                {
    //                    if (LGlobal::g_PlayerModel->IsShoot)
    //                    {
    //                        float ShotHeight = (m_Select->m_vIntersection.y - zombie->m_matControl._42);
    //                        if (ShotHeight > (zombie->m_OBBBox.fTall * 0.85))
    //                        {
    //                            zombie->IsHeadShot = true;
    //                        }
    //                        else
    //                        {
    //                            zombie->IsHeadShot = false;
    //                        }
    //                        zombie->IsTakeDamage = true;
    //                        m_bloodSplatter[m_crrBlood]->SetPos(m_Select->m_vIntersection + LGlobal::g_PlayerModel->m_matControl.Forward() * 150);
    //                        m_bloodSplatter[m_crrBlood]->GetScript<Animator>(L"Animator")->_currentKeyframeIndex = 0;
    //                        m_bloodSplatter[m_crrBlood]->SetIsRender(true);
    //                        m_crrBlood++;
    //                        if (m_crrBlood == m_bloodSplatter.size())
    //                            m_crrBlood = 0;
    //                    }

    //                    //std::string boxintersect = "박스와 직선의 충돌, 교점 = (" + std::to_string(m_Select->m_vIntersection.x) + "," + std::to_string(m_Select->m_vIntersection.y) + "," + std::to_string(m_Select->m_vIntersection.z) + ")";
    //                    //MessageBoxA(0, boxintersect.c_str(), 0, MB_OK);
    //                }
    //            }
    //        }
    //    }
    //}
    if (LGlobal::g_PlayerModel->m_Type == PlayerType::SWORD)
    {
        if (LGlobal::g_PlayerModel->IsResetBladeAttack)
        {
            for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
            {
                zombie->IsHitBladeAttack = false;
            }
            LGlobal::g_PlayerModel->IsResetBladeAttack = false;
        }
        if (m_ZombieWave->m_EnemyMap["LNPC"].size() > 0)
        {
            for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
            {
                if (LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.CollisionCheckOBB(&zombie->m_OBBBox) && 
                    LGlobal::g_PlayerModel->IsSlash && !zombie->IsHitBladeAttack)
                {
                    zombie->IsHitBladeAttack = true;
                    zombie->IsTakeDamage = true;
                }
            }
        }
    }
   
    std::wstring textState = L"InGameScene";
    //LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });



    
    
    //UI
    UIManager::GetInstance().Render();
 
    m_ShapeMinimap.SetMatrix(NULL, NULL, NULL);

    m_ShapeMinimap.PreRender();
    {
        LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, m_rtMinimap.m_pSRV.GetAddressOf());
    }
    m_ShapeMinimap.PostRender();

    //미니맵
    //
    //
    //
    //
    if (m_rtMinimap.Begin(vClearColor))
    {
        m_CustomMap->SetMatrix(nullptr, &m_MinimapCamera->m_matView, &m_MinimapCamera->m_matProj);
        m_CustomMap->Render();

       
 
        float offset = (   float(LGlobal::g_WindowWidth)/ float(LGlobal::g_WindowHeight))-0.3f;
        //256==렌더타켓 사이즈, 2048 = 맵 사이즈
        m_playerIcon->m_vPosition={ LGlobal::g_PlayerModel->m_matControl._41 * (256.0f / 2048.0f) ,0, LGlobal::g_PlayerModel->m_matControl._43 * (256.0f / 2048.0f) * offset };
        m_playerIcon->m_vRotation.z =- LGlobal::g_pMainCamera->m_fCameraYaw;
     


        m_playerIcon->SetMatrix(nullptr, &m_MinimapPosCamera->m_matView, &m_MinimapPosCamera->m_matOrthoProjection);
        m_playerIcon->Frame();
        m_playerIcon->Render();

        for (auto& obj : m_ZombieWave->m_EnemyMap["Zombie"])
        {
            obj->m_minimapMarker->m_vPosition = { obj->m_matControl._41 * (256.0f / 2048.0f) ,0, obj->m_matControl._43 * (256.0f / 2048.0f) * offset };
            //obj->m_minimapMarker->m_vRotation.z = -m_ModelCamera->m_fCameraYaw;
            obj->m_minimapMarker->SetMatrix(nullptr, &m_MinimapPosCamera->m_matView, &m_MinimapPosCamera->m_matOrthoProjection);
            obj->m_minimapMarker->Frame();
            obj->RenderMark();
        }

        for (auto& obj : m_ZombieWave->m_EnemyMap["Tank"])
        {
            obj->m_minimapMarker->m_vPosition = { obj->m_matControl._41 * (256.0f / 2048.0f) ,0, obj->m_matControl._43 * (256.0f / 2048.0f) * offset };
            //obj->m_minimapMarker->m_vRotation.z = -m_ModelCamera->m_fCameraYaw;
            obj->m_minimapMarker->SetMatrix(nullptr, &m_MinimapPosCamera->m_matView, &m_MinimapPosCamera->m_matOrthoProjection);
            obj->m_minimapMarker->Frame();
            obj->RenderMark();
        }

        for (auto& obj : m_ZombieWave->m_EnemyMap["Boss"])
        {
            obj->m_minimapMarker->m_vPosition = { obj->m_matControl._41 * (256.0f / 2048.0f) ,0, obj->m_matControl._43 * (256.0f / 2048.0f) * offset };
            //obj->m_minimapMarker->m_vRotation.z = -m_ModelCamera->m_fCameraYaw;
            obj->m_minimapMarker->SetMatrix(nullptr, &m_MinimapPosCamera->m_matView, &m_MinimapPosCamera->m_matOrthoProjection);
            obj->m_minimapMarker->Frame();
            obj->RenderMark();
        }


        //LGlobal::g_PlayerModel

        m_rtMinimap.End();
    }

   

   Release();

   if (IsEndGame)
   {
       UIManager::GetInstance().ChangeScene(Event::GOENDSCENE);
   }

   if (LInput::GetInstance().m_MouseState[2] == KEY_PUSH)
   {
       LScene::GetInstance().m_pActionList[State::INGAMESCENE]->Retry();
     //  Retry();
       
   }
}

void InGameScene::Retry()
{
    for (auto& kit : m_KitList)
    {
        kit->bVisible = false;
    }
    for (auto& ammo : m_AmmoList)
    {
		ammo->bVisible = false;
	}
    IsEndGame = false;
    DeleteCurrentObject();
    ResetWeapon();
    //m_pActionList.find(State::CHARACTERIDLE)->second.get();
    if (static_cast<SelectScene*>(LScene::GetInstance().m_pActionList.find(State::SELECTSCENE)->second.get())->m_playerType == PlayerType::GUN)
    {
        PlayerInit(PlayerType::GUN);
        LGlobal::g_PlayerModel->m_Gun->m_GunSpec.CurrentAmmo = LGlobal::g_PlayerModel->m_Gun->m_GunSpec.TotalAmmo;
        UIManager::GetInstance().GetUIObject(L"Selected_Sword")->SetIsRender(false);
        UIManager::GetInstance().GetUIObject(L"C_Ammo")->SetIsRender(true);
        UIManager::GetInstance().GetUIObject(L"T_Ammo")->SetIsRender(true);
        UIManager::GetInstance().GetUIObject(L"weaponUI")->SetIsRender(true);
        UIManager::GetInstance().GetUIObject(L"AmmoSlash")->SetIsRender(true);
        UIManager::GetInstance().GetUIObject(L"C_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LWeaponMgr::GetInstance().m_map[LGlobal::g_PlayerModel->m_CurrentGun]->m_GunSpec.CurrentAmmo);
        UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LWeaponMgr::GetInstance().m_map[LGlobal::g_PlayerModel->m_CurrentGun]->m_GunSpec.defaultTotalAmmo);
    }
    if (static_cast<SelectScene*>(LScene::GetInstance().m_pActionList.find(State::SELECTSCENE)->second.get())->m_playerType == PlayerType::SWORD)
    {
        PlayerInit(PlayerType::SWORD);
        UIManager::GetInstance().GetUIObject(L"Selected_Sword")->SetIsRender(true);
        UIManager::GetInstance().GetUIObject(L"C_Ammo")->SetIsRender(false);
        UIManager::GetInstance().GetUIObject(L"T_Ammo")->SetIsRender(false);
        UIManager::GetInstance().GetUIObject(L"weaponUI")->SetIsRender(false);
        UIManager::GetInstance().GetUIObject(L"AmmoSlash")->SetIsRender(false);
        UIManager::GetInstance().GetUIObject(L"Selected_Sword")->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/knife.png");
    }
    //PlayerInit(PlayerType::SWORD);
    LGlobal::g_PlayerModel->m_Money = 0;
    m_ZombieWave->m_CurrentWave = 0;
    NextWave();
    Init_2 = true;
    UpdateUI();
    
}

void InGameScene::DeleteCurrentObject()
{
    for (auto iter = m_ZombieWave->m_EnemyMap["Zombie"].begin(); iter != m_ZombieWave->m_EnemyMap["Zombie"].end();)
    {
        iter = m_ZombieWave->m_EnemyMap["Zombie"].erase(iter);
    }

    for (auto iter = m_ZombieWave->m_EnemyMap["Tank"].begin(); iter != m_ZombieWave->m_EnemyMap["Tank"].end();)
    {
        iter = m_ZombieWave->m_EnemyMap["Tank"].erase(iter);
    }

    for (auto iter = m_ZombieWave->m_EnemyMap["Boss"].begin(); iter != m_ZombieWave->m_EnemyMap["Boss"].end();)
    {
        iter = m_ZombieWave->m_EnemyMap["Boss"].erase(iter);
    }

    m_ZombieWave->m_EnemyMap["LNPC"].clear();

    delete LGlobal::g_PlayerModel;
    LGlobal::g_PlayerModel = nullptr;
}

void InGameScene::Release()
{
    for (auto iter = m_ZombieWave->m_EnemyMap["Zombie"].begin(); iter != m_ZombieWave->m_EnemyMap["Zombie"].end();)
    {
        if ((*iter)->IsDead)
        {
            iter = m_ZombieWave->m_EnemyMap["Zombie"].erase(iter);
            UIManager::GetInstance().GetUIObject(L"EnemyCount")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_EnemyMap["Zombie"].size());
            LGlobal::g_PlayerModel->m_ZedTimeCount += 1;
        }
        else
        {
            iter++;
        }
    }

    for (auto iter = m_ZombieWave->m_EnemyMap["Tank"].begin(); iter != m_ZombieWave->m_EnemyMap["Tank"].end();)
    {
        if ((*iter)->IsDead)
        {
            iter = m_ZombieWave->m_EnemyMap["Tank"].erase(iter);
            LGlobal::g_PlayerModel->m_ZedTimeCount += 1;
        }
        else
        {
            iter++;
        }
    }

    for (auto iter = m_ZombieWave->m_EnemyMap["Boss"].begin(); iter != m_ZombieWave->m_EnemyMap["Boss"].end();)
    {
        if ((*iter)->IsDead)
        {
            iter = m_ZombieWave->m_EnemyMap["Boss"].erase(iter);
            LGlobal::g_PlayerModel->m_ZedTimeCount += 1;
        }
        else
        {
            iter++;
        }
    }

    if (m_ZombieWave->m_EnemyMap["Zombie"].size() <= 0 && 
        m_ZombieWave->m_EnemyMap["Tank"].size() <= 0 &&
        m_ZombieWave->m_EnemyMap["Boss"].size() <= 0)
    {
        IsNextWave = true;
    }
}

void InGameScene::SoundInit()
{
    LSoundMgr::GetInstance().Load(L"../../res/sound/InGameSound.mp3");
    LSoundMgr::GetInstance().Load(L"../../res/sound/GunFire.wav");
    LSoundMgr::GetInstance().Load(L"../../res/sound/PlayerStep.wav");
    LSoundMgr::GetInstance().Load(L"../../res/sound/SteamPack.wav");
    LSoundMgr::GetInstance().Load(L"../../res/sound/ZedTimeFirst.mp3");
    LSoundMgr::GetInstance().Load(L"../../res/sound/ZedTimeLast.mp3");
    LSoundMgr::GetInstance().Load(L"../../res/sound/PlayerHitSound.WAV");
    LSoundMgr::GetInstance().Load(L"../../res/sound/headshot.mp3");
    LSoundMgr::GetInstance().Load(L"../../res/sound/killsound.mp3");
    LSoundMgr::GetInstance().Load(L"../../res/sound/soldierUlti.mp3");
    LSoundMgr::GetInstance().Load(L"../../res/sound/OneHandSlashOne.WAV");
    LSoundMgr::GetInstance().Load(L"../../res/sound/OneHandSlashTwo.WAV");
    LSoundMgr::GetInstance().Load(L"../../res/sound/TwoHandSlashOne.WAV");
    LSoundMgr::GetInstance().Load(L"../../res/sound/TwoHandSlashTwo.WAV");
    LSoundMgr::GetInstance().Load(L"../../res/sound/BladeHitSound.mp3");
}

void InGameScene::CameraInit()
{
    m_DebugCamera = std::make_shared<LDebugCamera>();
    m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
    m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);

    m_ModelCamera = std::make_shared<LModelCamera>();
    m_ModelCamera->SetTargetPos(TVector3(0.0f, 0.0f, 0.0f));
    m_ModelCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
    m_ModelCamera->m_fRadius = 100.0f;

    m_BackViewCamera = std::make_shared<LBackView>();
    m_BackViewCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
    m_BackViewCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
    LGlobal::g_pMainCamera = m_BackViewCamera.get();

    m_MinimapCamera = std::make_shared<LCamera>();
    m_MinimapCamera->CreateLookAt({ 0.0f, 6000.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    m_MinimapCamera->m_fCameraPitch = 0.f;
    m_MinimapCamera->CreatePerspectiveFov(L_PI * 0.1f, 1.0f, -1.0f, 10000.0f);
    //m_MinimapCamera->CreateOrthographic((float)256, (float)256, -1.0f, 10000.0f);

    m_MinimapPosCamera = std::make_shared<LCamera>();
    m_MinimapPosCamera->CreateLookAt({ 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    m_MinimapPosCamera->m_fCameraPitch = 0.f;
    m_MinimapPosCamera->CreateOrthographic((float)256, (float)256, -1.0f, 10000.0f);
}

void InGameScene::PlayerInit(PlayerType playerType)
{
    // PlayerSetting
    LGlobal::g_PlayerModel = new LPlayer;
    LGlobal::g_PlayerModel->m_Type = playerType;
    if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
    {
        LGlobal::g_PlayerModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"army3.fbx");
        LGlobal::g_PlayerModel->ItemChnge(WeaponState::PISTOL, 1);
    }
    else if (LGlobal::g_PlayerModel->m_Type == PlayerType::SWORD)
    {
        LGlobal::g_PlayerModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"BladeMan.fbx");
        LGlobal::g_PlayerModel->ItemChnge(WeaponState::TWOHANDSWORD, 3);
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->SetOBBBox({ -30.0f, 0.0f, 0.0f }, { 30.0f, 50.0f, 200.0f }, 1.0f);
        LGlobal::g_PlayerModel->ItemChnge(WeaponState::ONEHANDSWORD, 2);
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->SetOBBBox({ -30.0f, 0.0f, 0.0f }, { 30.0f, 150.0f, 100.0f }, 1.0f);
    }
    LGlobal::g_PlayerModel->CreateBoneBuffer();
    LGlobal::g_PlayerModel->FSM(FSMType::PLAYER);

    TMatrix matScale;
    TMatrix matRot;
    D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);
    LGlobal::g_PlayerModel->m_matControl *= matScale;
    D3DXMatrixRotationY(&matRot, 3.14159);
    LGlobal::g_PlayerModel->m_matControl *= matRot;

    m_ModelCamera->SetTarget(LGlobal::g_PlayerModel);

    std::wstring neck = L"Neck";
    std::wstring root = L"root";
  
    TMatrix Neck = LGlobal::g_PlayerModel->m_pModel->m_NameMatrixMap[0][neck];
    TMatrix Root = LGlobal::g_PlayerModel->m_pModel->m_NameMatrixMap[0][root];

    LGlobal::g_PlayerModel->SetOBBBox({ -30.0f, Root._42, -20.0f }, { 30.0f, Neck._42, 30.0f }, 0.2f);
}

void InGameScene::CharacterInit()
{
    // Zombie
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/Zombie.bin", L"../../res/hlsl/CharacterShaderInAnimationData.hlsl");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Attack_Anim.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Death.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Walk_Lock.bin");
    LFbxMgr::GetInstance().m_ZombieMap.push_back(LFbxMgr::GetInstance().GetPtr(L"Zombie_Attack_Anim.fbx"));
    LFbxMgr::GetInstance().m_ZombieMap.push_back(LFbxMgr::GetInstance().GetPtr(L"Zombie_Death.fbx"));
    LFbxMgr::GetInstance().m_ZombieMap.push_back(LFbxMgr::GetInstance().GetPtr(L"Zombie_TakeDamage.fbx"));
    LFbxMgr::GetInstance().m_ZombieMap.push_back(LFbxMgr::GetInstance().GetPtr(L"Zombie_Walk_Lock.fbx"));

    // Tank
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/Tank.bin", L"../../res/hlsl/CharacterShader.hlsl");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_Combo.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_Death.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_Walk.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_Run.bin");

    // Boss
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/Boss.bin", L"../../res/hlsl/CharacterShader2.hlsl");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Boss_JumpAttack.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Boss_Run.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Boss_Swiping.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Boss_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Boss_SpinAttack.bin");
  
    // ZombieWaveSetting
    m_ZombieWave = std::make_shared<ZombieWave>();
    m_ZombieWave->m_CurrentWave = 0;
    NextWave();

    // Gun Character
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/army3.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Fire_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Reload_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Sprint_Fwd_Rifle.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Walk_Fwd_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Idle_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Player_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Player_Death.bin");

    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Pistol_Idle.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Pistol_Run.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Pistol_Shoot.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Pistol_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Psitol_Walk.bin");

    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Shotgun_Fire.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Shotgun_Reload.bin");

    // Blade Character
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/BladeMan.bin");

    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/OneHand_Walk.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/OneHand_Run.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/OneHand_Rush.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/OneHand_Inward.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/OneHand_Outward.bin");

    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/TwoHand_Idle_Anim.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/TwoHand_Select_Idle_Anim.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/TwoHand_FrontSlash.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/TwoHand_Walk.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/TwoHand_Run.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/TwoHand_Rush.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/TwoHand_Death.bin");

    // Item
    LCharacterIO::GetInstance().ItemRead(L"../../res/UserFile/Item/Assault_Rifle_A.bin");
    LCharacterIO::GetInstance().ItemRead(L"../../res/UserFile/Item/Pistols_A.bin");
    LCharacterIO::GetInstance().ItemRead(L"../../res/UserFile/Item/Shotgun_A.bin");
    LCharacterIO::GetInstance().ItemRead(L"../../res/UserFile/Item/OneHandSword.bin");
    LCharacterIO::GetInstance().ItemRead(L"../../res/UserFile/Item/HeroBlade.bin");
    // form
    LExportIO::GetInstance().ExportRead(L"RightHandForm.bin");
    LExportIO::GetInstance().ExportRead(L"PistolPos.bin");
    LExportIO::GetInstance().ExportRead(L"OneHandForm.bin");
    LExportIO::GetInstance().ExportRead(L"TwoHandForm.bin");

    // PlayerSetting
    InitializeWeapon();
    PlayerInit(PlayerType::GUN);
    //PlayerInit(PlayerType::SWORD);

    m_Select = new LSelect;
}

void InGameScene::ResetWeapon()
{
    LWeapon* pistol = LWeaponMgr::GetInstance().GetPtr(WeaponState::PISTOL);
    pistol->m_GunSpec.TotalAmmo = pistol->m_GunSpec.defaultTotalAmmo;
    pistol->m_GunSpec.ShootDelay = pistol->m_GunSpec.defaultShootDelay;
    pistol->m_GunSpec.Damage = pistol->m_GunSpec.defaultDamage;
    pistol->m_GunSpec.CurrentAmmo = pistol->m_GunSpec.TotalAmmo;
    pistol->m_GunSpec.HasWeapon = true;
    pistol->m_GunSpec.MagazineLevel = 0;
    pistol->m_GunSpec.DamageLevel = 0;
    pistol->m_GunSpec.RPMLevel = 0;
    LWeapon* rifle = LWeaponMgr::GetInstance().GetPtr(WeaponState::ASSAULTRIFLE);
    rifle->m_GunSpec.TotalAmmo = rifle->m_GunSpec.defaultTotalAmmo;
    rifle->m_GunSpec.ShootDelay = rifle->m_GunSpec.defaultShootDelay;
    rifle->m_GunSpec.Damage = rifle->m_GunSpec.defaultDamage;
    rifle->m_GunSpec.CurrentAmmo = rifle->m_GunSpec.TotalAmmo;
    rifle->m_GunSpec.HasWeapon = false;
    rifle->m_GunSpec.MagazineLevel = 0;
    rifle->m_GunSpec.DamageLevel = 0;
    rifle->m_GunSpec.RPMLevel = 0;
    LWeapon* shotgun = LWeaponMgr::GetInstance().GetPtr(WeaponState::SHOTGUN);
    shotgun->m_GunSpec.TotalAmmo = shotgun->m_GunSpec.defaultTotalAmmo;
    shotgun->m_GunSpec.ShootDelay = shotgun->m_GunSpec.defaultShootDelay;
    shotgun->m_GunSpec.Damage = shotgun->m_GunSpec.defaultDamage;
    shotgun->m_GunSpec.CurrentAmmo = shotgun->m_GunSpec.TotalAmmo;
    shotgun->m_GunSpec.HasWeapon = false;
    shotgun->m_GunSpec.MagazineLevel = 0;
    shotgun->m_GunSpec.DamageLevel = 0;
    shotgun->m_GunSpec.RPMLevel = 0;
    LWeapon* oneHandSword = LWeaponMgr::GetInstance().GetPtr(WeaponState::ONEHANDSWORD);
    oneHandSword->m_SwordSpec.SlashSpeed = oneHandSword->m_SwordSpec.defaultSlashSpeed;
    oneHandSword->m_SwordSpec.Damage = oneHandSword->m_SwordSpec.defaultDamage;
    oneHandSword->m_SwordSpec.RushDamage = oneHandSword->m_SwordSpec.defaultRushDamage;
    oneHandSword->m_SwordSpec.HasWeapon = false;
    LWeapon* twoHandSword = LWeaponMgr::GetInstance().GetPtr(WeaponState::TWOHANDSWORD);
    twoHandSword->m_SwordSpec.SlashSpeed = twoHandSword->m_SwordSpec.defaultSlashSpeed;
    twoHandSword->m_SwordSpec.Damage = twoHandSword->m_SwordSpec.defaultDamage;
    twoHandSword->m_SwordSpec.RushDamage = twoHandSword->m_SwordSpec.defaultRushDamage;
    twoHandSword->m_SwordSpec.HasWeapon = false;
    
}

void InGameScene::NextWave()
{
    m_ZombieWave->m_CurrentWave++;

    if (m_ZombieWave->m_CurrentWave > MAXWAVE)
    {
        IsEndGame = true;
        return;
    }

    int zombieCount = m_ZombieWave->m_WaveZombieCountList[m_ZombieWave->m_CurrentWave];
    m_ZombieWave->m_EnemyMap["Zombie"].resize(zombieCount);

    int tankCount = m_ZombieWave->m_WaveTankCountList[m_ZombieWave->m_CurrentWave];
    m_ZombieWave->m_EnemyMap["Tank"].resize(tankCount);

    int bossCount = m_ZombieWave->m_WaveBossCountList[m_ZombieWave->m_CurrentWave];
    m_ZombieWave->m_EnemyMap["Boss"].resize(bossCount);

    m_ZombieWave->m_EnemyMap["LNPC"].resize(zombieCount + tankCount + bossCount);
    
    for (int i = 0; i < zombieCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Zombie"][i] = std::make_shared<Zombie>(LGlobal::g_PlayerModel);
        m_ZombieWave->m_EnemyMap["Zombie"][i]->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Zombie.fbx");
        m_ZombieWave->m_EnemyMap["Zombie"][i]->CreateBoneBuffer();
        m_ZombieWave->m_EnemyMap["Zombie"][i]->FSM(FSMType::ENEMY);
        
        m_ZombieWave->m_EnemyMap["Zombie"][i]->m_matControl._41 = m_ZombieWave->GetRandomNumber();
        m_ZombieWave->m_EnemyMap["Zombie"][i]->m_matControl._43 = m_ZombieWave->GetRandomNumber();

        m_ZombieWave->m_EnemyMap["LNPC"][i] = m_ZombieWave->m_EnemyMap["Zombie"][i];
    }

    if (m_ZombieWave->IsFirstCreate)
    {
        m_ZombieWave->m_EnemyMap["Zombie"][0]->ComputeOffset();
    }
   
    for (int i = 0; i < tankCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Tank"][i] = std::make_shared<Tank>(LGlobal::g_PlayerModel);
        m_ZombieWave->m_EnemyMap["Tank"][i]->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Tank.fbx");
        m_ZombieWave->m_EnemyMap["Tank"][i]->CreateBoneBuffer();
        m_ZombieWave->m_EnemyMap["Tank"][i]->FSM(FSMType::TANK);

        m_ZombieWave->m_EnemyMap["Tank"][i]->m_matControl._41 = m_ZombieWave->GetRandomNumber();
        m_ZombieWave->m_EnemyMap["Tank"][i]->m_matControl._43 = m_ZombieWave->GetRandomNumber();

        m_ZombieWave->m_EnemyMap["LNPC"][zombieCount + i] = m_ZombieWave->m_EnemyMap["Tank"][i];
    }

    for (int i = 0; i < bossCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Boss"][i] = std::make_shared<Boss>(LGlobal::g_PlayerModel);
        m_ZombieWave->m_EnemyMap["Boss"][i]->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Boss.fbx");
        m_ZombieWave->m_EnemyMap["Boss"][i]->CreateBoneBuffer();
        m_ZombieWave->m_EnemyMap["Boss"][i]->FSM(FSMType::BOSS);

        m_ZombieWave->m_EnemyMap["Boss"][i]->m_matControl._41 = m_ZombieWave->GetRandomNumber();
        m_ZombieWave->m_EnemyMap["Boss"][i]->m_matControl._43 = m_ZombieWave->GetRandomNumber();

        m_ZombieWave->m_EnemyMap["LNPC"][zombieCount + tankCount + i] = m_ZombieWave->m_EnemyMap["Boss"][i];
    }

    for (int i = 0; i < zombieCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Zombie"][i]->m_Player = LGlobal::g_PlayerModel;
        m_ZombieWave->m_EnemyMap["Zombie"][i]->SetAnimationArrayTexture();
        m_ZombieWave->m_EnemyMap["Zombie"][i]->SetAnimationArraySRV();
        m_ZombieWave->m_EnemyMap["Zombie"][i]->CreateCurrentFrameBuffer();
    }

    // Collision
    std::wstring head = L"Head";
    std::wstring root = L"root";
    std::wstring shoulder = L"RightShoulder";
    std::wstring hand = L"RightHand";
    std::wstring leftShoulder = L"LeftShoulder";
    std::wstring leftHand = L"LeftHand";

    TMatrix Head = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][head];
    TMatrix Root = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][root];
    TMatrix RightShoulder = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][shoulder];
    TMatrix RightHand = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][hand];

    TMatrix TankHead;
    TMatrix TankRoot;
    TMatrix TankRightShoulder;
    TMatrix TankRightHand;

    TMatrix BossHead;
    TMatrix BossRoot;
    TMatrix BossRightShoulder;
    TMatrix BossRightHand;
    TMatrix BossLeftShoulder;
    TMatrix BossLeftHand;

    if (tankCount != 0)
    {
        TankHead = m_ZombieWave->m_EnemyMap["Tank"][0]->m_pModel->m_NameMatrixMap[0][head];
        TankRoot = m_ZombieWave->m_EnemyMap["Tank"][0]->m_pModel->m_NameMatrixMap[0][root];
        TankRightShoulder = m_ZombieWave->m_EnemyMap["Tank"][0]->m_pModel->m_NameMatrixMap[0][shoulder];
        TankRightHand = m_ZombieWave->m_EnemyMap["Tank"][0]->m_pModel->m_NameMatrixMap[0][hand];
    }

    if (bossCount != 0)
    {
        BossHead = m_ZombieWave->m_EnemyMap["Boss"][0]->m_pModel->m_NameMatrixMap[0][head];
        BossRoot = m_ZombieWave->m_EnemyMap["Boss"][0]->m_pModel->m_NameMatrixMap[0][root];
        BossRightShoulder = m_ZombieWave->m_EnemyMap["Boss"][0]->m_pModel->m_NameMatrixMap[0][shoulder];
        BossRightHand = m_ZombieWave->m_EnemyMap["Boss"][0]->m_pModel->m_NameMatrixMap[0][hand];
        BossLeftShoulder = m_ZombieWave->m_EnemyMap["Boss"][0]->m_pModel->m_NameMatrixMap[0][leftShoulder];
        BossLeftHand = m_ZombieWave->m_EnemyMap["Boss"][0]->m_pModel->m_NameMatrixMap[0][leftHand];
    }
 
    for (int i = 0; i < zombieCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Zombie"][i]->SetOBBBox({ -20.0f, Root._42, -5.0f }, { 20.0f, Head._42, 30.0f }, 0.2f);
        m_ZombieWave->m_EnemyMap["Zombie"][i]->SetOBBBoxRightHand({ RightHand._41, RightHand._42, -20.0f }, { RightShoulder._41, RightShoulder._42, 40.0f }, 0.2f);
    }

    for (int i = 0; i < tankCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Tank"][i]->SetOBBBox({ -40.0f, TankRoot._42, -5.0f }, { 40.0f, TankHead._42, 30.0f }, 0.2f);
        m_ZombieWave->m_EnemyMap["Tank"][i]->SetOBBBoxRightHand({ TankRightHand._41 - 40.0f, TankRightHand._42 - 40.0f, -40.0f }, { TankRightShoulder._41 + 40.0f, TankRightShoulder._42 + 40.0f, 40.0f }, 0.2f);
    }

    for (int i = 0; i < bossCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Boss"][i]->SetOBBBox({ -100.0f, BossRoot._42, -50.0f }, { 100.0f, BossHead._42 + 180.0f, 50.0f }, 0.4f);
        m_ZombieWave->m_EnemyMap["Boss"][i]->SetOBBBoxRightHand({ BossRightHand._41 - 100.0f, BossRightHand._42 - 100.0f, -100.0f }, { BossRightShoulder._41 + 100.0f, BossRightShoulder._42 + 100.0f, 100.0f }, 0.4f);
        m_ZombieWave->m_EnemyMap["Boss"][i]->SetOBBBoxLeftHand({ BossLeftHand._41 - 100.0f, BossLeftHand._42 - 100.0f, -100.0f }, { BossLeftShoulder._41 + 100.0f, BossLeftShoulder._42 + 100.0f, 100.0f }, 0.6f);
    }

    if (m_ZombieWave->m_CurrentWave > 5) return;

    if (!m_ZombieWave->IsFirstCreate)
    {
        UIManager::GetInstance().GetUIObject(L"EnemyCount")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_EnemyMap["Zombie"].size());
        UIManager::GetInstance().GetUIObject(L"crr_Wave")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_CurrentWave);
    }

    m_ZombieWave->IsFirstCreate = false;
}

void InGameScene::RenderObject()
{
    LGlobal::g_PlayerModel->m_pModel->m_DrawList[0]->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_CBmatShadow.g_matShadow = LGlobal::g_PlayerModel->m_matForAnim * m_matViewLight * m_matProjLight * m_matTexture;
    D3DXMatrixTranspose(&m_CBmatShadow.g_matShadow, &m_CBmatShadow.g_matShadow);
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pCBShadow.Get(), 0, NULL, &m_CBmatShadow, 0, 0);
    LGlobal::g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pCBShadow.GetAddressOf());
    LGlobal::g_PlayerModel->m_pModel->m_DrawList[0]->PreRender();
    ID3D11ShaderResourceView* pSRV[] = { m_RT.m_pSRV.Get() };
    LGlobal::g_pImmediateContext->PSSetShaderResources(1, 1, pSRV);
    LGlobal::g_PlayerModel->Render();
}

void InGameScene::RenderShadow(TMatrix* matWorld, TMatrix* matShadow,
    TMatrix* matView, TMatrix* matProj)
{
    TMatrix matWorldShadow = (*matWorld) * (*matShadow);
    LGlobal::g_PlayerModel->m_pModel->m_DrawList[0]->SetMatrix( &matWorldShadow, matView, matProj);
    LGlobal::g_PlayerModel->Render();
}

void InGameScene::CreateShadowConstantBuffer()
{
    D3D11_BUFFER_DESC bufferDesc{};
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.ByteWidth = sizeof(SHADOW_CONSTANT_BUFFER);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hr = LGlobal::g_pDevice->CreateBuffer(
        &bufferDesc,
        nullptr,
        m_pCBShadow.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create ConstantBuffer Error", "Error Box", MB_OK);
        return;
    }
   
}

void InGameScene::RenderTrail()
{
    if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::ONEHANDSWORD)
    {
        if (LGlobal::g_PlayerModel->m_CurrentState == State::CHARACTERONEHANDSLASH)
        {
            m_SwordTrail->RenderTrail(&m_OneHandSwordLow,
                &m_OneHandSwordHigh,
                &LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl);
        }
    }
    else if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::TWOHANDSWORD)
    {
        if (LGlobal::g_PlayerModel->m_CurrentState == State::CHARACTERTWOHANDSLASH)
        {
            m_SwordTrail->RenderTrail(&m_TwoHandSwordLow,
                &m_TwoHandSwordHigh,
                &LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl);
        }
    }
}

void InGameScene::InterpolRenderTrail()
{
    if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::ONEHANDSWORD)
    {
        if (LGlobal::g_PlayerModel->m_CurrentState == State::CHARACTERONEHANDSLASH)
        {
            m_SwordTrail->InterpolRenderTrail(&m_OneHandSwordLow,
                &m_OneHandSwordHigh,
                &LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl);
        }
    }
    else if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::TWOHANDSWORD)
    {
        if (LGlobal::g_PlayerModel->m_CurrentState == State::CHARACTERTWOHANDSLASH)
        {
            m_SwordTrail->InterpolRenderTrail(&m_TwoHandSwordLow,
                &m_TwoHandSwordHigh,
                &LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl);
        }
    }
}

InGameScene::InGameScene(LScene* parent) : SceneState(parent)
{
    //Init();
}
InGameScene::~InGameScene() {}

ID3D11Buffer* InGameScene::CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic)
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

void InGameScene::InitializeWeapon()
{
    std::shared_ptr<LWeapon> pistol = std::make_shared<LWeapon>();
    pistol->m_WeaponModel = std::make_shared<LModel>();
    pistol->m_WeaponModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Pistols_A.fbx");

    pistol->m_GunSpec.offset = 23.0f;
    pistol->m_GunSpec.defaultTotalAmmo = 20;
    pistol->m_GunSpec.TotalAmmo = 20;
    pistol->m_GunSpec.defaultShootDelay = 0.5f;
    pistol->m_GunSpec.ShootDelay = 0.5f;
    pistol->m_GunSpec.defaultDamage = 50.0f;
    pistol->m_GunSpec.Damage = 50.0f;
    pistol->m_GunSpec.CurrentAmmo = pistol->m_GunSpec.TotalAmmo;

    std::shared_ptr<LWeapon> rifle = std::make_shared<LWeapon>();
    rifle->m_WeaponModel = std::make_shared<LModel>();
    rifle->m_WeaponModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Assault_Rifle_A.fbx");
    rifle->m_GunSpec.offset = 115.0f;
    rifle->m_GunSpec.defaultTotalAmmo = 30;
    rifle->m_GunSpec.TotalAmmo = 30;
    rifle->m_GunSpec.defaultShootDelay = 0.1f;
    rifle->m_GunSpec.ShootDelay = 0.1f;
    rifle->m_GunSpec.defaultDamage = 20.0f;
    rifle->m_GunSpec.Damage = 20.0f;
    rifle->m_GunSpec.CurrentAmmo = rifle->m_GunSpec.TotalAmmo;

    std::shared_ptr<LWeapon> shotGun = std::make_shared<LWeapon>();
    shotGun->m_WeaponModel = std::make_shared<LModel>();
    shotGun->m_WeaponModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Shotgun_A.fbx");
    shotGun->m_GunSpec.offset = 95.0f;
    shotGun->m_GunSpec.defaultTotalAmmo = 7;
    shotGun->m_GunSpec.TotalAmmo = 7;
    shotGun->m_GunSpec.defaultShootDelay = 1.3f;
    shotGun->m_GunSpec.ShootDelay = 1.3f;
    shotGun->m_GunSpec.defaultDamage = 100.0f;
    shotGun->m_GunSpec.Damage = 100.0f;
    shotGun->m_GunSpec.CurrentAmmo = shotGun->m_GunSpec.TotalAmmo;

    std::shared_ptr<LWeapon> oneHandSword = std::make_shared<LWeapon>();
    oneHandSword->m_WeaponModel = std::make_shared<LModel>();
    oneHandSword->m_WeaponModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"OneHandSword.fbx");
    oneHandSword->m_SwordSpec.SlashSpeed = 1.0;
    oneHandSword->m_SwordSpec.defaultSlashSpeed = 1.0;
    oneHandSword->m_SwordSpec.Damage = 50.0f;
    oneHandSword->m_SwordSpec.defaultDamage = 50.0f;
    oneHandSword->m_SwordSpec.RushDamage = 30.0f;
    oneHandSword->m_SwordSpec.defaultRushDamage = 30.0f;

    std::shared_ptr<LWeapon> twoHandSword = std::make_shared<LWeapon>();
    twoHandSword->m_WeaponModel = std::make_shared<LModel>();
    twoHandSword->m_WeaponModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"HeroBlade.fbx");
    twoHandSword->m_SwordSpec.SlashSpeed = 0.7;
    twoHandSword->m_SwordSpec.defaultSlashSpeed = 0.7;
    twoHandSword->m_SwordSpec.Damage = 60.0f;
    twoHandSword->m_SwordSpec.defaultDamage = 60.0f;
    twoHandSword->m_SwordSpec.RushDamage = 30.0f;
    twoHandSword->m_SwordSpec.defaultRushDamage = 30.0f;
 
    LWeaponMgr::GetInstance().Add(WeaponState::PISTOL, pistol);
    LWeaponMgr::GetInstance().Add(WeaponState::ASSAULTRIFLE, rifle);
    LWeaponMgr::GetInstance().Add(WeaponState::SHOTGUN, shotGun);
    LWeaponMgr::GetInstance().Add(WeaponState::ONEHANDSWORD, oneHandSword);
    LWeaponMgr::GetInstance().Add(WeaponState::TWOHANDSWORD, twoHandSword);
}

void InGameScene::InitializeObjects()
{

}

void InGameScene::InitializeSkyBox()
{
    m_SkyBox = std::make_shared<LSkyBox>();
    m_SkyBox->Set();
    //m_SkyBox->Create(L"../../res/hlsl/SkyBox.hlsl", L"../../res/sky/grassenvmap1024.dds");
    m_SkyBox->Create(L"../../res/hlsl/SkyBox.hlsl", L"../../res/sky/skycubemap2.dds");
}

void InGameScene::InitializePlayerIcon()
{
    m_playerIcon = std::make_shared<KObject>();
    m_playerIcon->Init();
    m_playerIcon->SetPos({ 0, 0, -1 });
    m_playerIcon->SetScale({ 10,10, 1 });
    m_playerIcon->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/PlayerIcon.png");
}

void InGameScene::InitializeBloodSplatters()
{
    m_bloodSplatter.resize(30);
    for (auto& blood : m_bloodSplatter)
    {
        blood = std::make_shared<KObject>();
        blood->Init();
        blood->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/Empty.png");
        blood->SetPos({ 0, 10000, 0 });
        float x = (rand() % 5) + 5.f;
        blood->SetScale({ x , 15 - x, 1 });
        blood->m_vScale = { x , 15 - x, 1 };
        blood->AddScripts(std::make_shared<Animator>(L"Anim.xml"));
        blood->SetIsRender(false);
    }
}


void InGameScene::InitializeTrees()
{
    m_TreeList.resize(30);
    auto treeObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/tree/Tree.fbx", L"../../res/hlsl/LightShadowMap.hlsl");
    for (auto& tree : m_TreeList)
    {
        tree = std::make_shared<LModel>();
        tree->SetLFbxObj(treeObj);
        tree->CreateBoneBuffer();
        InitializeTreePosition(tree);
    }
}

void InGameScene::InitializeTreePosition(std::shared_ptr<LModel>& tree)
{
    DirectX::XMMATRIX rotationMatrix, scalingMatrix, worldMatrix, translationMatrix, ratationYMatrix;
    float x = (rand() % 1800) - 900;
    float z = (rand() % 1800) - 900;
    float y = (rand() % 360);
    float scale = (rand() % 150) + 30.f;
    translationMatrix = DirectX::XMMatrixTranslation(x, m_CustomMap->GetHeight(x, z), z);
    rotationMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(270.0f));
    // 나무 Y축회전 랜덤 부여
    ratationYMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(y));
    rotationMatrix *= ratationYMatrix;
    scalingMatrix = DirectX::XMMatrixScaling(scale, scale, scale);
    worldMatrix = DirectX::XMMatrixIdentity();
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);
    tree->m_matControl = worldMatrix;
    tree->m_fRadius = scale / 110 * 16;
}

void InGameScene::InitializeWalls()
{
    m_WallList.resize(40);
    wallObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/wall/Concrete wall.fbx", L"../../res/hlsl/LightShadowMap.hlsl");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            m_WallList[i * 10 + j] = std::make_shared<LModel>();
            m_WallList[i * 10 + j]->SetLFbxObj(wallObj);
            m_WallList[i * 10 + j]->CreateBoneBuffer();

            DirectX::XMMATRIX rotationMatrix, translationMatrix, worldMatrix, scalingMatrix;
            float rotationAngle;

            // 위치에 따라 회전 각도를 설정
            if (i == 0) // 북쪽 벽
            {
                rotationAngle = 90.0f;
                rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
                translationMatrix = DirectX::XMMatrixTranslation(-1000.0f, 0.0f, -1000.0f + 200.0f * j);
            }
            else if (i == 1) // 동쪽 벽
            {
                rotationAngle = 180.0f;
                rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
                translationMatrix = DirectX::XMMatrixTranslation(-1000.0f + 200.0f * j, 0.0f, 1000.0f);
            }
            else if (i == 2) // 남쪽 벽
            {
                rotationAngle = 270.0f;
                rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
                translationMatrix = DirectX::XMMatrixTranslation(1000.0f, 0.0f, 1000.0f - 200.0f * j);
            }
            else // 서쪽 벽
            {
                rotationAngle = 0.0f;
                rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
                translationMatrix = DirectX::XMMatrixTranslation(1000.0f - 200.0f * j, 0.0f, -1000.0f);
            }

            scalingMatrix = DirectX::XMMatrixScaling(1.0f, 2.0f, 1.0f);
            worldMatrix = DirectX::XMMatrixIdentity();
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);

            m_WallList[i * 10 + j]->m_matControl = worldMatrix;
        }
    }
}

void InGameScene::InitializeWallPosition(std::shared_ptr<LModel>& wall, int i, int j)
{
    DirectX::XMMATRIX rotationMatrix, translationMatrix, worldMatrix, scalingMatrix;
    float rotationAngle;
    if (i == 0) rotationAngle = 90.0f;
    else if (i == 1) rotationAngle = 180.0f;
    else if (i == 2) rotationAngle = 270.0f;
    else rotationAngle = 0.0f;

    rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
    translationMatrix = DirectX::XMMatrixTranslation(-1000.0f + 200.0f * j, 0.0f, 1000.0f - 200.0f * i);
    scalingMatrix = DirectX::XMMatrixScaling(1.0f, 2.0f, 1.0f);
    worldMatrix = DirectX::XMMatrixIdentity();
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);
    wall->m_matControl = worldMatrix;
}

void InGameScene::InitializeBullets()
{
    auto bulletObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/bullet/Tennis.fbx", L"../../res/hlsl/Bullet.hlsl");

    // rifle
    m_RifleBulletList.resize(200);
    for (int i = 0; i < m_RifleBulletList.size(); ++i)
    {
        m_RifleBulletList[i] = std::make_shared<Bullet>();
        m_RifleBulletList[i]->bVisible = false;
        m_RifleBulletList[i]->SetLFbxObj(bulletObj);
        m_RifleBulletList[i]->CreateBoneBuffer();
        DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(0.03f, 0.03f, 0.033f);

        m_RifleBulletList[i]->m_matControl = scalingMatrix * LGlobal::g_PlayerModel->m_matControl;
        m_RifleBulletList[i]->m_fRadius = 3.f;
    }

    // shotgun
    m_ShotgunBulletListArray.resize(50);
    for (int iList = 0; iList < m_ShotgunBulletListArray.size(); ++iList)
    {
        m_ShotgunBulletListArray[iList].resize(8);
        for (int iBullet = 0; iBullet < m_ShotgunBulletListArray[iList].size(); ++iBullet)
        {
            m_ShotgunBulletListArray[iList][iBullet] = make_shared<Bullet>();
            m_ShotgunBulletListArray[iList][iBullet]->bVisible = false;
            m_ShotgunBulletListArray[iList][iBullet]->SetLFbxObj(bulletObj);
            m_ShotgunBulletListArray[iList][iBullet]->CreateBoneBuffer();
            DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(0.03f, 0.03f, 0.03f);
            m_ShotgunBulletListArray[iList][iBullet]->m_matControl = scalingMatrix * LGlobal::g_PlayerModel->m_matControl;
            m_ShotgunBulletListArray[iList][iBullet]->m_fRadius = 5.f;
        }
    }
}


void InGameScene::InitializeLighting()
{
    m_pConstantBufferLight[0].Attach(CreateConstantBuffer(
        LGlobal::g_pDevice.Get(), &m_cbLight1, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
    m_pConstantBufferLight[1].Attach(CreateConstantBuffer(
        LGlobal::g_pDevice.Get(), &m_cbLight2, 1, sizeof(LIGHT_CONSTANT_BUFFER2)));
    float fLightRange = 50.0f;
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

void InGameScene::InitializeShadow()
{
    m_pQuad.Set();
    m_pQuad.SetScreenVertex(0, 0, 250, 250, TVector2(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight));
    m_pQuad.Create(L"../../res/hlsl/ShadowMap.hlsl", L"../../res/map/castle.jpg");

    m_RT.Create(1024, 1024);

    m_matTexture = TMatrix(0.5f, 0.0f, 0.0f, 0.0f
        , 0.0f, -0.5f, 0.0f, 0.0f
        , 0.0f, 0.0f, 1.0f, 0.0f
        , 0.5f, 0.5f, 0.0f, 1.0f);


}

void InGameScene::InitializeMap()
{
    m_CustomMap = std::make_shared<LHeightMap>();
    m_CustomMap->Set();
    m_CustomMap->CreateHeightMap(L"../../res/Heightmap/heightMap513.bmp");
    //map
    LMapDesc MapDesc = {};
    MapDesc.iNumCols = m_CustomMap->m_iNumCols;
    MapDesc.iNumRows = m_CustomMap->m_iNumRows;
    MapDesc.fCellDistance = 4.0f;
    MapDesc.fScaleHeight = 0.4f;
    MapDesc.ShaderFilePath = L"../../res/hlsl/LightShadowMap.hlsl";
    MapDesc.TextureFilePath = L"../../res/map/aerial_grass_rock_diff_8k.jpg";
    m_CustomMap->Load(MapDesc);
}

void InGameScene::InitializeGrasses()
{
    m_GrassList.resize(110);
    grassObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/grass/Grass_green.fbx", L"../../res/hlsl/LightShadowMap.hlsl");
    for (auto& grass : m_GrassList)
    {
        grass = std::make_shared<LModel>();
        grass->SetLFbxObj(grassObj);
        grass->CreateBoneBuffer();
        {
            DirectX::XMMATRIX rotationMatrix, scalingMatrix, worldMatrix, translationMatrix;

            // make translation matrix randomly ( -1000 ~ 1000 )
            float x = (rand() % 1800) - 900;
            float z = (rand() % 1800) - 900;
            float y = m_CustomMap->GetHeight(x, z) + ((rand() % 5) + 5);

            translationMatrix = DirectX::XMMatrixTranslation(x, y, z);
            rotationMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(270.0f));
            scalingMatrix = DirectX::XMMatrixScaling(20.0f, 20.0f, 20.0f);
            worldMatrix = DirectX::XMMatrixIdentity();
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);
            grass->m_matControl = worldMatrix;
        }
    }
}

void InGameScene::InitializeMinimap()
{
    m_ShapeMinimap.Set();
    m_ShapeMinimap.SetScreenVertex(10, 10, 256, 256, TVector2(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight));
    m_ShapeMinimap.Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/Hud_Box_128x64.png");
    m_rtMinimap.Create(256, 256);
}

void InGameScene::InitializeMuzzleFlash()
{
    //muzzleflash
    m_muzzleFlash = make_shared<KObject>();
    m_muzzleFlash->Init();
    m_muzzleFlash->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/muzzleflash.png");
    m_muzzleFlash->SetScale({ 10,10,1.f });
    m_muzzleFlash->SetPos({ 0,30,0 });
    // m_muzzleFlash->AddScripts(make_shared<BillBoard>());
}


///////////////////process/////////////////////

void InGameScene::ProcessMuzzleFlash()
{
    //muzzle Frame
    TMatrix matRotation, matTrans, matScale, worldMat;
    matScale = TMatrix::Identity;
    D3DXMatrixInverse(&matRotation, nullptr, &LGlobal::g_pMainCamera->m_matView);
    matRotation._41 = 0.0f;
    matRotation._42 = 0.0f;
    matRotation._43 = 0.0f;
    matRotation._44 = 1.0f;
    TVector3 foward;
    foward = LGlobal::g_PlayerModel->m_matControl.Forward() * LGlobal::g_PlayerModel->m_Gun->m_GunSpec.offset;
    TVector3 vTrans = LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->GetPosition();
    vTrans = vTrans + foward;
    D3DXMatrixTranslation(&matTrans, vTrans.x,
        vTrans.y,
        vTrans.z
    );

    D3DXMatrixScaling(&matScale, m_muzzleFlash->m_vScale.x,
        m_muzzleFlash->m_vScale.y,
        m_muzzleFlash->m_vScale.z
    );
    worldMat = matScale * matRotation * matTrans;
    m_muzzleFlash->SetMatrix(&worldMat, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    // m_muzzleFlash->SetPos({ m_GunModel->m_matControl._41,m_GunModel->m_matControl._42 ,m_GunModel->m_matControl._43 });
    // m_muzzleFlash->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_muzzleFlash->Frame();
}

void InGameScene::InitializeItem()
{
    m_KitList.resize(100);
    kitObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/item/medkit.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    for (auto& kit : m_KitList)
    {
		kit = std::make_shared<LModel>();
		kit->SetLFbxObj(kitObj);
		kit->CreateBoneBuffer();
        {
			DirectX::XMMATRIX rotationMatrix, scalingMatrix, worldMatrix, translationMatrix;

			// make translation matrix randomly ( -1000 ~ 1000 )
            float x = (rand() % 180) - 90;
			float z = (rand() % 180) - 90;
			float y = m_CustomMap->GetHeight(x, z) + 5.f;

			translationMatrix = DirectX::XMMatrixTranslation(x, y, z);
			rotationMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(0.0f));
			scalingMatrix = DirectX::XMMatrixScaling(40.0f, 40.0f, 40.0f);
			worldMatrix = DirectX::XMMatrixIdentity();
			worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
			worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
			worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);
			kit->m_matControl = worldMatrix;
            kit->m_fRadius = 40.f;
            kit->bVisible = false;
		}
	}

    // ammo

        m_AmmoList.resize(100);
        ammoObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/item/AmmoBox.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
        for (auto& ammo : m_AmmoList)
        {
            ammo = std::make_shared<LModel>();
            ammo->SetLFbxObj(ammoObj);
            ammo->CreateBoneBuffer();
            {
                DirectX::XMMATRIX rotationMatrix, scalingMatrix, worldMatrix, translationMatrix;

                // make translation matrix randomly ( -1000 ~ 1000 )
                float x = (rand() % 180) - 90;
                float z = (rand() % 180) - 90;
                float y = m_CustomMap->GetHeight(x, z) + 5.f;

                translationMatrix = DirectX::XMMatrixTranslation(x, y, z);
                rotationMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90.0f));
                scalingMatrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
                worldMatrix = DirectX::XMMatrixIdentity();
                worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
                worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
                worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);
                ammo->m_matControl = worldMatrix;
                ammo->m_fRadius = 30.f;
                ammo->bVisible = false;
            }
        }
}

void InGameScene::InitializeTrail()
{
    m_SwordTrail = new LTrail;
    m_SwordTrail->Set();
    // Texture버전
    m_SwordTrail->Create(L"../../res/hlsl/SwordTrailTexture.hlsl", L"../../res/Trail/T_SwipeTrail.png");
    // VertexColor버전
    //m_SwordTrail->Create(L"../../res/hlsl/SwordTrailVertexColor.hlsl", L"../../res/map/topdownmap.jpg");
}

void InGameScene::ProcessBloodSplatter()
{
    for (auto& obj : m_bloodSplatter)
    {
        if (obj->GetIsRender())
        {
            TMatrix matRotation, matTrans, matScale, worldMat;
            matScale = TMatrix::Identity;
            D3DXMatrixInverse(&matRotation, nullptr, &LGlobal::g_pMainCamera->m_matView);
            matRotation._41 = 0.0f;
            matRotation._42 = 0.0f;
            matRotation._43 = 0.0f;
            matRotation._44 = 1.0f;
            TVector3 foward;
            foward = LGlobal::g_PlayerModel->m_matControl.Forward();
            TVector3 vTrans = obj->m_vPosition;
            vTrans = vTrans + (foward * -180);
            D3DXMatrixTranslation(&matTrans, vTrans.x,
                vTrans.y,
                vTrans.z
            );

            D3DXMatrixScaling(&matScale, obj->m_vScale.x,
                obj->m_vScale.y,
                obj->m_vScale.z
            );
            worldMat = matScale * matRotation * matTrans;

            obj->SetMatrix(&worldMat, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
            obj->Frame();
            if (obj->GetScript<Animator>(L"Animator")->_currentKeyframeIndex == 16)
            {
                obj->SetIsRender(false);
            }
        }
    }
}

void InGameScene::CheckPlayerDeath()
{
    if (LGlobal::g_PlayerModel->IsDeath)
    {
        IsEndGame = true;

    }
}

void InGameScene::PlayInGameSound()
{
    LSoundMgr::GetInstance().GetPtr(L"InGameSound.mp3")->Play();
}

void InGameScene::UpdateUI()
{
    if (Init_2)
    {
        UIManager::GetInstance().GetUIObject(L"total_Wave")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_WaveZombieCountList.size());
        UIManager::GetInstance().GetUIObject(L"crr_Wave")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_CurrentWave);
        UIManager::GetInstance().GetUIObject(L"EnemyCount")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_EnemyMap["Zombie"].size());
        UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);


            UIManager::GetInstance().GetUIObject(L"Gun1_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
            UIManager::GetInstance().GetUIObject(L"Gun2_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
            UIManager::GetInstance().GetUIObject(L"Gun3_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
            UIManager::GetInstance().GetUIObject(L"Gun1_DAMAGE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
            UIManager::GetInstance().GetUIObject(L"Gun2_DAMAGE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
            UIManager::GetInstance().GetUIObject(L"Gun3_DAMAGE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
            UIManager::GetInstance().GetUIObject(L"Gun1_MAGAZINE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
            UIManager::GetInstance().GetUIObject(L"Gun2_MAGAZINE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
            UIManager::GetInstance().GetUIObject(L"Gun3_MAGAZINE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"100$");
             UIManager::GetInstance().GetUIObject(L"face")->GetScript<UIEvent>(L"UIEvent")->UpdatePlayerFace();

             

            UIManager::GetInstance().GetUIObject(L"HPbar")->GetScript<HpBar>(L"HpBar")->UpdateHp();
        Init_2 = false;
    }
}

void InGameScene::ProcessWaveTransition()
{
    if (IsNextWave)
    {
        IsNextWave = false;
        NextWave();
    }
}

void InGameScene::UpdateMapObjects()
{
    //m_MapModel->Frame();
    m_CustomMap->Frame();
}

void InGameScene::UpdateWallModels()
{
    for (int i = 0; i < m_WallList.size(); i++)
    {
        m_WallList[i]->Frame();
    }
}

void InGameScene::UpdateTreeModels()
{
    for (auto& tree : m_TreeList)
    {
        tree->Frame();
    }
}

void InGameScene::UpdateBulletModels()
{
    //rifle
    for (int i = 0; i < m_RifleBulletList.size(); i++)
    {
        if (m_RifleBulletList[i]->bVisible == false)
            continue;

        m_RifleBulletList[i]->Frame();
        if (m_RifleBulletList[i]->target)
        {
            if (m_RifleBulletList[i]->target->IsDead)
            {
                TVector3 forward = m_RifleBulletList[i]->m_matControl.Forward();
                AutoTargetBullet(forward, i, m_RifleBulletList[i]->GetPosition());
            }
        }
        /*if (m_RifleBulletList[i]->m_matControl._41 > 1000.f
            || m_RifleBulletList[i]->m_matControl._41 < -1000.f
            || m_RifleBulletList[i]->m_matControl._43 > 1000.f
            || m_RifleBulletList[i]->m_matControl._43 < -1000.f
            || m_RifleBulletList[i]->m_matControl._42 > 300.f
            || m_RifleBulletList[i]->m_matControl._42 < m_CustomMap->GetHeight(m_RifleBulletList[i]->m_matControl._41, m_RifleBulletList[i]->m_matControl._43))
        {
            m_RifleBulletList[i]->bVisible = false;
        }*/

        m_RifleBulletList[i]->lifeStart += LGlobal::g_fSPF;
        if (m_RifleBulletList[i]->lifeStart > m_RifleBulletList[i]->lifeTime)
            m_RifleBulletList[i]->bVisible = false;

        for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
        {
            if (zombie->IsDead)
                continue;
            if (zombie->m_OBBBox.IsSphereInBox(m_RifleBulletList[i]->GetPosition(), m_RifleBulletList[i]->m_fRadius))
            {
                if (m_RifleBulletList[i]->m_matControl._42 > (zombie->m_matControl._42 + zombie->m_OBBBox.fTall * 0.85))
                {
                    zombie->IsHeadShot = true;
                }
                else
                {
                    zombie->IsHeadShot = false;
                }
                zombie->IsTakeDamage = true;
                m_bloodSplatter[m_crrBlood]->SetPos(m_RifleBulletList[i]->GetPosition() + m_RifleBulletList[i]->m_matControl.Forward() * 150);
                m_bloodSplatter[m_crrBlood]->GetScript<Animator>(L"Animator")->_currentKeyframeIndex = 0;
                m_bloodSplatter[m_crrBlood]->SetIsRender(true);
                m_crrBlood++;
                if (m_crrBlood == m_bloodSplatter.size())
                    m_crrBlood = 0;
                m_RifleBulletList[i]->bVisible = false;
            }
        }
    }

    //shotgun
    for (int i = 0; i < m_ShotgunBulletListArray.size(); i++)
    {
        for (int j = 0; j < m_ShotgunBulletListArray[i].size(); j++)
        {
            if (m_ShotgunBulletListArray[i][j]->bVisible == false)
            {
                continue;
            }
            m_RifleBulletList[i]->lifeStart += LGlobal::g_fSPF;
            if (m_RifleBulletList[i]->lifeStart > m_RifleBulletList[i]->lifeTime)
                m_RifleBulletList[i]->bVisible = false;
            m_ShotgunBulletListArray[i][j]->Frame();
            m_ShotgunBulletListArray[i][j]->m_matControl._41 += m_ShotgunBulletListArray[i][j]->m_matControl.Forward().x * 100.f;
            m_ShotgunBulletListArray[i][j]->m_matControl._42 += m_ShotgunBulletListArray[i][j]->m_matControl.Forward().y * 100.f;
            m_ShotgunBulletListArray[i][j]->m_matControl._43 += m_ShotgunBulletListArray[i][j]->m_matControl.Forward().z * 100.f;
            /*if (m_ShotgunBulletListArray[i][j]->m_matControl._41 > 1000.f
                || m_ShotgunBulletListArray[i][j]->m_matControl._41 < -1000.f
                || m_ShotgunBulletListArray[i][j]->m_matControl._43 > 1000.f
                || m_ShotgunBulletListArray[i][j]->m_matControl._43 < -1000.f
                || m_ShotgunBulletListArray[i][j]->m_matControl._42 > 300.f
                || m_ShotgunBulletListArray[i][j]->m_matControl._42 < m_CustomMap->GetHeight(m_ShotgunBulletListArray[i][j]->m_matControl._41, m_ShotgunBulletListArray[i][j]->m_matControl._43))
            {
                m_ShotgunBulletListArray[i][j]->bVisible = false;
            }*/
            for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
            {
                if (zombie->IsDead)
					continue;
                if (zombie->m_OBBBox.IsSphereInBox(m_ShotgunBulletListArray[i][j]->GetPosition(), m_ShotgunBulletListArray[i][j]->m_fRadius))
                {
                    if (m_ShotgunBulletListArray[i][j]->m_matControl._42 > ((zombie->m_matControl._42 + zombie->m_OBBBox.fTall) * 0.85))
                    {
                        zombie->IsHeadShot = true;
                    }
                    else
                    {
                        zombie->IsHeadShot = false;
                    }
                    zombie->IsTakeDamage = true;
                    m_bloodSplatter[m_crrBlood]->SetPos(m_ShotgunBulletListArray[i][j]->GetPosition() + m_ShotgunBulletListArray[i][j]->m_matControl.Forward() * 150);
                    m_bloodSplatter[m_crrBlood]->GetScript<Animator>(L"Animator")->_currentKeyframeIndex = 0;
                    m_bloodSplatter[m_crrBlood]->SetIsRender(true);
                    m_crrBlood++;
                    if (m_crrBlood == m_bloodSplatter.size())
                        m_crrBlood = 0;
                    m_ShotgunBulletListArray[i][j]->bVisible = false;
                }
            }

        }
    }

}

void InGameScene::AdjustPlayerHeight()
{

    float fHeight = m_CustomMap->GetHeight(LGlobal::g_PlayerModel->m_matControl._41, LGlobal::g_PlayerModel->m_matControl._43);
    if (LGlobal::g_PlayerModel->IsOnAir == false)
    {
        LGlobal::g_PlayerModel->m_matControl._42 = fHeight + 1.0f;
    }
    if (LGlobal::g_PlayerModel->m_matControl._42 < fHeight)
    {
		LGlobal::g_PlayerModel->m_matControl._42 = fHeight + 1.0f;
        LGlobal::g_PlayerModel->IsOnAir = false;
        GPLAYER->m_Speed = 0.f;
	}
}

void InGameScene::AdjustNpcHeight()
{
    for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
    {
		float fHeight = m_CustomMap->GetHeight(zombie->m_matControl._41, zombie->m_matControl._43);
        if (zombie->IsOnAir == false)
        {
			zombie->m_matControl._42 = fHeight + 1.0f;
		}
        if (zombie->m_matControl._42 < fHeight)
        {
			zombie->m_matControl._42 = fHeight + 1.0f;
			zombie->IsOnAir = false;
		}

        zombie->m_MapHeight = fHeight + 1.0f;
        //for (auto& zombie : m_ZombieWave->m_EnemyMap["Zombie"])
        //{
        //    float fHeight = m_CustomMap->GetHeight(zombie->m_matControl._41, zombie->m_matControl._43);
        //    zombie->m_matControl._42 = fHeight + 1.0f;
        //}

        //for (auto& tank : m_ZombieWave->m_EnemyMap["Tank"])
        //{
        //    float fHeight = m_CustomMap->GetHeight(tank->m_matControl._41, tank->m_matControl._43);
        //    tank->m_matControl._42 = fHeight + 1.0f;
        //}
	}
}

void InGameScene::RenderItem()
{
    for (auto& item : m_KitList)
    {
        if (item->bVisible)
            item->Render();
    }
    if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
    {
        for (auto& item : m_AmmoList)
        {
            if (item->bVisible)
                item->Render();
        }
    }
}

void InGameScene::RenderBullets()
{
    // rifle
    for (int i = 0; i < m_RifleBulletList.size(); i++)
    {
        if (m_RifleBulletList[i]->bVisible)
        {
            m_RifleBulletList[i]->Render();
        }
    }

    // shotgun
    for (int i = 0; i < m_ShotgunBulletListArray.size(); i++)
    {
        for (int j = 0; j < m_ShotgunBulletListArray[i].size(); j++)
        {
            if (m_ShotgunBulletListArray[i][j]->bVisible)
            {
                m_ShotgunBulletListArray[i][j]->Render();
            }
        }
    }

}
void InGameScene::SwitchCameraView()
{
    if (LInput::GetInstance().m_KeyStateOld[DIK_1] > KEY_PUSH)
    {
        LGlobal::g_pMainCamera = m_DebugCamera.get();
    }

    if (LInput::GetInstance().m_KeyStateOld[DIK_2] > KEY_PUSH)
    {
        LGlobal::g_pMainCamera = m_ModelCamera.get();
    }

    if (LInput::GetInstance().m_KeyStateOld[DIK_3] > KEY_PUSH)
    {
        LGlobal::g_pMainCamera = m_BackViewCamera.get();
    }
}

void InGameScene::UpdateCameraTargetPosition()
{
    m_ModelCamera->SetTargetPos(TVector3(LGlobal::g_PlayerModel->m_matControl._41 + 20, LGlobal::g_PlayerModel->m_matControl._42 + 15, LGlobal::g_PlayerModel->m_matControl._43));
}

void InGameScene::FramePlayerModel()
{
    LGlobal::g_PlayerModel->m_matForAnim = LGlobal::g_PlayerModel->m_matControl;
    LGlobal::g_PlayerModel->Frame();

    if (LGlobal::g_PlayerModel->IsShoot)
    {
        ShootBullet();
        ProcessMuzzleFlash();
        fLightStart = 0.0f;
    }
    LGlobal::g_PlayerModel->Process();
}
 
void InGameScene::ProcessItem()
{
    for (auto& kit : m_KitList)
    {
        kit->Frame();
    }
    if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
    {
        for (auto& ammo : m_AmmoList)
        {
            ammo->Frame();
        }
    }
}

void InGameScene::GetItem()
{
    
}

void InGameScene::GenerateItem()
{
    if (GPLAYER->m_ZedTimeCount % 20 == 0)
    {
        for (auto& kit : m_KitList)
        {
            if (kit->bVisible == false)
            {
				kit->bVisible = true;
                kit->SetPosition(kit->GetPosition() + GPLAYER->GetPosition());
                kit->m_matControl._42 = m_CustomMap->GetHeight(kit->m_matControl._41, kit->m_matControl._43) + 1.0f;
				break;
			}
        }
        GPLAYER->m_ZedTimeCount++;
    }

    if (GPLAYER->m_ZedTimeCount % 20 == 1)
    {
        for (auto& ammo : m_AmmoList)
        {
            if (ammo->bVisible == false)
            {
                ammo->bVisible = true;
                ammo->SetPosition(ammo->GetPosition() + GPLAYER->GetPosition());
                ammo->m_matControl._42 = m_CustomMap->GetHeight(ammo->m_matControl._41, ammo->m_matControl._43) + 1.0f;
                break;
            }
        }
        GPLAYER->m_ZedTimeCount++;
    }

    
}

void InGameScene::ShootBullet()
{
    if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::PISTOL ||
        LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::ASSAULTRIFLE)
    {
        ShootRifle();
    }
    else if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::SHOTGUN)
    {
        ShootShotgun();
    }
    
}
void InGameScene::ShootRifle()
{
    int index = LGlobal::g_PlayerModel->m_Gun->m_GunSpec.CurrentAmmo;
    m_RifleBulletList[index]->bVisible = true;
    m_RifleBulletList[index]->bTarget = false;
    m_RifleBulletList[index]->target = nullptr;
    /*TMatrix scale = TMatrix::CreateScale(0.03f, 0.03f, 0.03f);
    m_RifleBulletList[index]->m_matControl = scale * LGlobal::g_PlayerModel->m_matControl;*/
    TVector3 vTrans = LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->GetPosition();
    TVector3 forward = LGlobal::g_PlayerModel->m_matControl.Forward();
    //TVector3 right = LGlobal::g_PlayerModel->m_matControl.Right();
    m_RifleBulletList[index]->m_matControl._41 = vTrans.x;// -forward.x * 10.f;
    m_RifleBulletList[index]->m_matControl._42 = vTrans.y;
    m_RifleBulletList[index]->m_matControl._43 = vTrans.z;// -forward.z * 10.f;
    m_RifleBulletList[index]->m_Forward = forward;
    m_RifleBulletList[index]->m_Forward.Normalize();
    //m_RifleBulletList[index]->m_Forward += {0.f, 0.01f, 0.f};
    TVector3 right = GPLAYER->m_matControl.Right();
    right.Normalize();
    //m_RifleBulletList[index]->m_Forward -= right * 0.01f;
    for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
    {
        if (m_Select->ChkOBBToRay(&zombie->m_OBBBox.m_Box))
        {
            m_RifleBulletList[index]->m_Forward = m_Select->m_vIntersection - m_RifleBulletList[index]->GetPosition();
            m_RifleBulletList[index]->m_Forward.Normalize();
        }

    }
    m_RifleBulletList[index]->lifeStart = 0.f;
    forward = LGlobal::g_pMainCamera->m_vTargetPos - LGlobal::g_pMainCamera->m_vCameraPos;
    forward.Normalize();
    m_RifleBulletList[index]->m_matControl.Forward(forward * 0.05f);
    if (LGlobal::g_PlayerModel->IsZedTime && m_RifleBulletList[index]->bTarget == false)
        AutoTargetBullet(forward, index, LGlobal::g_PlayerModel->GetPosition());
}
void InGameScene::ShootShotgun()
{
    int index = LGlobal::g_PlayerModel->m_Gun->m_GunSpec.CurrentAmmo;
    
    TMatrix scale = TMatrix::CreateScale(0.03f, 0.03f, 0.03f) * LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl;
    
    for (int i = 0; i < m_ShotgunBulletListArray[index].size(); ++i)
    {
        m_ShotgunBulletListArray[index][i]->bVisible = true;
        float randomUp = m_Distribution(m_Generator);
        float randomRight = m_Distribution(m_Generator);
        TVector3 direction = LGlobal::g_PlayerModel->m_matControl.Forward();
        TVector3 addUp = LGlobal::g_PlayerModel->m_matControl.Up() * randomUp;
        TVector3 addRight = LGlobal::g_PlayerModel->m_matControl.Right() * randomRight;
        direction += addUp + addRight;

        m_ShotgunBulletListArray[index][i]->m_matControl = scale;
        m_ShotgunBulletListArray[index][i]->m_matControl.Forward(direction);
        m_ShotgunBulletListArray[index][i]->m_matControl._41 += m_ShotgunBulletListArray[index][i]->m_matControl.Forward().x * LGlobal::g_PlayerModel->m_Gun->m_GunSpec.offset;
        m_ShotgunBulletListArray[index][i]->m_matControl._43 += m_ShotgunBulletListArray[index][i]->m_matControl.Forward().z * LGlobal::g_PlayerModel->m_Gun->m_GunSpec.offset;
        m_RifleBulletList[index]->lifeStart = 0.f;
    }
}
void InGameScene::UpdateZombieAndTankModels()
{
    m_ZombieWave->CollisionCheckWithObstacle(m_TreeList);
    m_ZombieWave->CollisionCheckInNpc();

    m_ZombieWave->Frame();
}

void InGameScene::HandlePlayerCollisions()
{
    for (auto& tree : m_TreeList)
    {
        float offsetX = LGlobal::g_PlayerModel->m_matControl._41 - tree->m_matControl._41;
        //float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - tree->m_matControl._42;
        float offsetZ = LGlobal::g_PlayerModel->m_matControl._43 - tree->m_matControl._43;

        TVector2 dir = { offsetX, offsetZ };
        float distance = dir.Length();
        float r = LGlobal::g_PlayerModel->m_fRadius + tree->m_fRadius;
        if (distance <= r)
        {
            dir.Normalize();
            dir *= (r - distance);
            LGlobal::g_PlayerModel->m_matControl._41 += dir.x;
            LGlobal::g_PlayerModel->m_matControl._43 += dir.y;
        }
    }

    for (auto& zombie : m_ZombieWave->m_EnemyMap["Zombie"])
    {
        if (LGlobal::g_PlayerModel->IsRush == false)
        {
            zombie->IsTakeRushDamage = false;
            zombie->IsFirstRushDamage = true;
        }
        if (GPLAYER->m_OBBBox.CollisionCheckOBB(&zombie->m_OBBBoxRightHand)
            && zombie->m_CurrentState == State::ENEMYATTACK
            && zombie->IsHitPlayer)
            {
                LGlobal::g_PlayerModel->IsTakeDamage = true;
        }
            float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - zombie->m_matControl._41;
            float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - zombie->m_matControl._42;
            float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - zombie->m_matControl._43;

            TVector3 dir = { offsetX, offsetY, offsetZ };
            float distance = dir.Length();
            float r = LGlobal::g_PlayerModel->m_fRadius + zombie->m_fRadius;
            if (distance <= r)
            {
                if (GPLAYER->IsRush)
                {
                    if (zombie->IsFirstRushDamage)
                    {
                        zombie->IsTakeDamage = true;
                        zombie->IsTakeRushDamage = true;
                        zombie->IsFirstRushDamage = false;
                    }
                    TVector3 vNormal = { -offsetX, 0.f, -offsetZ };
                    vNormal.Normalize();
                    vNormal.y = 0.5f;
                    zombie->m_Velocity = vNormal * 400;
                    zombie->IsOnAir = true;
                }
                dir.Normalize();
                dir *= (r - distance);
                LGlobal::g_PlayerModel->m_matControl._41 += dir.x;
                LGlobal::g_PlayerModel->m_matControl._43 += dir.z;
            }
    }

    for (auto& tank : m_ZombieWave->m_EnemyMap["Tank"])
    {
        if (LGlobal::g_PlayerModel->IsRush == false)
        {
            tank->IsTakeRushDamage = false;
            tank->IsFirstRushDamage = true;
        }
        float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - tank->m_matControl._41;
        float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - tank->m_matControl._42;
        float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - tank->m_matControl._43;

        TVector3 dir = { offsetX, offsetY, offsetZ };
        float distance = dir.Length();
        //TVector3 range = tank->m_OBBBox.m_Box.vMax - tank->m_OBBBox.m_Box.vMin;
        //float r = range.Length() * 0.7f;
        float r = LGlobal::g_PlayerModel->m_fRadius + tank->m_fRadius;
        if (distance <= r)
        {
            if (tank->IsRush)
            {
                TVector3 vNormal = { offsetX, 0.f, offsetZ };
                vNormal.Normalize();
                vNormal.y = 0.5f;
                GPLAYER->m_Velocity = vNormal * 400;
                GPLAYER->IsOnAir = true;
                GPLAYER->IsTakeDamage = true;
                GPLAYER->m_RushStart += 10.f;
            }
            if (GPLAYER->IsRush)
            {
                if (tank->IsFirstRushDamage)
                {
                    tank->IsTakeDamage = true;
                    tank->IsTakeRushDamage = true;
                    tank->IsFirstRushDamage = false;
                }
                TVector3 vNormal = { -offsetX, 0.f, -offsetZ };
                vNormal.Normalize();
                vNormal.y = 0.5f;
                tank->m_Velocity = vNormal * 400;
                tank->IsOnAir = true;
                tank->m_RushStart += 10.f;
            }
            dir.Normalize();
            dir *= (r - distance);
            LGlobal::g_PlayerModel->m_matControl._41 += dir.x;
            LGlobal::g_PlayerModel->m_matControl._43 += dir.z;
        }
    }

    for (auto& zombie : m_ZombieWave->m_EnemyMap["Boss"])
    {
        if (LGlobal::g_PlayerModel->IsRush == false)
        {
            zombie->IsTakeRushDamage = false;
            zombie->IsFirstRushDamage = true;
        }
        if (GPLAYER->m_OBBBox.CollisionCheckOBB(&zombie->m_OBBBoxRightHand)
            && zombie->IsHitPlayer)
        {
            LGlobal::g_PlayerModel->IsTakeDamage = true;
        }

        if (GPLAYER->m_OBBBox.CollisionCheckOBB(&zombie->m_OBBBoxLeftHand)
            && zombie->IsHitPlayer)
        {
           LGlobal::g_PlayerModel->IsTakeDamage = true;
        }
        float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - zombie->m_matControl._41;
        float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - zombie->m_matControl._42;
        float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - zombie->m_matControl._43;

        TVector3 dir = { offsetX, offsetY, offsetZ };
        float distance = dir.Length();
        float r = LGlobal::g_PlayerModel->m_fRadius + zombie->m_fRadius;
        if (distance <= r)
        {
            if (GPLAYER->IsRush)
            {
                if (zombie->IsFirstRushDamage)
                {
                    zombie->IsTakeDamage = true;
                    zombie->IsTakeRushDamage = true;
                    zombie->IsFirstRushDamage = false;
                }
                /*TVector3 vNormal = { -offsetX, 0.f, -offsetZ };
                vNormal.Normalize();
                vNormal.y = 0.5f;
                zombie->m_Velocity = vNormal * 400;
                zombie->IsOnAir = true;*/
            }
            dir.Normalize();
            dir *= (r - distance);
            LGlobal::g_PlayerModel->m_matControl._41 += dir.x;
            LGlobal::g_PlayerModel->m_matControl._43 += dir.z;
        }
    }

    std::vector<std::shared_ptr<LModel>>::iterator it = m_KitList.begin();
    while (it != m_KitList.end())
    {
        if ((*it)->bVisible == false)
        {
			it++;
			continue;
		}
        float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - (*it)->m_matControl._41;
        float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - (*it)->m_matControl._42;
        float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - (*it)->m_matControl._43;

        TVector3 dir = { offsetX, offsetY, offsetZ };
        float distance = dir.Length();
        float r = LGlobal::g_PlayerModel->m_fRadius + (*it)->m_fRadius;

        if (distance <= r)
        {
            LGlobal::g_PlayerModel->m_HP += 20;
            if (LGlobal::g_PlayerModel->m_HP > 100)
				LGlobal::g_PlayerModel->m_HP = 100;
            it = m_KitList.erase(it);
            UIManager::GetInstance().GetUIObject(L"HPbar")->GetScript<HpBar>(L"HpBar")->UpdateHp();
            UIManager::GetInstance().GetUIObject(L"face")->GetScript<UIEvent>(L"UIEvent")->UpdatePlayerFace();
        }
        else
        {
            ++it;
        }
    }

    //ammo

    std::vector<std::shared_ptr<LModel>>::iterator it2 = m_AmmoList.begin();
    while (it2 != m_AmmoList.end())
    {
        if ((*it2)->bVisible == false)
        {
			it2++;
			continue;
		}
		float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - (*it2)->m_matControl._41;
		float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - (*it2)->m_matControl._42;
		float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - (*it2)->m_matControl._43;

		TVector3 dir = { offsetX, offsetY, offsetZ };
		float distance = dir.Length();
		float r = LGlobal::g_PlayerModel->m_fRadius + (*it2)->m_fRadius;

        if (distance <= r && LGlobal::g_PlayerModel->m_Gun->m_GunSpec.CurrentAmmo != LGlobal::g_PlayerModel->m_Gun->m_GunSpec.TotalAmmo)
        {
            LGlobal::g_PlayerModel->m_Gun->m_GunSpec.CurrentAmmo = LGlobal::g_PlayerModel->m_Gun->m_GunSpec.TotalAmmo;
			it2 = m_AmmoList.erase(it2);
            UIManager::GetInstance().GetUIObject(L"C_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Gun->m_GunSpec.CurrentAmmo);
		}
        else
        {
			++it2;
		}
	}
}

void InGameScene::LimitPlayerMovement()
{
    if (LGlobal::g_PlayerModel->m_matControl._41 > 970.f) 
        LGlobal::g_PlayerModel->m_matControl._41 = 970.f;
    if (LGlobal::g_PlayerModel->m_matControl._41 < -970.f)
        LGlobal::g_PlayerModel->m_matControl._41 = -970.f;
    if (LGlobal::g_PlayerModel->m_matControl._43 > 970.f)
        LGlobal::g_PlayerModel->m_matControl._43 = 970.f;
    if (LGlobal::g_PlayerModel->m_matControl._43 < -970.f)
        LGlobal::g_PlayerModel->m_matControl._43 = -970.f;
}



void InGameScene::AutoTargetBullet(TVector3 forward, int index, TVector3 thisPosition)
{
    float fNear = 10000.f;
    float distance;
    TVector3 dir, zombiePos;
    LNPC* target = nullptr;
    for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
    {
        if (zombie->IsDead)
            continue;
        zombiePos = zombie->GetPosition();
        dir = zombiePos - thisPosition;
        distance = dir.Length();
        // 캐릭터 정면 방향과 좀비 사이의 각도 계산

        dir.Normalize();
        float dotProduct = forward.Dot(dir);
        if (dotProduct < 0)
            continue;
        float angle = acos(dotProduct) * (180 / L_PI); // 라디안을 도로 변환

        //if (angle <= 10)
        //{
            if (fNear > distance)
            {
                fNear = distance;
                target = zombie.get();
            }
    }
    if (target == nullptr)
    {
        m_RifleBulletList[index]->bTarget = false;
        //m_RifleBulletList[index]->bVisible = false;
        return;
    }
    m_RifleBulletList[index]->target = target;
    m_RifleBulletList[index]->bTarget = true;
}

void InGameScene::LimitNpcMovement()
{
    for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
    {
		if (zombie->m_matControl._41 > 970.f) zombie->m_matControl._41 = 970.f;
		if (zombie->m_matControl._41 < -970.f) zombie->m_matControl._41 = -970.f;
		if (zombie->m_matControl._43 > 970.f) zombie->m_matControl._43 = 970.f;
		if (zombie->m_matControl._43 < -970.f) zombie->m_matControl._43 = -970.f;
	}
}

void InGameScene::UpdateGunModelPosition()
{
    if (LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_pModel != nullptr && LGlobal::g_PlayerModel->m_pActionModel != nullptr)
    {
        if (LGlobal::g_PlayerModel->m_pActionModel->m_iEndFrame <= int(LGlobal::g_PlayerModel->m_fCurrentAnimTime)) return;

        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_pModel->m_matSocket = 
            LGlobal::g_PlayerModel->m_pActionModel->
            m_NameMatrixMap[int(LGlobal::g_PlayerModel->m_fCurrentAnimTime)][LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_ParentBoneName];

        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl = 
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_pModel->m_matScale * 
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_pModel->m_matRotation * 
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_pModel->m_matTranslation *
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_pModel->m_matSocket * 
            LGlobal::g_PlayerModel->m_matForAnim;
    }

    
}

void InGameScene::UpdateOBB()
{
    m_Select->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    LGlobal::g_PlayerModel->m_OBBBox.UpdateOBBBoxPosition(
        { LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._41,
            LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._42,
            LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._43 });

    if (LGlobal::g_PlayerModel->IsSlash)
    {
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.CreateOBBBox(
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.fExtent[0],
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.fExtent[1],
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.fExtent[2],
            { LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.m_matWorld._41,
                LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.m_matWorld._42,
                LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.m_matWorld._43 },
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl.Right(),
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl.Up(),
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl.Forward());
    }
}

void InGameScene::FrameUI()
{
    UIManager::GetInstance().Frame();
}

void InGameScene::FramePointLight()
{
    // Frame point light
    m_PointLight[0].Frame(
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl._41 + LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl.Forward().x * 150,
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl._42,
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl._43 + LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_matControl.Forward().z * 150);
    m_PointLight[0].m_vDirection = { 0, -1, -2 };
    m_PointLight[0].m_vDirection.Normalize();
}

void InGameScene::UpdatePlayerPhysics()
{
    if (LGlobal::g_PlayerModel->IsOnAir == false)
        return;

    LGlobal::g_PlayerModel->m_matControl._41 += LGlobal::g_PlayerModel->m_Velocity.x * LGlobal::g_fSPF;
    LGlobal::g_PlayerModel->m_matControl._42 += LGlobal::g_PlayerModel->m_Velocity.y * LGlobal::g_fSPF;
    LGlobal::g_PlayerModel->m_matControl._43 += LGlobal::g_PlayerModel->m_Velocity.z * LGlobal::g_fSPF;

    LGlobal::g_PlayerModel->m_Velocity.y -= GRAVITY * LGlobal::g_fSPF * 30;
    LGlobal::g_fGameTimer = LGlobal::g_fGameTimer;
}

void InGameScene::UpdateNpcPhysics()
{
    for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
    {
		if (zombie->IsOnAir == false)
			continue;

		zombie->m_matControl._41 += zombie->m_Velocity.x * LGlobal::g_fSPF;
		zombie->m_matControl._42 += zombie->m_Velocity.y * LGlobal::g_fSPF;
		zombie->m_matControl._43 += zombie->m_Velocity.z * LGlobal::g_fSPF;

		zombie->m_Velocity.y -= GRAVITY * LGlobal::g_fSPF * 30;
	}
    for (auto& zombie : m_ZombieWave->m_EnemyMap["Boss"])
    {
        if (zombie->IsOnAir == false)
            continue;

        zombie->m_Velocity.y -= GRAVITY * LGlobal::g_fSPF * 90;

        if (zombie->m_matControl._42 <= zombie->m_MapHeight)
        {
            zombie->IsJumpAttackEnd = true;
        }
    }
}

void InGameScene::InitializeOBBBox()
{
    LGlobal::g_PlayerModel->m_OBBBox.Frame();
    LGlobal::g_PlayerModel->m_OBBBox.CreateOBBBox(
        LGlobal::g_PlayerModel->m_SettingBox.fExtent[0],
        LGlobal::g_PlayerModel->m_SettingBox.fExtent[1],
        LGlobal::g_PlayerModel->m_SettingBox.fExtent[2],
        { LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._41,
            LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._42,
            LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._43 },
        LGlobal::g_PlayerModel->m_SettingBox.vAxis[0],
        LGlobal::g_PlayerModel->m_SettingBox.vAxis[1],
        LGlobal::g_PlayerModel->m_SettingBox.vAxis[2]);

    LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.CreateOBBBox(
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.fExtent[0],
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.fExtent[1],
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.fExtent[2],
        { LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.m_matWorld._41,
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.m_matWorld._42,
            LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_OBBBox.m_matWorld._43 },
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.vAxis[0],
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.vAxis[1],
        LGlobal::g_PlayerModel->m_Gun->m_WeaponModel->m_SettingBox.vAxis[2]);
    m_BackViewCamera->SetTarget(LGlobal::g_PlayerModel);
}

void InGameScene::ZedAutoAim()
{
    //float fNear = 1000.f;
    //float distance;
    //TVector3 dir, target, playerPosition;
    //playerPosition = LGlobal::g_PlayerModel->GetPosition();
    //for (auto& zombie : m_ZombieWave->m_EnemyMap["LNPC"])
    //{
    //    if (zombie->m_HP < 0.001f)
    //        continue;
    //    dir = playerPosition - zombie->GetPosition();
    //    distance = dir.Length();
    //    if (fNear > distance)
    //    {
    //        fNear = distance;
    //        target = dir;
    //    }
    //}
    //
    //if (LInput::GetInstance().m_MouseState[0] > KEY_PUSH)
    //{
    //    //m_Dir = target - TVector3(m_matControl._41, m_matControl._42, m_matControl._43);
    //    dir.Normalize();
    //    TVector3 forward = LGlobal::g_PlayerModel->m_matControl.Forward();
    //    float dirX = dir.x;
    //    float dirZ = dir.z;
    //    DirectX::XMVECTOR gRotation;
    //    DirectX::XMMATRIX matRotation;
    //    float yawRadians = atan2(dirZ, dirX);
    //    gRotation = DirectX::XMQuaternionRotationRollPitchYaw(0, -yawRadians - 1.5708, 0);
    //    DirectX::XMVECTOR xmPos = DirectX::XMVectorSet(LGlobal::g_PlayerModel->m_matControl._41, LGlobal::g_PlayerModel->m_matControl._42, LGlobal::g_PlayerModel->m_matControl._43, 1.0f);
    //    matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmPos);
    //    TMatrix scale;
    //    D3DXMatrixScaling(&scale, 0.2f, 0.2f, 0.2f);
    //    TMatrix Pos = scale * matRotation;
    //    LGlobal::g_PlayerModel->m_matControl = Pos;
    //}


}
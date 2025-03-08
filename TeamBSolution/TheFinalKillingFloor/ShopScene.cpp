#include "ShopScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "UIManager.h"
#include "LPlayer.h"
#include "LWeaponMgr.h"
bool ShopScene::Init()
{   
  
    if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
    {
        UIManager::GetInstance().GetUIObject(L"T_Gun1_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.TotalAmmo));
        UIManager::GetInstance().GetUIObject(L"T_Gun2_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.TotalAmmo));

        if (LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo < 10)
            UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo).insert(0, L"0"));
        else
            UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo));

        UIManager::GetInstance().GetUIObject(L"T_Gun1_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.ShootDelay)));
        UIManager::GetInstance().GetUIObject(L"T_Gun2_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.ShootDelay)));
        UIManager::GetInstance().GetUIObject(L"T_Gun3_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.ShootDelay)));

        UIManager::GetInstance().GetUIObject(L"T_Gun1_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.Damage));
        UIManager::GetInstance().GetUIObject(L"T_Gun2_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.Damage));
        UIManager::GetInstance().GetUIObject(L"T_Gun3_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.Damage));
        UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);

        bool hasRifle = LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.HasWeapon;
        UIManager::GetInstance().GetUIObject(L"Shop_Gun2_Purchase")->SetIsRender(!hasRifle);
        UIManager::GetInstance().GetUIObject(L"Shop_Gun2_empty")->SetIsRender(!hasRifle);
        UIManager::GetInstance().GetUIObject(L"T_Purchase_Gun2")->SetIsRender(!hasRifle);



        bool hasShotGun = LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.HasWeapon;
        UIManager::GetInstance().GetUIObject(L"Shop_Gun3_Purchase")->SetIsRender(!hasShotGun);
        UIManager::GetInstance().GetUIObject(L"Shop_Gun3_empty")->SetIsRender(!hasShotGun);
        UIManager::GetInstance().GetUIObject(L"T_Purchase_Gun3")->SetIsRender(!hasShotGun);
    }
    else if (LGlobal::g_PlayerModel->m_Type == PlayerType::SWORD)
    {

        UIManager::GetInstance().GetUIObject(L"T_Sword1_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / LWeaponMgr::GetInstance().m_map[WeaponState::ONEHANDSWORD]->m_SwordSpec.SlashSpeed)));
        UIManager::GetInstance().GetUIObject(L"T_Sword2_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.SlashSpeed)));


        UIManager::GetInstance().GetUIObject(L"T_Sword1_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)LWeaponMgr::GetInstance().m_map[WeaponState::ONEHANDSWORD]->m_SwordSpec.Damage));
        UIManager::GetInstance().GetUIObject(L"T_Sword2_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.Damage));
        UIManager::GetInstance().GetUIObject(L"Shop_Money2")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);



        bool hasTwoHandSword = LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.HasWeapon;
        UIManager::GetInstance().GetUIObject(L"Shop_Sword2_Purchase")->SetIsRender(!hasTwoHandSword);
        UIManager::GetInstance().GetUIObject(L"Sword2_Icon_Empty")->SetIsRender(!hasTwoHandSword);
        UIManager::GetInstance().GetUIObject(L"T_Purchase_Sword2")->SetIsRender(!hasTwoHandSword);
    }
    return true;
}

void ShopScene::Render()
{
    UIManager::GetInstance().Render();
}

void ShopScene::Process()
{
    UIManager::GetInstance().Frame();

    
    if (UIManager::GetInstance().GetUIObject(L"B_Gun1_MAGAZINE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeMagazine(WeaponState::PISTOL, 100, 15))
        {
            UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.TotalAmmo);
            UIManager::GetInstance().GetUIObject(L"T_Gun1_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.TotalAmmo));

            if (LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.MagazineLevel == 15)
            {
                UIManager::GetInstance().GetUIObject(L"Gun1_MAGAZINE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    if (UIManager::GetInstance().GetUIObject(L"B_Gun2_MAGAZINE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeMagazine(WeaponState::ASSAULTRIFLE, 100, 15))
        {
            UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.TotalAmmo);
            UIManager::GetInstance().GetUIObject(L"T_Gun2_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.TotalAmmo));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.MagazineLevel == 15)
            {
                UIManager::GetInstance().GetUIObject(L"Gun2_MAGAZINE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    if (UIManager::GetInstance().GetUIObject(L"B_Gun3_MAGAZINE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeMagazine(WeaponState::SHOTGUN, 100, 15))
        {
            UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo);

            if (LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo < 10)
                UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo).insert(0, L"0"));
            else
                UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo));

            if (LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.MagazineLevel == 15)
            {
                UIManager::GetInstance().GetUIObject(L"Gun3_MAGAZINE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }

    if (UIManager::GetInstance().GetUIObject(L"B_Gun1_RPM")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeRPM(WeaponState::PISTOL, 100, 8))
        {
            UIManager::GetInstance().GetUIObject(L"T_Gun1_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.ShootDelay)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.RPMLevel == 8)
            {
                UIManager::GetInstance().GetUIObject(L"Gun1_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    if (UIManager::GetInstance().GetUIObject(L"B_Gun2_RPM")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeRPM(WeaponState::ASSAULTRIFLE, 100, 8))
        {
            UIManager::GetInstance().GetUIObject(L"T_Gun2_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.ShootDelay)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.RPMLevel == 8)
            {
                UIManager::GetInstance().GetUIObject(L"Gun2_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    if (UIManager::GetInstance().GetUIObject(L"B_Gun3_RPM")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeRPM(WeaponState::SHOTGUN, 100, 8))
        {
            UIManager::GetInstance().GetUIObject(L"T_Gun3_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.ShootDelay)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.RPMLevel == 8)
            {
                UIManager::GetInstance().GetUIObject(L"Gun3_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }


    if (UIManager::GetInstance().GetUIObject(L"B_Gun1_DAMAGE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeDamage(WeaponState::PISTOL, 100, 10))
        {
            UIManager::GetInstance().GetUIObject(L"T_Gun1_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.Damage)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.DamageLevel == 10)
            {
                UIManager::GetInstance().GetUIObject(L"Gun1_DAMAGE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    if (UIManager::GetInstance().GetUIObject(L"B_Gun2_DAMAGE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeDamage(WeaponState::ASSAULTRIFLE, 100, 10))
        {
            UIManager::GetInstance().GetUIObject(L"T_Gun2_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.Damage)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.DamageLevel == 10)
            {
                UIManager::GetInstance().GetUIObject(L"Gun2_DAMAGE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    if (UIManager::GetInstance().GetUIObject(L"B_Gun3_DAMAGE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeDamage(WeaponState::SHOTGUN, 100, 10))
        {
            UIManager::GetInstance().GetUIObject(L"T_Gun3_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.Damage)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.DamageLevel == 10)
            {
                UIManager::GetInstance().GetUIObject(L"Gun3_DAMAGE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }

    if (UIManager::GetInstance().GetUIObject(L"Shop_Gun2_Purchase")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {
        UIManager::GetInstance().GetUIObject(L"Shop_Gun2_Purchase")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::NONE;
        if (!LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.HasWeapon && LGlobal::g_PlayerModel->m_Money >= 500)
        {
            LGlobal::g_PlayerModel->m_Money -= 500;
            UIManager::GetInstance().GetUIObject(L"Shop_Gun2_Purchase")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"Shop_Gun2_empty")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"T_Purchase_Gun2")->SetIsRender(false);
            LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.HasWeapon = true;
            UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
            UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
        }

    }
    if (UIManager::GetInstance().GetUIObject(L"Shop_Gun3_Purchase")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {
        UIManager::GetInstance().GetUIObject(L"Shop_Gun3_Purchase")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::NONE;
        if (!LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.HasWeapon && LGlobal::g_PlayerModel->m_Money >= 800)
        {
            LGlobal::g_PlayerModel->m_Money -= 800;
            UIManager::GetInstance().GetUIObject(L"Shop_Gun3_Purchase")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"Shop_Gun3_empty")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"T_Purchase_Gun3")->SetIsRender(false);
            LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.HasWeapon = true;
            UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
            UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
        }
    }

    //SWORD

    if (UIManager::GetInstance().GetUIObject(L"B_Sword1_RPM")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeRPM(WeaponState::ONEHANDSWORD, 100, 6))
        {
            UIManager::GetInstance().GetUIObject(L"T_Sword1_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60*LWeaponMgr::GetInstance().m_map[WeaponState::ONEHANDSWORD]->m_SwordSpec.SlashSpeed)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::ONEHANDSWORD]->m_SwordSpec.RPMLevel == 6)
            {
                UIManager::GetInstance().GetUIObject(L"Sword1_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    if (UIManager::GetInstance().GetUIObject(L"B_Sword1_DAMAGE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeDamage(WeaponState::ONEHANDSWORD, 100, 10))
        {
            UIManager::GetInstance().GetUIObject(L"T_Sword1_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(LWeaponMgr::GetInstance().m_map[WeaponState::ONEHANDSWORD]->m_SwordSpec.Damage)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::ONEHANDSWORD]->m_SwordSpec.DamageLevel == 10)
            {
                UIManager::GetInstance().GetUIObject(L"Sword1_DAMAGE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    //SWORD2
    if (UIManager::GetInstance().GetUIObject(L"B_Sword2_RPM")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeRPM(WeaponState::TWOHANDSWORD, 100, 6))
        {
            UIManager::GetInstance().GetUIObject(L"T_Sword2_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 * LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.SlashSpeed)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.RPMLevel == 6)
            {
                UIManager::GetInstance().GetUIObject(L"Sword2_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }
    if (UIManager::GetInstance().GetUIObject(L"B_Sword2_DAMAGE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {

        if (LWeaponMgr::GetInstance().UpgradeDamage(WeaponState::TWOHANDSWORD, 100, 10))
        {
            UIManager::GetInstance().GetUIObject(L"T_Sword2_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.Damage)));
            if (LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.DamageLevel == 10)
            {
                UIManager::GetInstance().GetUIObject(L"Sword2_DAMAGE_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
            }
        }
    }

    if (UIManager::GetInstance().GetUIObject(L"Shop_Sword2_Purchase")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
    {
        UIManager::GetInstance().GetUIObject(L"Shop_Sword2_Purchase")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::NONE;
        if (!LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.HasWeapon && LGlobal::g_PlayerModel->m_Money >= 500)
        {
            LGlobal::g_PlayerModel->m_Money -= 500;
            UIManager::GetInstance().GetUIObject(L"Shop_Sword2_Purchase")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"Sword2_Icon_Empty")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"T_Purchase_Sword2")->SetIsRender(false);
            LWeaponMgr::GetInstance().m_map[WeaponState::TWOHANDSWORD]->m_SwordSpec.HasWeapon = true;
            UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
            UIManager::GetInstance().GetUIObject(L"Shop_Money2")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
        }

    }
}

void ShopScene::Release()
{

}

ShopScene::ShopScene(LScene* parent) : SceneState(parent)
{

}
ShopScene::~ShopScene() {}
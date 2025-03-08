#include "LWeaponMgr.h"
#include "LGlobal.h"
#include "LPlayer.h"
#include "UIManager.h"
void LWeaponMgr::Add(WeaponState key, std::shared_ptr<LWeapon> data)
{
	m_map.insert(std::make_pair(key, data));
}

LWeapon* LWeaponMgr::GetPtr(WeaponState key)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return nullptr;
	}

	return iter->second.get();
}

bool LWeaponMgr::Get(WeaponState key, LWeapon& ret)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return false;
	}

	ret = *(iter->second);

	return true;
}

bool LWeaponMgr::UpgradeDamage(WeaponState weapon, int price, int MaxLevel)
{


	if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
	{
		if (LGlobal::g_PlayerModel->m_Money < price || m_map[weapon]->m_GunSpec.DamageLevel == MaxLevel)
			return false;
		LGlobal::g_PlayerModel->m_Money -= price;

		m_map[weapon]->m_GunSpec.DamageLevel += 1;
		m_map[weapon]->m_GunSpec.Damage = m_map[weapon]->m_GunSpec.defaultDamage + (m_map[weapon]->m_GunSpec.defaultDamage * 0.1 * m_map[weapon]->m_GunSpec.DamageLevel);
	}
	if (LGlobal::g_PlayerModel->m_Type == PlayerType::SWORD)
	{
		if (LGlobal::g_PlayerModel->m_Money < price || m_map[weapon]->m_SwordSpec.DamageLevel == MaxLevel)
			return false;
		LGlobal::g_PlayerModel->m_Money -= price;

		m_map[weapon]->m_SwordSpec.DamageLevel += 1;
		m_map[weapon]->m_SwordSpec.Damage = m_map[weapon]->m_SwordSpec.defaultDamage + (m_map[weapon]->m_SwordSpec.defaultDamage * 0.1 * m_map[weapon]->m_SwordSpec.DamageLevel);
	}


	UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money2")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	return true;
}

bool LWeaponMgr::UpgradeRPM(WeaponState weapon, int price, int MaxLevel)
{
	if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
	{
		if (LGlobal::g_PlayerModel->m_Money < price || m_map[weapon]->m_GunSpec.RPMLevel == MaxLevel)
			return false;
		LGlobal::g_PlayerModel->m_Money -= price;

		m_map[weapon]->m_GunSpec.RPMLevel += 1;
		m_map[weapon]->m_GunSpec.ShootDelay = m_map[weapon]->m_GunSpec.defaultShootDelay - ((0.1 * m_map[weapon]->m_GunSpec.RPMLevel * m_map[weapon]->m_GunSpec.defaultShootDelay));
	}

	if (LGlobal::g_PlayerModel->m_Type == PlayerType::SWORD)
	{
		if (LGlobal::g_PlayerModel->m_Money < price || m_map[weapon]->m_SwordSpec.RPMLevel == MaxLevel)
			return false;
		LGlobal::g_PlayerModel->m_Money -= price;

		m_map[weapon]->m_SwordSpec.RPMLevel += 1;

		m_map[weapon]->m_SwordSpec.SlashSpeed = m_map[weapon]->m_SwordSpec.defaultSlashSpeed + 0.15 * m_map[weapon]->m_SwordSpec.RPMLevel;
	}

	UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money2")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	return true;
}

bool LWeaponMgr::UpgradeMagazine(WeaponState weapon, int price, int MaxLevel)
{
	if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
	{
		if (LGlobal::g_PlayerModel->m_Money < price || m_map[weapon]->m_GunSpec.MagazineLevel == MaxLevel)
			return false;
		LGlobal::g_PlayerModel->m_Money -= price;

		m_map[weapon]->m_GunSpec.MagazineLevel += 1;
		m_map[weapon]->m_GunSpec.TotalAmmo = m_map[weapon]->m_GunSpec.defaultTotalAmmo + (m_map[weapon]->m_GunSpec.defaultTotalAmmo * 0.1 * m_map[weapon]->m_GunSpec.MagazineLevel);
	}

	UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money2")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	return true;
}






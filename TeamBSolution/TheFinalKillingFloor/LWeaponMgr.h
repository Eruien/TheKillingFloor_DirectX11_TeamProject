#pragma once
#include "LModel.h"



struct GunSpec
{
	float defaultShootDelay = 0.0f;
	float ShootDelay = 0.0f;
	float defaultDamage = 0.0f;
	float Damage = 0.0f;
	int defaultTotalAmmo = 0;
	int TotalAmmo = 0;
	int CurrentAmmo = 0;
	int DamageLevel = 0;
	int RPMLevel = 0;
	int MagazineLevel = 0;
	bool HasWeapon = false;
	float offset = 150.0f;
};

struct SwordSpec
{
	float defaultSlashSpeed = 0.0f;
	float SlashSpeed = 0.0f;
	float defaultDamage = 0.0f;
	float Damage = 0.0f;
	float defaultRushDamage = 0.0f;
	float RushDamage = 0.0f;
	int DamageLevel = 0;
	int MagazineLevel = 0;
	int RPMLevel = 0;
	bool HasWeapon = false;
};

class LWeapon
{
public:
	std::shared_ptr<LModel> m_WeaponModel = nullptr;
public:
	GunSpec m_GunSpec;
	SwordSpec m_SwordSpec;
};

class LWeaponMgr
{
	using LList = std::map<WeaponState, std::shared_ptr<LWeapon>>;
public:
	static LWeaponMgr& GetInstance()
	{
		static LWeaponMgr input;
		return input;
	}
	LList m_map;
public:
	void Add(WeaponState key, std::shared_ptr<LWeapon> data);
public:
	LWeapon* GetPtr(WeaponState key);
	bool Get(WeaponState key, LWeapon& ret);
public:
	virtual ~LWeaponMgr() {};
	bool UpgradeDamage(WeaponState weapon, int price, int MaxLevel);
	bool UpgradeRPM(WeaponState weapon, int price, int MaxLevel);
	bool UpgradeMagazine(WeaponState weapon, int price, int MaxLevel);
};


#pragma once
#include "LNPC.h"
#include "LGlobal.h"
#include <random>
#include <chrono>

enum {
	MAXWAVE = 5
};

class ZombieWave
{
public:
	std::default_random_engine m_Generator;
	std::uniform_int_distribution<int> m_Distribution{ -1020, 1020 };
	std::map<int, int> m_WaveZombieCountList;
	std::map<int, int> m_WaveTankCountList;
	std::map<int, int> m_WaveBossCountList;
	
    const int ZombieWave1 = 5;
	const int ZombieWave2 = 7;
	const int ZombieWave3 = 10;
	const int ZombieWave4 = 12;
	const int ZombieWave5 = 3;
	const int TankWave1 = 0;
	const int TankWave2 = 1;
	const int TankWave3 = 2;
	const int TankWave4 = 3;
	const int TankWave5 = 0;
    const int BossWave1 = 0;
    const int BossWave2 = 0;
    const int BossWave3 = 0;
    const int BossWave4 = 0;
    const int BossWave5 = 1;
public:
    std::map<std::string, std::vector<std::shared_ptr<LNPC>>> m_EnemyMap;
	int m_CurrentWave = 0;
    bool IsFirstCreate = true;
public:
	void SetZombie();
	void SpawnZombieWave(LPlayer* player);
	float GetRandomNumber();
	void CollisionCheckOBB(std::vector<shared_ptr<LModel>>& collisionObject, std::vector<LNPC*>& zombieModelList);
    void CollisionCheckInNpc();

	template <typename T, typename U>
	void CollisionCheckByDistance(std::vector<std::shared_ptr<T>>& collisionObject, std::vector<std::shared_ptr<U>>& zombieModelList)
	{
        for (int i = 0; i < zombieModelList.size(); i++)
        {
            // collision check
            if (LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBox)
                || LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBoxRightHand))
            {
                LGlobal::g_PlayerModel->IsTakeDamage = true;
            }

            for (int j = i + 1; j < zombieModelList.size(); j++)
            {
                float offsetX = zombieModelList[i]->m_OBBBox.m_Box.vCenter.x - zombieModelList[j]->m_OBBBox.m_Box.vCenter.x;
                float offsetY = zombieModelList[i]->m_OBBBox.m_Box.vCenter.y - zombieModelList[j]->m_OBBBox.m_Box.vCenter.y;
                float offsetZ = zombieModelList[i]->m_OBBBox.m_Box.vCenter.z - zombieModelList[j]->m_OBBBox.m_Box.vCenter.z;
                TVector3 length = { offsetX, offsetY, offsetZ };
                float distance = length.Length();
                TVector3 range = zombieModelList[i]->m_OBBBox.m_Box.vMax - zombieModelList[i]->m_OBBBox.m_Box.vMin;
                float r = range.Length();
                r *= 0.5f; // Á»ºñ³¢¸®´Â ´õ °¡±î¿öµµ ±¦ÂúÀ½
                if (distance <= r)
                {

                    if (zombieModelList[i]->m_CurrentState != State::ENEMYATTACK)
                    {
                        TVector3 vNormal = { offsetX, zombieModelList[i]->m_matControl._42, offsetZ };
                        vNormal.Normalize();
                        vNormal *= (r - distance);
                        zombieModelList[i]->m_matControl._41 += vNormal.x;
                        zombieModelList[i]->m_matControl._43 += vNormal.z;
                    }
                    else
                    {
                        TVector3 vNormal = { offsetX, zombieModelList[i]->m_matControl._42, offsetZ };
                        vNormal.Normalize();
                        vNormal *= (r - distance);
                        zombieModelList[j]->m_matControl._41 -= vNormal.x;
                        zombieModelList[j]->m_matControl._43 -= vNormal.z;
                    }
                }

            }
            //// Player <-> zombie
            //if (LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBox))
            //{
            //    float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - zombieModelList[i]->m_OBBBox.m_Box.vCenter.x;
            //    float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - zombieModelList[i]->m_OBBBox.m_Box.vCenter.z;

            //    LGlobal::g_PlayerModel->m_matControl._41 += offsetX * 0.1;
            //    LGlobal::g_PlayerModel->m_matControl._43 += offsetZ * 0.1;
            //}

  
        }

        for (int i = 0; i < zombieModelList.size(); i++)
        {
            zombieModelList[i]->m_OBBBox.Frame();
            zombieModelList[i]->m_OBBBox.CreateOBBBox(zombieModelList[i]->m_SettingBox.fExtent[0], zombieModelList[i]->m_SettingBox.fExtent[1], zombieModelList[i]->m_SettingBox.fExtent[2],
                { zombieModelList[i]->m_OBBBox.m_matWorld._41, zombieModelList[i]->m_OBBBox.m_matWorld._42, zombieModelList[i]->m_OBBBox.m_matWorld._43 },
                zombieModelList[i]->m_SettingBox.vAxis[0], zombieModelList[i]->m_SettingBox.vAxis[1], zombieModelList[i]->m_SettingBox.vAxis[2]);

            zombieModelList[i]->m_OBBBoxRightHand.Frame();
            zombieModelList[i]->m_OBBBoxRightHand.CreateOBBBox(zombieModelList[i]->m_SettingBoxRightHand.fExtent[0], zombieModelList[i]->m_SettingBoxRightHand.fExtent[1], zombieModelList[i]->m_SettingBoxRightHand.fExtent[2],
                { zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._41, zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._42, zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._43 },
                zombieModelList[i]->m_SettingBoxRightHand.vAxis[0], zombieModelList[i]->m_SettingBoxRightHand.vAxis[1], zombieModelList[i]->m_SettingBoxRightHand.vAxis[2]);
        }
	}
    void CollisionCheckWithObstacle(std::vector<std::shared_ptr<LModel>>& collisionObject);
	void CollisionBoxRender();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	ZombieWave();
};


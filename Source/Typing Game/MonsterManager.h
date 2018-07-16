#pragma once

#ifndef _CMONSTERMANAGER_H_
#define _CMONSTERMANAGER_H_

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "Sprite.h"
#include "Monster.h"

class CGame;

// Class Declaration:
class CMonsterManager
{
	// class functions
	public:
		CMonsterManager();
		~CMonsterManager();

		void Initialise(int _iScreenWidth, int _iScreenHeight);
		void Process(float _fDelta, float _fTime);
		void Render();
		void SpawnMonster(bool _bPlayer, float _fTime, MONSTER _eType);
		bool ReadyToSpawn(bool _bPlayer, float _fTime, MONSTER _eType);	

		float FindDistance(D3DXVECTOR3 _vec3Point1, D3DXVECTOR3 _vec3Point2);
		void TriggerCoolDown(float _fTime, MONSTER _eType);
		CMonster* GetLatestMonster() const;
		int GetMonsterKills() const;

	protected:

	private:
		void ProcessFightSystem();
		void ProcessAI(float _fTime);
		void ProcessQueuing(std::vector<CMonster*> _vecMonsters);
		void SetMonsterStates(std::vector<CMonster*> _vecMonsters, MONSTERSTATE _eState);
		void ProcessArchers(float _fTime, float _fDelta, std::vector<CMonster*> _vecMonsters, CMonster* _pArcher, CSprite* _pArrow, float& _fTimer, float& _fDeathTimer);
		void DeathAnimationTimer(float& _rfTimer, std::vector<CMonster*>& _rvecMonsters);

	// class members
	public:

	protected:

	private:
		// Timers
		float m_fPlayerTimer[MONSTER_TYPES];
		float m_fAITimer[MONSTER_TYPES];
		float m_fFightTimer;
		float m_fDeathTimerPlayer;
		float m_fDeathTimerAI;
		float m_fAmbientTimerAI;
		float m_fAmbientTimer;

		// Monsters
		std::vector<CMonster*> m_vecpPlayerMonsters;
		std::vector<CMonster*> m_vecpAIMonsters;

		// Archers / arrows
		CMonster* m_pMonsterArcher;
		CMonster* m_pKnightArcher;
		CSprite* m_pArrowPlayer;
		CSprite* m_pArrowAI;
		float m_fArrowTimerPlayer;
		float m_fArrowTimerAI;

		float m_fSpawnPosX;
		float m_fSpawnPosY;

		int m_iWidth;
		int	m_iHeight;

		int m_iMonsterKills;
		bool m_bFighting, m_bCastleAttackPlayer, m_bCastleAttackAI;

		// AI
		float m_fAISpawnTimer;
		MONSTER m_eSpawnType;
		float m_fTimer;

		// Effects
		CSprite* m_pExplosion[2];
		bool m_bExplosion[2];
		float m_fExplosionTimer[2];
};

#endif // _CMONSTERMANAGER_H_
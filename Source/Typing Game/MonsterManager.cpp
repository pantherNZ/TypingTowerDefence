// Self Include:
#include "MonsterManager.h"

// Local Includes:
#include "System.h"
#include "Game.h"

// Library Includes:

// Implementation:
CMonsterManager::CMonsterManager() : m_bFighting(false), m_fFightTimer(0.0f), m_fDeathTimerPlayer(0.0f), m_fDeathTimerAI(0.0f), m_bCastleAttackPlayer(false), m_bCastleAttackAI(false), 
									m_fAISpawnTimer(0.0f), m_pMonsterArcher(nullptr), m_pKnightArcher(nullptr), m_pArrowPlayer(nullptr), m_pArrowAI(nullptr), m_fArrowTimerPlayer(0.0f), 				
									m_fArrowTimerAI(0.0f), m_fTimer(0.0f), m_iMonsterKills(0), m_fAmbientTimerAI(clock()), m_fAmbientTimer(clock())
{
	for(int i = 0; i < MONSTER_TYPES; ++i)
	{
		m_fPlayerTimer[i] = 0.0f;
		m_fAITimer[i] = 0.0f;
	}

	for(int i = 0; i < 2; ++i)
	{
		m_fExplosionTimer[i] = 0.0f;
		m_bExplosion[i] = false;
	}
}

CMonsterManager::~CMonsterManager()
{
	// Delete game monsters
	for(unsigned int i = 0; i < m_vecpPlayerMonsters.size(); ++i)
	{
		DeleteOBJ(m_vecpPlayerMonsters[i]);
	}

	for(unsigned int i = 0; i < m_vecpAIMonsters.size(); ++i)
	{
		DeleteOBJ(m_vecpAIMonsters[i]);
	}

	if (m_pMonsterArcher)
	{
		DeleteOBJ(m_pMonsterArcher);
	}

	if (m_pKnightArcher)
	{
		DeleteOBJ(m_pKnightArcher);
	}

	if (m_pArrowPlayer)
	{
		DeleteOBJ(m_pArrowPlayer);
	}

	if (m_pArrowAI)
	{
		DeleteOBJ(m_pArrowAI);
	}

	for(int i = 0; i < 2; ++i)
	{
		DeleteOBJ(m_pExplosion[i]);
	}
}

int CMonsterManager::GetMonsterKills() const
{
	return(m_iMonsterKills);
}

void CMonsterManager::Initialise(int _iScreenWidth, int _iScreenHeight)
{
	m_iWidth = _iScreenWidth;
	m_iHeight = _iScreenHeight;	
	m_fSpawnPosX = -10.0f;
	m_fSpawnPosY = -6.0f;

	m_fAISpawnTimer = 2000.0f;
	m_eSpawnType = MONSTERL1; //MONSTERARCHER;//

	m_fTimer = (float)clock();

	// Explosion sprites
	for(int i = 0; i < 2; ++i)
	{
		m_pExplosion[i] = new CSprite;
		m_pExplosion[i]->Initialise(L"Data\\explosion.png", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 100, D3DXVECTOR2(158.0f, 168.0f));
	}
}

void CMonsterManager::Process(float _fDelta, float _fTime)
{
	// Process player monster
	for(unsigned int i = 0; i < m_vecpPlayerMonsters.size(); ++i)
	{
		m_vecpPlayerMonsters[i]->Process(_fTime, _fDelta);
	}

	// Process AI monsters
	for(unsigned int i = 0; i < m_vecpAIMonsters.size(); ++i)
	{
		m_vecpAIMonsters[i]->Process(_fTime, _fDelta);
	}

	// Process AI
	ProcessAI(_fTime);

	// Process fighting
	if(m_vecpAIMonsters.size() != 0 || m_vecpPlayerMonsters.size() != 0)
	{
		ProcessFightSystem();
	}

	// Process for explosions
	for(int i = 0; i < 2; ++i)
	{
		if(m_bExplosion[i])
		{
			m_pExplosion[i]->Process(_fTime);

			if(m_fExplosionTimer[i] + m_pExplosion[i]->GetAnimationInterval() / 2.0f < (float)clock())
			{
				m_bExplosion[i] = false;
				m_pExplosion[i]->ResetTextureCoords();
			}
		}
	}

	// Process archers for both player & AI
	ProcessArchers(_fTime, _fDelta, m_vecpAIMonsters, m_pKnightArcher, m_pArrowPlayer, m_fArrowTimerPlayer, m_fDeathTimerAI);
	ProcessArchers(_fTime, _fDelta, m_vecpPlayerMonsters, m_pMonsterArcher, m_pArrowAI, m_fArrowTimerAI, m_fDeathTimerPlayer);

	if (m_fAmbientTimer + 10000.0f < clock())
	{
		// Play ambient sound
		if (!m_vecpPlayerMonsters.empty())
		{
			CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlayAmbiance(true, m_vecpPlayerMonsters.front()->GetTier());
		}

		m_fAmbientTimer = clock();
	}

	if (m_fAmbientTimerAI + 10000.0f < clock())
	{
		// Play ambient sound
		if (!m_vecpAIMonsters.empty())
		{
			CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlayAmbiance(false, m_vecpAIMonsters.front()->GetTier());
		}

		m_fAmbientTimerAI = clock();
	}

}

void CMonsterManager::ProcessFightSystem()
{
	// Check Monster -> Castle attack
	if(m_vecpAIMonsters.empty() || m_vecpPlayerMonsters.empty() || m_vecpAIMonsters.front()->GetState() == DEAD || m_vecpPlayerMonsters.front()->GetState() == DEAD)
	{
		if (!m_vecpPlayerMonsters.empty())
		{
			if (m_vecpPlayerMonsters.front()->GetState() == MOVING)
			{
				float fDistance = FindDistance(m_vecpPlayerMonsters.front()->GetPosition(), CGame::GetCastle(AI_ID)->GetPosition());

				// Check if player is attacking AI's castle
				if ((m_vecpAIMonsters.size() == 0 || m_vecpAIMonsters.front()->GetState() == DEAD) && m_vecpPlayerMonsters.size() > 0 && fDistance < MONSTER_ATTACK_CASTLE_DISTANCE)
				{
					m_bCastleAttackAI = true;
					m_vecpPlayerMonsters.front()->SetState(ATTACKING);
					m_fFightTimer = (float)clock();	

					CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlayCastleDamage();
				}
			}
		}
		
		if (!m_vecpAIMonsters.empty())
		{
			if (m_vecpAIMonsters.front()->GetState() == MOVING)
			{
				float fDistance = FindDistance(m_vecpAIMonsters.front()->GetPosition(), CGame::GetCastle(PLAYER_ID)->GetPosition());

				// Check if AI is attacking players's castle 
				if ((m_vecpPlayerMonsters.size() == 0 || m_vecpPlayerMonsters.front()->GetState() == DEAD) && m_vecpAIMonsters.size() > 0 && fDistance < MONSTER_ATTACK_CASTLE_DISTANCE) 
				{
					m_bCastleAttackPlayer = true;
					m_vecpAIMonsters.front()->SetState(ATTACKING);
					m_fFightTimer = (float)clock();	

					CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlayCastleDamage();
				}
			}
		}
	}
	else // Check Monster -> Monster attack
	{
		bool bDead = (m_vecpAIMonsters.front()->GetState() == DEAD || m_vecpPlayerMonsters.front()->GetState() == DEAD);
		bool bMoving = (m_vecpAIMonsters.front()->GetState() == MOVING || m_vecpPlayerMonsters.front()->GetState() == MOVING);

		if(!bDead && bMoving)
		{
			if(FindDistance(m_vecpAIMonsters.front()->GetPosition(), m_vecpPlayerMonsters.front()->GetPosition()) < MONSTER_ATTACK_DISTANCE)
			{
				m_vecpAIMonsters.front()->SetState(ATTACKING); 
				m_vecpPlayerMonsters.front()->SetState(ATTACKING);
				m_bFighting = true;
				m_bCastleAttackPlayer = false;
				m_bCastleAttackAI = false;
				CSystemClass::GetInstance()->GetGame()->GetSoundManager()->StopSound("CASTLE_DAMAGE");
				CSystemClass::GetInstance()->GetGame()->GetSoundManager()->StartConflict();
				m_fFightTimer = (float)clock();			
			}
		}
	}

	if (!m_vecpAIMonsters.empty())
	{
		if(m_vecpAIMonsters.front()->GetState() == ATTACKING)
		{
			ProcessQueuing(m_vecpAIMonsters);
		}
	}

	if (!m_vecpPlayerMonsters.empty())
	{
		if(m_vecpPlayerMonsters.front()->GetState() == ATTACKING)
		{
			ProcessQueuing(m_vecpPlayerMonsters);
		}
	}

	if (m_bFighting)
	{
		// Process the fight happening
		if ((m_fFightTimer + m_vecpAIMonsters.front()->GetAnimationInterval()) < clock())
		{
			int iPlayerDamage = m_vecpPlayerMonsters.front()->GetDamage();
			int iAIDamage = m_vecpAIMonsters.front()->GetDamage();

			m_vecpPlayerMonsters.front()->TakeDamage(iAIDamage);
			m_vecpAIMonsters.front()->TakeDamage(iPlayerDamage);

			if(m_vecpAIMonsters.front()->IsDead() || m_vecpPlayerMonsters.front()->IsDead())
			{			
				SetMonsterStates(m_vecpAIMonsters, MOVING);
				SetMonsterStates(m_vecpPlayerMonsters, MOVING);

				if (m_vecpAIMonsters.front()->IsDead())
				{
					m_fDeathTimerAI = (float)clock();
					m_vecpAIMonsters.front()->SetState(DEAD);
					m_vecpAIMonsters.front()->SetAnimationInterval(400.0f);

					// Play sound
					CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlayDeathNoise(false, m_vecpAIMonsters.front()->GetTier());

					m_bExplosion[0] = true;
					m_fExplosionTimer[0] = (float)clock();

					D3DXVECTOR3 vec3TargPos = m_vecpAIMonsters.front()->GetPosition();

					if(m_vecpAIMonsters.front()->GetTier() == 2)
					{
						vec3TargPos.x -= 0.35f;
					}
					if(m_vecpAIMonsters.front()->GetTier() == 3)
					{
						vec3TargPos.x -= 0.5f;
					}

					m_pExplosion[0]->SetPosition(D3DXVECTOR3(-vec3TargPos.z, 0.0f, vec3TargPos.x - 0.5f));

					++m_iMonsterKills;
				}

				if (m_vecpPlayerMonsters.front()->IsDead())
				{
					m_fDeathTimerPlayer = (float)clock();
					m_vecpPlayerMonsters.front()->SetState(DEAD);
					m_vecpPlayerMonsters.front()->SetAnimationInterval(400.0f);

					// play sound
					CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlayDeathNoise(true,	m_vecpPlayerMonsters.front()->GetTier());
					m_bExplosion[1] = true;
					m_fExplosionTimer[1] = (float)clock();

					D3DXVECTOR3 vec3TargPos = m_vecpPlayerMonsters.front()->GetPosition();

					if(m_vecpPlayerMonsters.front()->GetTier() == 2)
					{
						vec3TargPos.x -= 0.15f;
					}
					if(m_vecpPlayerMonsters.front()->GetTier() == 3)
					{
						vec3TargPos.x -= 0.2f;
					}

					m_pExplosion[1]->SetPosition(D3DXVECTOR3(-vec3TargPos.z - 0.5f, 0.0f, vec3TargPos.x - 0.5f));
				}

				m_bFighting = false;
				CSystemClass::GetInstance()->GetGame()->GetSoundManager()->EndConflict();
			}

			m_fFightTimer = (float)clock();
		}
	}

	if (m_bCastleAttackAI)
	{
		if ((m_fFightTimer + m_vecpPlayerMonsters.front()->GetAnimationInterval()) < clock())
		{
			int iDamage = m_vecpPlayerMonsters.front()->GetDamage();
			CGame::GetCastle(AI_ID)->DealDamage(iDamage);
			m_fFightTimer = (float)clock();
		}
	}
	else if (m_bCastleAttackPlayer)
	{
		if ((m_fFightTimer + m_vecpAIMonsters.front()->GetAnimationInterval()) < clock())
		{
			int iDamage = m_vecpAIMonsters.front()->GetDamage();
			CGame::GetCastle(PLAYER_ID)->DealDamage(iDamage);
			m_fFightTimer = (float)clock();
		}
	}

	// Death animation timer
	DeathAnimationTimer(m_fDeathTimerAI, m_vecpAIMonsters);
	DeathAnimationTimer(m_fDeathTimerPlayer, m_vecpPlayerMonsters);
}

void CMonsterManager::DeathAnimationTimer(float& _rfTimer, std::vector<CMonster*>& _rvecMonsters)
{
	if(_rfTimer != 0.0f)
	{
		if(!_rvecMonsters.empty())
		{
			float fTime = _rvecMonsters.front()->GetAnimationInterval() / 2.0f;

			if((_rfTimer + fTime) < clock())
			{
				_rfTimer = 0.0f;

				if (_rvecMonsters.front()->GetState() == DEAD)
				{
					DeleteOBJ(_rvecMonsters.front());
					_rvecMonsters.erase(_rvecMonsters.begin());
				}
			}
		}
	}
}

void CMonsterManager::ProcessQueuing(std::vector<CMonster*> _vecMonsters)
{
	for(unsigned int i = 1; i < _vecMonsters.size(); ++i)
	{
		if(_vecMonsters[i]->GetState() == MOVING)
		{
			if(FindDistance(_vecMonsters[i - 1]->GetPosition(), _vecMonsters[i]->GetPosition()) < MONSTER_QUEUE_DISTANCE)
			{
				_vecMonsters[i]->SetState(STOPPED); 
			}
		}
	}
}

void CMonsterManager::SetMonsterStates(std::vector<CMonster*> _vecMonsters, MONSTERSTATE _eState)
{
	for(unsigned int i = 0; i < _vecMonsters.size(); ++i)
	{
		_vecMonsters[i]->SetState(_eState);
	}
}

void CMonsterManager::ProcessAI(float _fTime)
{
	float fCurtime = (float)clock();

	// AI CODE
	if((m_fAISpawnTimer + m_fTimer) < fCurtime)
	{
		if(m_eSpawnType == MONSTERL1)
		{
			SpawnMonster(false, _fTime, MONSTERL1);
		}
		else if(m_eSpawnType == MONSTERL2)
		{
			SpawnMonster(false, _fTime, MONSTERL2);
		}
		else if(m_eSpawnType == MONSTERL3)
		{
			SpawnMonster(false, _fTime, MONSTERL3);
		}
		else if(m_eSpawnType == MONSTERARCHER)
		{
			SpawnMonster(false, _fTime, MONSTERARCHER);
		}

		m_fTimer = fCurtime;
		int iRand =  rand() % 100;
		m_eSpawnType = (iRand <= 45 ? MONSTERL1 : (iRand <= 80 ? MONSTERL2 : (iRand <= 95 ? MONSTERL3 : (m_pMonsterArcher ? MONSTERL1 : MONSTERARCHER))));

		int iTime = (m_eSpawnType == MONSTERARCHER ? 3 : m_eSpawnType - 3);
		m_fAISpawnTimer = (2000.0f + 2000.0f * iTime) + (3000.0f + RandomFloat() * 4500.0f * iTime);	
		//m_fAISpawnTimer = (1000.0f + 1000.0f * iTime) + (1000.0f + RandomFloat() * 1000.0f * iTime);	
		//m_fAISpawnTimer = 99999999999;
	}
}

void CMonsterManager::Render()
{
	// Render explosions
	for(int i = 0; i < 2; ++i)
	{
		if(m_bExplosion[i])
		{
			CGame::RenderSprite(m_pExplosion[i]);
		}
	}

	// Render Player monsters
	for(unsigned int i = 0; i < m_vecpPlayerMonsters.size(); ++i)
	{
		CGame::RenderSprite(m_vecpPlayerMonsters[i]);
	}

	// Render AI monsters
	for(unsigned int i = 0; i < m_vecpAIMonsters.size(); ++i)
	{
		CGame::RenderSprite(m_vecpAIMonsters[i]);
	}
	
	// Render arrows
	if (m_pArrowPlayer) CGame::RenderSprite(m_pArrowPlayer);
	if (m_pArrowAI) CGame::RenderSprite(m_pArrowAI);

	// Remder Archers
	if (m_pMonsterArcher) CGame::RenderSprite(m_pMonsterArcher);
	if (m_pKnightArcher) CGame::RenderSprite(m_pKnightArcher);
}

bool CMonsterManager::ReadyToSpawn(bool _bPlayer, float _fTime, MONSTER _eType)
{
	if(_fTime >= (_bPlayer ? m_fPlayerTimer[_eType] : m_fAITimer[_eType]) + g_kiMonsterCooldown[_eType])
	{
		return(true);
	}
	
	return(false);
}

void CMonsterManager::SpawnMonster(bool _bPlayer, float _fTime, MONSTER _eType)
{
	if (_eType == ARCHER && m_pKnightArcher != nullptr)
	{
		return;
	}

	CMonster* pMonster = new CMonster;
	LPCWSTR sMonsterName = L"";
	D3DXVECTOR3 vec3Scale(1.5f, 1.5f, 1.5f);
	int iDataIndex = (!_bPlayer ? _eType - 3 : _eType);

	switch(_eType)
	{
		case SWORDSMANL1:
		{
			sMonsterName = L"Data\\Knight Blue.png";
			vec3Scale = D3DXVECTOR3(1.3f, 1.3f, 1.3f);
			m_fSpawnPosY = -6.0f;
			pMonster->SetTier(1);
		}
		break;

		case SWORDSMANL2:
		{
			sMonsterName = L"Data\\Knight Green.png";
			vec3Scale = D3DXVECTOR3(1.4f, 1.4f, 1.4f);
			m_fSpawnPosY = -5.8f;
			pMonster->SetTier(2);
		}
		break;
		
		case SWORDSMANL3:
		{
			sMonsterName = L"Data\\Knight Red.png";
			vec3Scale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
			m_fSpawnPosY = -5.8f;
			pMonster->SetTier(3);
		}
		break;

		case MONSTERL1:
		{
			sMonsterName = L"Data\\Beast Brown.png";
			vec3Scale = D3DXVECTOR3(1.3f, 1.3f, 1.3f);
			m_fSpawnPosY = -5.8f;
			pMonster->SetTier(1);
		}
		break;

		case MONSTERL2:
		{
			sMonsterName = L"Data\\Beast Grey.png";
			vec3Scale = D3DXVECTOR3(1.6f, 1.6f, 1.6f);
			m_fSpawnPosY = -5.6f;
			pMonster->SetTier(2);
		}
		break;

		case MONSTERL3:
		{
			sMonsterName = L"Data\\Beast Red.png";
			vec3Scale = D3DXVECTOR3(1.8f, 1.8f, 1.8f);
			m_fSpawnPosY = -5.5f;
			pMonster->SetTier(3);
		}
		break;

		case ARCHER:
		{
			sMonsterName = L"Data\\Knight Archer.png";
			m_fSpawnPosY = -2.2f;
			m_fSpawnPosX = -11.95f;
			iDataIndex = 3;
		}
		break;

		case MONSTERARCHER:
		{
			sMonsterName = L"Data\\Beast Archer.png";
			m_fSpawnPosY = -1.2f;
			m_fSpawnPosX = -11.95f;
			iDataIndex = 3;
		}
		break;

		default: break;
	};

	CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlaySpawnNoise(_bPlayer, pMonster->GetTier());

	pMonster->Initialise(sMonsterName, D3DXVECTOR3((_bPlayer ? m_fSpawnPosX : -m_fSpawnPosX), 0.0f, m_fSpawnPosY), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
							vec3Scale, 200.0f,  D3DXVECTOR2(158.0f, 168.0f), 6, g_kiMonsterHealth[iDataIndex], g_kiMonsterDamage[iDataIndex], _bPlayer);

	if (_eType == ARCHER)
	{
		m_pKnightArcher = pMonster;
		m_fPlayerTimer[iDataIndex] = _fTime;
		m_pKnightArcher->SetState(STOPPED);
		m_pKnightArcher->SetAnimationInterval(300.0f);
		return;
	}

	if (_eType == MONSTERARCHER)
	{
		m_pMonsterArcher = pMonster;
		m_fAITimer[iDataIndex] = _fTime;
		m_pMonsterArcher->SetState(STOPPED);
		m_pMonsterArcher->SetAnimationInterval(300.0f);
		return;
	}

	if(_bPlayer)
	{
		m_fPlayerTimer[iDataIndex] = _fTime;
		m_vecpPlayerMonsters.push_back(pMonster);
	}
	else
	{
		m_fAITimer[iDataIndex] = _fTime;
		m_vecpAIMonsters.push_back(pMonster);
	}
}

void CMonsterManager::TriggerCoolDown(float _fTime, MONSTER _eType)
{
	m_fPlayerTimer[_eType] = _fTime;
}

float CMonsterManager::FindDistance(D3DXVECTOR3 _vec3Point1, D3DXVECTOR3 _vec3Point2)
{
	return (sqrt(pow(_vec3Point2.x - _vec3Point1.x, 2) + pow(_vec3Point2.y - _vec3Point1.y, 2) + pow(_vec3Point2.z - _vec3Point1.z, 2)));
}

void CMonsterManager::ProcessArchers(float _fTime, float _fDelta, std::vector<CMonster*> _vecMonsters, CMonster* _pArcher, CSprite* _pArrow, float& _fArrowTimer, float& _fDeathTimer)
{
	if(_pArcher)
	{		
		bool bPlayer = (_fArrowTimer == m_fArrowTimerPlayer);

		if (!_vecMonsters.empty())
		{
			float fDist = FindDistance(_pArcher->GetPosition(), _vecMonsters.front()->GetPosition());

			if(fDist < ARCHER_SHOOT_DISTANCE)
			{
				float fCurTime = (float)clock();

				// Time to shoot arrow at monster
				if(_pArrow == nullptr)
				{
					if(_pArcher->GetState() == STOPPED)
					{
						_pArcher->SetState(ARCHER_ATTACK);
						_fArrowTimer = fCurTime - _pArcher->GetAnimationInterval() * 0.5f;
					}

					// spawn arrow
					if(_fArrowTimer + _pArcher->GetAnimationInterval() <= fCurTime)
					{
						_fArrowTimer = fCurTime;		
						D3DXVECTOR3 vec3Pos = D3DXVECTOR3(-_pArcher->GetPosition().z + (bPlayer ? 1.0f : -1.0f), 0.0f, _pArcher->GetPosition().x - 1.0f);

						if(_fArrowTimer == m_fArrowTimerPlayer)
						{
							m_pArrowPlayer = new CSprite; 
							m_pArrowPlayer->Initialise(L"Data\\Arrow.png", vec3Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
						}
						else
						{
							m_pArrowAI = new CSprite; 
							m_pArrowAI->Initialise(L"Data\\Arrow.png", vec3Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
						}

						CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlayArrow();
					}
				}
				else
				{
					D3DXVECTOR3 vec3TargPos = _vecMonsters.front()->GetPosition();

					D3DXVECTOR3 v = D3DXVECTOR3(-vec3TargPos.z, 0.0f, vec3TargPos.x);
					D3DXVECTOR3 v2 = D3DXVECTOR3(-_pArcher->GetPosition().z, 0.0f, _pArcher->GetPosition().x);

					float fArrowTargDistance = FindDistance(_pArrow->GetPosition(), vec3TargPos);
					float fArcherTargDistance = FindDistance(v, v2);
					float fDistance = fArrowTargDistance / fArcherTargDistance;

					D3DXVECTOR3 vec3Lead = (v - v2) / 1.0f;
					D3DXVECTOR3 vec3Target = vec3Lead * _fDelta * (1.0f - fDistance) + D3DXVECTOR3((bPlayer ? 1.8f : -1.8f), 0.0f, 0.0f) * _fDelta * fDistance;

					_pArrow->Move(vec3Target * 2.0f);
					D3DXVECTOR3 vec3Angle = D3DXVECTOR3(-atan2(vec3Target.z, vec3Target.x), 0.0f, 0.0f);
					_pArrow->SetRotation(vec3Angle);

					// if arrow has hit
					if(fArrowTargDistance <= 1.0f)
					{
						// Remove the arrow
						if(bPlayer) 
						{
							DeleteOBJ(m_pArrowPlayer);
						}
						else 
						{
							DeleteOBJ(m_pArrowAI);		
						}

						_vecMonsters.front()->TakeDamage(g_kiMonsterDamage[3]);

						if(_vecMonsters.front()->IsDead() && _vecMonsters.front()->GetState() != DEAD)
						{
							m_bFighting = false;
							m_bCastleAttackPlayer = false;
							m_bCastleAttackAI = false;
							_fDeathTimer = fCurTime;
							CSystemClass::GetInstance()->GetGame()->GetSoundManager()->StopSound("CASTLE_DAMAGE");

							SetMonsterStates(m_vecpAIMonsters, MOVING);
							SetMonsterStates(m_vecpPlayerMonsters, MOVING);

							_vecMonsters.front()->SetState(DEAD);

							// Play sound
							CSystemClass::GetInstance()->GetGame()->GetSoundManager()->PlayDeathNoise(!bPlayer,	_vecMonsters.front()->GetTier());

							_vecMonsters.front()->SetAnimationInterval(400.0f);
							_pArcher->SetState(STOPPED);

							if(bPlayer)
							{
								m_bExplosion[0] = true;
								m_fExplosionTimer[0] = fCurTime;

								D3DXVECTOR3 vec3TargPos = m_vecpAIMonsters.front()->GetPosition();

								if(m_vecpAIMonsters.front()->GetTier() == 2)
								{
									vec3TargPos.x -= 0.35f;
								}
								if(m_vecpAIMonsters.front()->GetTier() == 3)
								{
									vec3TargPos.x -= 0.5f;
								}

								m_pExplosion[0]->SetPosition(v - D3DXVECTOR3(0.0f, 0.0f, 0.5f));
								++m_iMonsterKills;
							}
							else
							{
								m_bExplosion[1] = true;
								m_fExplosionTimer[1] = fCurTime;

								if(m_vecpPlayerMonsters.front()->GetTier() == 2)
								{
									vec3TargPos.x -= 0.15f;
								}
								if(m_vecpPlayerMonsters.front()->GetTier() == 3)
								{
									vec3TargPos.x -= 0.2f;
								}

								m_pExplosion[1]->SetPosition(v - D3DXVECTOR3(0.5f, 0.0f, 0.5f));
							}
						}
					}
				}

				// Process (for shooting animation)
				_pArcher->Process(_fTime, _fDelta);

				return;
			}
		}	
		
		if(_pArrow != nullptr)
		{
			D3DXVECTOR3 vec3Dir = D3DXVECTOR3(cos(_pArrow->GetRotation().x), 0.0f, -sin(_pArrow->GetRotation().x)) * _fDelta;
			_pArrow->Move(vec3Dir * 10.0f);

			if(_pArrow->GetPosition().x <= -6.0f)
			{
				// Remove the arrow
				if(bPlayer)
				{				
					DeleteOBJ(m_pArrowPlayer);
				}
				else
				{
					DeleteOBJ(m_pArrowAI);
				}

				_pArcher->SetState(STOPPED);
			}
		}
	}
}

CMonster* CMonsterManager::GetLatestMonster() const
{
	return(m_vecpPlayerMonsters.empty() ? nullptr : m_vecpPlayerMonsters.back());
}
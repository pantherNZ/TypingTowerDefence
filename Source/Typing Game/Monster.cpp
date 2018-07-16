// Self Include:
#include "Monster.h"

// Local Includes:

// Library Includes:

// Static variables

// Implementation:
CMonster::CMonster() : m_eState(MOVING), m_iHealth(10), m_iDamage(0), m_iAnimationCount(1), m_bPlayerMonster(false), m_fMovementSpeed(1.6f), m_iTier(0)
{

}

CMonster::~CMonster()
{

}

void CMonster::Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, float _fAnimationInterval, 
						D3DXVECTOR2 _vec2SpriteSize, int _iAnimations, int _iHealth, int _iDamage, bool _bPlayerMonster)
{
	CSprite::Initialise(_pstrTexture, _vec3Position, _vec3Rotation, _vec3Scale, _fAnimationInterval, _vec2SpriteSize);

	m_iHealth = _iHealth;
	m_iDamage = _iDamage;
	m_iAnimationCount = _iAnimations;
	m_bPlayerMonster = _bPlayerMonster;

	SetState(m_eState);
}

void CMonster::Process(float _fTime, float _fDelta)
{
	CSprite::Process(_fTime);

	switch (m_eState)
	{
		case (MOVING):
		{
			//if(m_bPlayerMonster && m_vec3Position.z > -8.0f || !m_bPlayerMonster && m_vec3Position.z < 8.0f)
			//{
				Move(D3DXVECTOR3(m_fMovementSpeed * _fDelta * (m_bPlayerMonster ? 1 : -1), 0.0f, 0.0f));
			//}
		}
		break;

		case (ATTACKING):
		{
			// Not Used
		}
		break;

		default: break;
	};
}

void CMonster::SetState(MONSTERSTATE _eState)
{
	int iStateNum = (int)_eState;

	if(_eState == STOPPED)
	{
		iStateNum = 0;
		m_bAnimationEnabled = false;
	}
	else 
	{
		m_bAnimationEnabled = true;
	}

	m_eState = _eState;
	m_iAnimationRow = iStateNum + (!m_bPlayerMonster ? 3 : 0);

	float fY = float(m_vec2SpriteSize.y / m_iTextureHeight) * m_iAnimationRow;
	SetTexCoord(D3DXVECTOR4(0.0f, fY, m_vec2TexIncr.x, fY + m_vec2TexIncr.y));

	if(m_eState == ATTACKING)
	{
		for(int i = 0; i < int(RandomFloat() * 3); ++i)
		{
			TranslateTexCoord(D3DXVECTOR4(m_vec2TexIncr.x, 0.0f, m_vec2TexIncr.x, 0.0f));
		}
	}
}

MONSTERSTATE CMonster::GetState() const
{
	return (m_eState);
}

void CMonster::TakeDamage(int _iDamage)
{
	m_iHealth -= _iDamage;
}

int CMonster::GetDamage() const
{
	return (m_iDamage);
}

bool CMonster::IsDead() const
{
	return (m_iHealth < 1);
}

void CMonster::SetTier(int _iTier)
{
	m_iTier = _iTier;
}

int CMonster::GetTier() const
{
	return (m_iTier);
}
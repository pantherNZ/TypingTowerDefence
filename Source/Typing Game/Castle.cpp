// Self Include:
#include "Castle.h"

// Local Includes:

// Library Includes:

// Static variables

// Implementation:
CCastle::CCastle(): m_iMaxHealth(g_kiCastleHealth), m_iHealth(g_kiCastleHealth), m_iCastleLevel(1), m_bPlayerCastle(false)
{

}

CCastle::~CCastle()
{
	
}

bool CCastle::Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, bool _bPlayer, D3DXVECTOR4 _vec4TexCoord)
{
	CSprite::Initialise(_pstrTexture, _vec3Position, _vec3Rotation, _vec3Scale, _vec4TexCoord);

	m_bPlayerCastle = _bPlayer;

	return true;
}

void CCastle::LevelUp()
{
	m_iCastleLevel++;

	m_iMaxHealth = g_kiCastleHealth;

	m_iHealth += g_kiCastleHealth - g_kiCastleHealth;
}

void CCastle::DealDamage(int _iDamageDealt)
{
	m_iHealth -= _iDamageDealt;

	float fHealthPercent = (float)m_iHealth / (float)m_iMaxHealth;

	if(m_bPlayerCastle)
	{
		// under 30%
		if(fHealthPercent <= 0.3f)
		{
			SetTexCoord(D3DXVECTOR4(0.666667f, 0.0f, 1.0f, 1.0f));
		}
		else if(fHealthPercent <= 0.6f) // under 60%
		{
			SetTexCoord(D3DXVECTOR4(0.333334f, 0.0f, 0.666667f, 1.0f));
		}
	}
	else
	{
		// under 40%
		if(fHealthPercent <= 0.4f)
		{
			SetTexCoord(D3DXVECTOR4(0.5f, 0.0f, 1.0f, 1.0f));
		}
	}
}

bool CCastle::IsDead() const
{
	return(m_iHealth <= 0);
}

int CCastle::GetHealth() const
{
	return(m_iHealth);
}
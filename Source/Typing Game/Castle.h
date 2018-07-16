#pragma once

#ifndef _CCASTLE_H_
#define _CCASTLE_H_

// Definitions

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "Sprite.h"

// Class Declaration:
class CCastle : public CSprite
{
	// class functions
	public:	
		CCastle();
		~CCastle();

		bool Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, bool _bPlayer, D3DXVECTOR4 _vec4TexCoord);
		void LevelUp();
		void DealDamage(int _iDamageDealt);
		bool IsDead() const;
		int GetHealth() const;

	protected:

	private:

	// class members
	public:

	protected:

	private:
		int m_iHealth, m_iMaxHealth;
		int m_iCastleLevel;
		bool m_bPlayerCastle;
};

#endif // _CCASTLE_H_
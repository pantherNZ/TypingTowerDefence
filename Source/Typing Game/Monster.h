
#pragma once

#ifndef _CMONSTER_H_
#define _CMONSTER_H_

// Definitions

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "Sprite.h"

// Class Declaration:
class CMonster : public CSprite
{
	// class functions
	public:	
		CMonster();
		~CMonster();
		void Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, float _fAnimationInterval, 
						D3DXVECTOR2 _vec2SpriteSize, int _iAnimations, int _iHealth, int _iDamage, bool _bPlayerMonster);

		void SetState(MONSTERSTATE _eState);
		void Process(float _fTime, float _fDelta);

		MONSTERSTATE GetState() const;

		void TakeDamage(int _iDamage);
		int GetDamage() const;

		bool IsDead() const;

		void SetTier(int _iTier);
		int GetTier() const;	

	protected:

	private:
		

	// class members
	public:

	protected:

	private:
		int m_iHealth;
		int m_iTier;
		int m_iDamage;
		int m_iAnimationCount;
		MONSTERSTATE m_eState;
		bool m_bPlayerMonster;
		float m_fMovementSpeed;
};

#endif // _CMONSTER_H_
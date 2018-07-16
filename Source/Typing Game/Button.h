#pragma once

#ifndef _CBUTTON_H_
#define _CBUTTON_H_

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "Sprite.h"

// Class Declaration:
class CButton : public CSprite
{
	// class functions
	public:
		CButton();
		~CButton();
		bool Process(float _fTime);
		void Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, D3DXVECTOR2 _vec2CollisionBox);
		void Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, D3DXVECTOR2 _vec2CollisionBox, D3DXVECTOR4 _vec4TexCoord);
		void Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, D3DXVECTOR2 _vec2CollisionBox, D3DXVECTOR4 _vec4TexCoord, D3DXVECTOR4 _vec4HighlightTexCoord);

		static void SetMouseState(bool _bMouseDown);
		static void SetMousePosition(int _iX, int _iY);

	protected:

	private:


	// class members
	public:

	protected:

	private:
		static bool m_bMouseDown;
		static int m_iMousePosX;
		static int m_iMousePosY;

		D3DXVECTOR4 m_vec4TexCoordOriginal;
		D3DXVECTOR4 m_vec4HighlightTexCoord;
		D3DXVECTOR2 m_vec2CollisionBox;
		bool m_bHighlight;
		bool m_bHighlightEnabled;
};

#endif // _CBUTTON_H_
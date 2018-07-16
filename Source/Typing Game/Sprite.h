#pragma once

#ifndef _CSPRITE_H_
#define _CSPRITE_H_

// Library Includes:

// Local Includes:
#include "Utility.h"

// Class Declaration:
class CSprite
{
	// class functions
	public:
		CSprite();
		~CSprite();

		static void CreateBuffers(ID3D10Device* _pDevice);
		static void ReleaseBuffers();
		static int GetFaceCount();
		static void ApplyRenderOptions();

		void Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale);
		void Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, D3DXVECTOR4 _vec4TexCoord);
		void Initialise(LPCWSTR _pstrTexture, D3DXVECTOR3 _vec3Position, D3DXVECTOR3 _vec3Rotation, D3DXVECTOR3 _vec3Scale, float _fAnimationInterval, D3DXVECTOR2 _vec2SpriteSize);

		void Process(float _fTime);

		ID3D10ShaderResourceView* GetTexture() const;
	
		void SetPosition(D3DXVECTOR3 _vec3Position);
		D3DXVECTOR3 GetPosition() const;

		void SetRotation(D3DXVECTOR3 _vec3Rotation);
		D3DXVECTOR3 GetRotation() const;

		void SetScale(D3DXVECTOR3 _vec3Scale);
		D3DXVECTOR3 GetScale() const;
		void ModifyScale(D3DXVECTOR3 _vec3Scale);

		void Move(D3DXVECTOR3 _vec3Position);
		void Rotate(D3DXVECTOR3 _vec3Rotation);

		D3DXMATRIX GetTextureMatrix() const;
		D3DXMATRIX GetWorldMatrix() const;

		void SetTexCoord(D3DXVECTOR4 _vec4TexCoord);
		D3DXVECTOR4 GetTexCoord() const;

		void TranslateTexCoord(D3DXVECTOR4 _vec4TexCoord);

		int GetWidth() const;
		int GetHeight() const;

		float GetAnimationInterval() const;
		void SetAnimationInterval(float _fInterval);

		void NormalSize();
		void ResetTextureCoords();

	protected:

	private:
		void BuildWorldMatrix();
		void BuildTextureMatrix();

	// class members
	public:

	protected:
		D3DXVECTOR2 m_vec2TexIncr;
		D3DXVECTOR4 m_vec4TexCoord;
		ID3D10ShaderResourceView* m_pTexture;
		D3DXVECTOR3 m_vec3Position;
		D3DXVECTOR2 m_vec2SpriteSize;
		int m_iAnimationRow;
		
		int m_iTextureWidth;
		int m_iTextureHeight;

		bool m_bAnimationEnabled;

	private:
		static ID3D10Device* m_spDevice;
		static ID3D10Buffer* m_spVertexBuffer;
		static ID3D10Buffer* m_spIndexBuffer; 
		static int m_siFaceCount;
	
		D3DXVECTOR3 m_vec3Scale;
		D3DXVECTOR3 m_vec3Rotation;
	
		D3DXMATRIX m_matWorld;
		D3DXMATRIX m_matTexture;
		
		float m_fAnimationInterval;
		float m_fTimer;		
};

#endif // _CSPRITE_H_
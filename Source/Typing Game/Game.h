#pragma once

#ifndef _CGAME_H_
#define _CGAME_H_

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "DirectX.h"
#include "Sprite.h"
#include "Camera.h"
#include "SpriteShader.h"
#include "Button.h"
#include "MonsterManager.h"
#include "QuestionManager.h"
#include "Castle.h"
#include "SoundManager.h"

// Class Declaration:
class CGame
{
	// class functions
	public:
		CGame();
		~CGame();

		bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight);
		void Process(float _fDelta, float _fTime);
		void Render();

		static void RenderSprite(void* _pSprite);
		static CCastle* GetCastle(int _iCastle);
		static void RenderText(LPCWSTR _str, RECT _r, D3DXCOLOR _c);

		CQuestionManager* GetQuestionManager();
		CSoundManager* GetSoundManager();

		void SetKeyDown(bool _b);

	protected:

	private:
		void DeleteMenuObjects();
		void StartMenuSetup();
		void DeleteGameObjects();
		void GameSetup();

	// class members
	public:

	protected:

	private:
		ID3D10Device* m_pDevice;

		int m_iWidth;
		int m_iHeight;
		HWND m_hWnd;

		// classes used
		static ID3DX10Font* m_spFont;
		static CSpriteShader* m_pSpriteShader;		
		static CCamera* m_pCameraClass;
		static CCastle* m_pCastle[2];
		CDirectX* m_pDirectXClass;
		CQuestionManager* m_pQuestionManager;
		CSoundManager* m_pSoundManager;

		// Sprites
		CSprite* m_pHealthBar;
		CSprite* m_pHealthBarOutline;
		CSprite* m_pGameOverSprite;
		CSprite* m_pHowToPlay;

		std::vector<CSprite*> m_vecpGameObjects;
		std::vector<CButton*> m_vecpGameButtons;
		float m_fTimer;
		GAMESTATE m_eGameState;	

		float m_fDelta;
		float m_fGameTime;
		bool m_bKeyDown;
};

#endif // _CGAME_H_
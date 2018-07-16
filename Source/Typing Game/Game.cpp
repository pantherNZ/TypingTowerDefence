// Self Include:
#include "Game.h"

// Local Includes:
#include "System.h"

// Library Includes:

// Classes

// Statics:
CSpriteShader* CGame::m_pSpriteShader = nullptr;
CCastle* CGame::m_pCastle[2] = {nullptr, nullptr};
CCamera* CGame::m_pCameraClass = nullptr;
ID3DX10Font* CGame::m_spFont = nullptr;

// Implementation:
CGame::CGame() : m_pDevice(nullptr), m_pDirectXClass(nullptr), m_hWnd(NULL), m_iWidth(0), m_iHeight(0), m_fTimer(0.0f), m_eGameState(MENU), m_fDelta(0.0f), m_fGameTime(0.0f), m_bKeyDown(false),
				m_pQuestionManager(nullptr), m_pHealthBar(nullptr), m_pHealthBarOutline(nullptr), m_pSoundManager(nullptr), m_pGameOverSprite(nullptr), m_pHowToPlay(nullptr)
{
	
}

CGame::~CGame()
{
	// Delete classes used
	DeleteOBJ(m_pDirectXClass);
	DeleteOBJ(m_pCameraClass);
	DeleteOBJ(m_pSpriteShader);
	DeleteOBJ(m_pQuestionManager);
	DeleteOBJ(m_pSoundManager);
	DeleteOBJ(m_pGameOverSprite);

	// Delete castle objects
	for (unsigned int i = 0; i < 2; ++i)
	{
		DeleteOBJ(m_pCastle[i]);
	}

	DeleteOBJ(m_pHealthBar);
	DeleteOBJ(m_pHealthBarOutline);

	DeleteMenuObjects();

	// Release sprite buffers
	CSprite::ReleaseBuffers();

	m_spFont->Release();
}

void CGame::DeleteMenuObjects()
{
	// Delete game objects
	for(unsigned int i = 0; i < m_vecpGameObjects.size(); ++i)
	{
		DeleteOBJ(m_vecpGameObjects[i]);
	}

	// Delete game buttons
	for(unsigned int i = 0; i < m_vecpGameButtons.size(); ++i)
	{
		DeleteOBJ(m_vecpGameButtons[i]);
	}

	m_vecpGameObjects.clear();
	m_vecpGameButtons.clear();
}

void CGame::DeleteGameObjects()
{
	DeleteMenuObjects();

	// Delete castle objects
	for (unsigned int i = 0; i < 2; ++i)
	{
		DeleteOBJ(m_pCastle[i]);
	}

	DeleteOBJ(m_pGameOverSprite);
	DeleteOBJ(m_pHealthBar);
	DeleteOBJ(m_pHealthBarOutline);	
	DeleteOBJ(m_pHowToPlay);

	m_pQuestionManager->DeleteObjects();
}

bool CGame::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight)
{
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;
	m_hWnd = _hWnd;

	// Create dx10 class
	m_pDirectXClass = new CDirectX;

	// Initalise Dx10 class
	if(!m_pDirectXClass->Initialise(_hWnd, _iWidth, _iHeight))
	{
		MessageBox(NULL, L"Initialising DirectX10 Failed", L"Error", MB_OK);	
		return(false);
	}

	// store created device
	m_pDevice = m_pDirectXClass->GetDevice(); 

	if(m_pCameraClass == nullptr)
	{
		// Create camera class
		m_pCameraClass = new CCamera;

		// Initialise camera
		m_pCameraClass->Initialise(D3DXVECTOR3(0.0f, 25.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), m_iWidth, m_iHeight);
	}

	// Initialise sprite buffers
	CSprite::CreateBuffers(m_pDevice);
	CSprite::ApplyRenderOptions();

	if(m_pSpriteShader == nullptr)
	{
		// Create sprite shader class
		m_pSpriteShader = new CSpriteShader;

		// Initialise sprite shader class
		if(!m_pSpriteShader->Initialise(m_pDevice, _hWnd))
		{
			MessageBox(NULL, L"Initialising Sprite Shader Failed", L"Error", MB_OK);	
			return(false);
		}
	}

	// Create question manager class
	m_pQuestionManager = new CQuestionManager;

	m_pSoundManager = new CSoundManager;
	m_pSoundManager->Initialise();

	// Create menu objects
	StartMenuSetup();
	
	// Create font
	if(FAILED(D3DX10CreateFont(m_pDevice, 15, 10, 50, 0, false, 0, 0, 0, 0, L"Times New Roman", &m_spFont)))
	{
		MessageBox(NULL, L"Initialising DX10Font Failed", L"Error", MB_OK);	
		return(false);
	}

	return(true);
}

void CGame::Process(float _fDelta, float _fTime)
{
	m_fDelta = _fDelta;

	// Process game sprites
	if(m_eGameState == GAMEOVER)
	{
		// wait 7.5 seconds to go to main menu)
		if(m_fTimer + 7500.0f <= (float)clock())
		{
			DeleteGameObjects();
			StartMenuSetup();

			m_pSoundManager->FinishGame();
		}
	}
	else
	{
		for(unsigned int i = 0; i < m_vecpGameObjects.size(); ++i)
		{
			m_vecpGameObjects[i]->Process(_fTime);		
		}
	
		// Process game buttons
		for(unsigned int i = 0; i < m_vecpGameButtons.size(); ++i)
		{
			if(m_vecpGameButtons[i]->Process(_fTime))
			{
				DeleteMenuObjects();
				GameSetup();
			}
		}
		
		if(m_eGameState == GAME)
		{
			// Process question manager
			m_pQuestionManager->Process(_fDelta, _fTime);

			// Check if the game is over
			if(m_pCastle[0]->IsDead() || m_pCastle[1]->IsDead())
			{
				m_fTimer = (float)clock();
				m_eGameState = GAMEOVER;

				m_pGameOverSprite = new CSprite;
				m_pGameOverSprite->Initialise(L"Data\\Scroll.png", D3DXVECTOR3(0.75f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.8f, 0.75f, 0.75f));

				if(m_pCastle[0]->IsDead())
				{
					m_pSoundManager->PlayLose();
				}
				else
				{
					m_pSoundManager->PlayWin();
				}
			}
		}
		else if(m_eGameState == MENU)
		{
			if(GetAsyncKeyState(VK_F1) & 0x8000)
			{
				m_eGameState = HOWTOPLAY;

				DeleteMenuObjects();

				CSprite* pSprite = new CSprite;
				pSprite->Initialise(L"Data\\howtoplay.png", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				m_vecpGameObjects.push_back(pSprite);
			}
		}
		else if(m_eGameState == HOWTOPLAY)
		{
			if(m_bKeyDown)
			{
				m_eGameState = MENU;
				DeleteMenuObjects();
				StartMenuSetup();
			}
		}
	}
}

void CGame::SetKeyDown(bool _b)
{
	m_bKeyDown = _b;
}

void CGame::Render()
{
	// Begin scene 
	m_pDirectXClass->BeginScene(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// If in game:
	if(m_eGameState == GAME || m_eGameState == GAMEOVER)
	{		
		if(m_eGameState == GAMEOVER)
		{
			// Game over sprite
			RenderSprite(m_pGameOverSprite);		
		}
		// Render health bars
		for (unsigned int i = 0; i < 2; ++i)
		{			
			m_pHealthBar->SetPosition(D3DXVECTOR3(13.0f * (i == 0 ? -1.0f : 1.0f), 0.0f, (i == 0 ? 6.0f : 0.5f)));
			m_pHealthBarOutline->SetPosition(D3DXVECTOR3(13.0f * (i == 0 ? -1.0f : 1.0f), 0.0f, (i == 0 ? 6.0f : 0.5f)));
			float fScale = 2.9f * m_pCastle[i]->GetHealth() / g_kiCastleHealth;
			m_pHealthBar->SetScale(D3DXVECTOR3(fScale, 1.0f, 0.2f));
			RenderSprite(m_pHealthBar);	
			RenderSprite(m_pHealthBarOutline);
		}

		// Render question manager
		m_pQuestionManager->Render();

		// Reapply vertex / index buffer for the sprite class (ID3DX10Font changes the device states)
		CSprite::ApplyRenderOptions();

		// Render castles
		for (unsigned int i = 0; i < 2; ++i)
		{		
			RenderSprite(m_pCastle[i]);	
		}
	}

	// Render game buttons
	for(unsigned int i = 0; i < m_vecpGameButtons.size(); ++i)
	{
		RenderSprite(m_vecpGameButtons[i]);
	}

	// Render game sprites
	for(unsigned int i = 0; i < m_vecpGameObjects.size(); ++i)
	{
		RenderSprite(m_vecpGameObjects[i]);
	}

	/* FPS COUNTER
	if(m_eGameState == GAME)
	{
		// Render FPS
		RECT r = {0, 0, 0, 0};
		std::wstringstream ss;
		ss << "FPS: " << 1.0f / m_fDelta;
		RenderText(ss.str().c_str(), r, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// Reapply vertex / index buffer for the sprite class (ID3DX10Font changes the device states)
		CSprite::ApplyRenderOptions();
	}
	*/

	if(m_eGameState == GAMEOVER)
	{
		CSystemClass::GetInstance()->GetGame()->GetSoundManager()->StopSound("CASTLE_DAMAGE");

		// Render game over text		
		std::wstringstream ss;
		ss << "Game Over, ";
		ss << (m_pCastle[0]->IsDead() ? " You Lost.." : " You Won!");
		ss << "\n\nMonsters Killed: " << m_pQuestionManager->GetMonsterKills();
		ss << "\nTime Survived: " << int((m_fTimer - m_fGameTime) / 1000) << " seconds\n";
		float fTime = 7500.0f - ((float)clock() - m_fTimer);
		ss << "\nThe game will return to the main menu in " << int(fTime / 1000) << " seconds..";
		ss << "\nThanks for playing!\n";

		RECT r = {m_iWidth / 2 - 200, m_iHeight / 2 - 75, 0, 0};
		RenderText(ss.str().c_str(), r, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// Reapply vertex / index buffer for the sprite class (ID3DX10Font changes the device states)
		CSprite::ApplyRenderOptions();
	}

	// If in menu
	if(m_eGameState == MENU)
	{
		RECT r = {m_iWidth / 2 - 100, m_iHeight / 2 + 90, 0, 0};
		RenderText(L"Push F1 for help", r, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

		// Reapply vertex / index buffer for the sprite class (ID3DX10Font changes the device states)
		CSprite::ApplyRenderOptions();
	}

	// End scene
	m_pDirectXClass->EndScene();
}

void CGame::RenderText(LPCWSTR _str, RECT _r, D3DXCOLOR _c)
{
	m_spFont->DrawText(nullptr, _str, -1, &_r, DT_NOCLIP, _c);
}

void CGame::StartMenuSetup()
{
	CButton* pButton = new CButton;
	pButton->Initialise(L"Data\\Start.png", D3DXVECTOR3(-0.34f, 0.0f, -0.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 1.0f, 4.0f), D3DXVECTOR2(160.0f, 70.0f), 
							D3DXVECTOR4(0.502f, 0.0f, 0.998f, 1.0f), D3DXVECTOR4(0.002f, 0.0f, 0.498f, 1.0f));
	pButton->NormalSize();
	m_vecpGameButtons.push_back(pButton);

	CSprite* pBackground = new CSprite;
	pBackground->Initialise(L"Data\\MenuBackground.png", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecpGameObjects.push_back(pBackground);

	m_eGameState = MENU;

	// Play our menu music
	m_pSoundManager->PlaySound("MENU_MUSIC", true);
}

void CGame::GameSetup()
{
	m_fGameTime = (float)clock();

	// Buttons
	CSprite* pSprite4 = new CSprite;
	pSprite4->Initialise(L"Data\\B1.png", D3DXVECTOR3(-14.5f, 0.0f, 8.75f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecpGameObjects.push_back(pSprite4);
	
	CSprite* pSprite5 = new CSprite;
	pSprite5->Initialise(L"Data\\B2.png", D3DXVECTOR3(-12.0f, 0.0f, 8.75f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecpGameObjects.push_back(pSprite5);

	CSprite* pSprite6 = new CSprite;
	pSprite6->Initialise(L"Data\\B3.png", D3DXVECTOR3(-9.5f, 0.0f, 8.75f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecpGameObjects.push_back(pSprite6);
	
	CSprite* pSprite7 = new CSprite;
	pSprite7->Initialise(L"Data\\B4.png", D3DXVECTOR3(-7.0f, 0.0f, 8.75f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecpGameObjects.push_back(pSprite7);

	// Castles
	for (int i = 0; i < 2; ++i)
	{
		m_pCastle[i] = new CCastle;
	}

	m_pCastle[0]->Initialise(L"Data\\Castle.png", D3DXVECTOR3(-12.1f, 0.0f, -0.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 1.0f, 6.5f), true, D3DXVECTOR4(0.0f, 0.0f, 0.333334f, 1.0f));
	m_pCastle[1]->Initialise(L"Data\\Cave.png", D3DXVECTOR3(12.1f, 0.0f, -2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 1.0f, 6.0f), false, D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f));
	
	m_pCastle[0]->NormalSize();
	m_pCastle[1]->NormalSize();

	// HP bars
	m_pHealthBar = new CSprite;
	m_pHealthBarOutline = new CSprite;
	m_pHealthBar->Initialise(L"Data\\hpbar.png", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 1.0f, 1.0f));
	m_pHealthBarOutline->Initialise(L"Data\\hpbaroutline.png", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 1.0f, 1.0f));

	m_pHealthBar->NormalSize();

	// Background
	CSprite* pSprite1 = new CSprite;
	pSprite1->Initialise(L"Data\\Background.png", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecpGameObjects.push_back(pSprite1);

	m_eGameState = GAME;
	CSystemClass::GetInstance()->GetTimer()->Reset();

	// Initialise question / monster managers
	m_pQuestionManager->Initialise(m_iWidth, m_iHeight);

	// Turn off menu music and turn on game music
	m_pSoundManager->StopSound("MENU_MUSIC");
	m_pSoundManager->PlaySound("GAME_MUSIC", true);
}

void CGame::RenderSprite(void* _pSprite)
{
	CSprite* pSprite = (CSprite*)_pSprite;

	m_pSpriteShader->Render(pSprite->GetWorldMatrix(), m_pCameraClass->GetViewMatrix(), m_pCameraClass->GetProjectionMatrix(),
							pSprite->GetTextureMatrix(), pSprite->GetTexture(), pSprite->GetFaceCount());
}

CQuestionManager* CGame::GetQuestionManager()
{
	return(m_pQuestionManager);
}

CCastle* CGame::GetCastle(int _iCastle)
{
	return(m_pCastle[_iCastle]);
}

CSoundManager* CGame::GetSoundManager()
{
	return (m_pSoundManager);
}
// Self Include:
#include "QuestionManager.h"

// Local Includes:
#include "System.h"

// Library Includes:

// Implementation:
CQuestionManager::CQuestionManager() : m_fTimer(0.0f), m_pMonsterManager(nullptr), m_eState(NONE), m_strCurrentQuestion(""),
										m_strInput(""), m_pTextboxSprite(nullptr), m_strCurrentAnswer("")
{
	for (UINT i = 0 ; i < 4; ++i)
	{
		m_vecQuestions[i].clear();
	}
}

CQuestionManager::~CQuestionManager()
{
	DeleteObjects();
}

int CQuestionManager::GetMonsterKills() const
{
	return(m_pMonsterManager->GetMonsterKills());
}

void CQuestionManager::DeleteObjects()
{
	DeleteOBJ(m_pMonsterManager);
	DeleteOBJ(m_pTextboxSprite);
}

void CQuestionManager::Initialise(int _iScreenWidth, int _iScreenHeight)
{
	// Create monster manager class
	m_pMonsterManager = new CMonsterManager;

	// Initialise monster manager
	m_pMonsterManager->Initialise(_iScreenWidth, _iScreenHeight);

	// Create textbox sprite
	m_pTextboxSprite = new CSprite;

	// Initialise textbox sprite
	m_pTextboxSprite->Initialise(L"Data\\Textbox.png", D3DXVECTOR3(0.0f, 0.0f, (4.0f / 1050.0f) * _iScreenHeight), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 0.5f));
	
	m_vecQuestions[0] = OpenQuestionFile("Data\\QuestionsLv1.txt");
	m_vecQuestions[1] = OpenQuestionFile("Data\\QuestionsLv2.txt");
	m_vecQuestions[2] = OpenQuestionFile("Data\\QuestionsLv3.txt");
	m_vecQuestions[3] = OpenQuestionFile("Data\\QuestionsLv4.txt");
}

void CQuestionManager::Render()
{
	// Render monster manager
	m_pMonsterManager->Render();

	if(m_eState == QUESTION)
	{
		CGame::RenderSprite(m_pTextboxSprite);

		std::wstring stemp = std::wstring(m_strCurrentQuestion.begin(), m_strCurrentQuestion.end());

		RECT r = {375, 225, 0, 0};
		CGame::RenderText(stemp.c_str(), r, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//m_pFont->DrawText(

		RECT r2 = {375, 365, 0, 0};
		CGame::RenderText(L"ANSWER: ", r2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		stemp = std::wstring(m_strInput.begin(), m_strInput.end());

		RECT r3 = {375, 400, 0, 0};
		CGame::RenderText(stemp.c_str(), r3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if(m_eState > QUESTION)
	{
		CGame::RenderSprite(m_pTextboxSprite);

		RECT r = {400, 250, 0, 0};

		if(m_eState == ANSWER_RIGHT)
		{			
			CGame::RenderText(L"Unit has been spawned!", r, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else 
		{
			CGame::RenderText(L"Incorrect Answer!", r, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

void CQuestionManager::Process(float _fDelta, float _fTime)
{
	if(m_eState == NONE)
	{
		// Checks for the spawn monster buttons (1 - 4)
		if(GetAsyncKeyState(0x31) & 0x8000) // 1
		{
			if(IsSpaceToSpawn())
			{
				m_eMonsterTypeSpawning = SWORDSMANL1;
				PickQuestion(_fTime, m_eMonsterTypeSpawning);
			}	
		}
		else if(GetAsyncKeyState(0x32) & 0x8000) // 2
		{
			if(IsSpaceToSpawn())
			{
				m_eMonsterTypeSpawning = SWORDSMANL2;
				PickQuestion(_fTime, m_eMonsterTypeSpawning);
			}
		}
		else if(GetAsyncKeyState(0x33) & 0x8000) // 3
		{
			if(IsSpaceToSpawn())
			{
				m_eMonsterTypeSpawning = SWORDSMANL3;
				PickQuestion(_fTime, m_eMonsterTypeSpawning);
			}
		}
		else if(GetAsyncKeyState(0x34) & 0x8000) // 4
		{
			if(IsSpaceToSpawn())
			{
				m_eMonsterTypeSpawning = ARCHER;
				PickQuestion(_fTime, m_eMonsterTypeSpawning);
			}
		}
	}
	else if(m_eState == QUESTION)
	{
		// Check for entering an answer
		if(GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			// convert both to lowercase to ensure case is not a problem
			std::transform(m_strInput.begin(), m_strInput.end(), m_strInput.begin(), ::tolower);
			std::transform(m_strCurrentAnswer.begin(), m_strCurrentAnswer.end(), m_strCurrentAnswer.begin(), ::tolower);

			m_fTimer = _fTime;

			if(m_strInput == m_strCurrentAnswer)
			{
				m_eState = ANSWER_RIGHT;
				m_pMonsterManager->SpawnMonster(true, _fTime, m_eMonsterTypeSpawning);
			}
			else
			{
				m_eState = ANSWER_WRONG;
				m_pMonsterManager->TriggerCoolDown(_fTime, m_eMonsterTypeSpawning);
			}
		}
	}
	else
	{
		if(_fTime > m_fTimer + 1.0f)
		{
			//m_fTimer = _fTime;
			m_eState = NONE;	
		}	
	}

	// Process monster manager
	m_pMonsterManager->Process(_fDelta, _fTime);
}

bool CQuestionManager::IsSpaceToSpawn()
{
	CMonster* pMonster = m_pMonsterManager->GetLatestMonster();

	// Check if there is room to spawn
	if(pMonster == nullptr || D3DXVec3Length(&(pMonster->GetPosition() - D3DXVECTOR3(-6.0f, 0.0f, 10.0f))) > 2.0f)
	{
		return(true);
	}

	return(false);
}

void CQuestionManager::PickQuestion(float _fTime, MONSTER _eType)
{
	// if the cooldown is ready
	if(m_pMonsterManager->ReadyToSpawn(true, _fTime, m_eMonsterTypeSpawning))
	{
		m_eState = QUESTION;
		m_strInput = "";

		int iIndex = (m_eMonsterTypeSpawning == ARCHER ? 3 : m_eMonsterTypeSpawning);

		int iRand = rand() % (m_vecQuestions[iIndex].size() / 2);
		m_strCurrentQuestion = m_vecQuestions[iIndex][iRand * 2];
		m_strCurrentAnswer = m_vecQuestions[iIndex][iRand * 2 + 1];
	}
}

void CQuestionManager::SetKeyDown(char _cKey)
{
	if(m_eState == QUESTION)
	{
		if(_cKey == 8)
		{
			if(m_strInput.size() > 0)
			{
				m_strInput.pop_back();
			}
		}
		else if(m_strInput.size() < 100)
		{
			m_strInput.push_back(_cKey);
		}
	}
}

std::vector<std::string> CQuestionManager::OpenQuestionFile(std::string _strFileName)
{
	std::ifstream myfile(_strFileName);
	std::string strLine;
	std::vector<std::string> vecQuestions;

	if(myfile.is_open())
	{
		while(getline(myfile, strLine))
		{
			vecQuestions.push_back(strLine);
		}

		myfile.close();
	}
	else
	{
		MessageBox(NULL, L"Finding Questions File Failed", L"ERROR", MB_OK);	
		PostQuitMessage(0);
	}

	return(vecQuestions);
}
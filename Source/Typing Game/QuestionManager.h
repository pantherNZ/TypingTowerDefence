#pragma once

#ifndef _CQUESTIONMANAGER_H_
#define _CQUESTIONMANAGER_H_

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "Sprite.h"
#include "MonsterManager.h"

class CGame;

// Class Declaration:
class CQuestionManager
{
	// class functions
	public:
		CQuestionManager();
		~CQuestionManager();

		void Initialise(int _iScreenWidth, int _iScreenHeight);
		void Process(float _fDelta, float _fTime);
		void Render();
		void SetKeyDown(char _cKey);
		int GetMonsterKills() const;
		void DeleteObjects();

	protected:

	private:
		std::vector<std::string> OpenQuestionFile(std::string _strFileName);
		void PickQuestion(float _fTime, MONSTER _eType);
		bool IsSpaceToSpawn();

	// class members
	public:

	protected:

	private:
		CSprite* m_pTextboxSprite;
		CMonsterManager* m_pMonsterManager;

		std::string m_strInput;
		std::string m_strCurrentQuestion;
		std::string m_strCurrentAnswer;

		//std::vector<std::string> m_vecQuestionsLv1;

		std::vector<std::string> m_vecQuestions[4];

		float m_fTimer;
		QUESTIONSTATE m_eState;
		MONSTER m_eMonsterTypeSpawning;
};

#endif // _CQUESTIONMANAGER_H_
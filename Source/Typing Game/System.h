#pragma once

#ifndef _CSYSTEMCLASS_H_
#define _CSYSTEMCLASS_H_

// Definitions

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "Game.h"
#include "QuestionManager.h"
#include "Timer.h"

// Class Declaration:
class CSystemClass
{
	// class functions
	public:	
		~CSystemClass();

		bool Initialise(HINSTANCE _hInstance);
		void Run();

		LRESULT CALLBACK MessageHandler(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);

		CGame* GetGame();
		CTimer* GetTimer();

		// Singleton Methods
		static CSystemClass* GetInstance();
		static void DestroyInstance();

	protected:

	private:

		// private constructor (singleton)
		CSystemClass();

		// privately called functions
		void ExecuteFrame();
		void CreateRegisterWindow();
		void Process();

	// class members
	public:

	protected:

	private:
		// Singleton Instance
		static CSystemClass* s_pApplication;

		// System app data
		HWND m_hWnd;
		int m_iWidth;
		int m_iHeight;

		// Other data
		HMENU hMenu;
		LPCWSTR m_applicationName;
		HINSTANCE m_hInstance;		

		// Classes used
		CGame* m_pGame;
		CTimer* m_pTimer;
		CQuestionManager* m_pQuestionManager;

		// Dialog data
		HWND m_hDlg;
};

// Windows callback definition
static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);

#endif // _CSYSTEMCLASS_H_
// Self Include:
#include "System.h"

// Local Includes:
//#include "resource.h"

// Library Includes:

// Implementation:
CSystemClass::CSystemClass() : m_pTimer(nullptr), m_pQuestionManager(nullptr)
{

}

CSystemClass::~CSystemClass()
{
	// Remove the window.
	DestroyWindow(m_hWnd);
	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hInstance);

	// delete used classes
	DeleteOBJ(m_pGame);
	DeleteOBJ(m_pTimer);
}

bool CSystemClass::Initialise(HINSTANCE _hInstance)
{
	// Basic program settings
	m_applicationName = L"Typing TD";
	m_hInstance = _hInstance;

	// Initialise the windows api
	CreateRegisterWindow();

	// Create timer 
	m_pTimer = new CTimer;

	// Reset timer
	m_pTimer->Reset();

	// Create game class
	m_pGame = new CGame;

	// Initialise game class
	if(!m_pGame->Initialise(m_hInstance, m_hWnd, m_iWidth, m_iHeight))
	{
		return(false);
	}

	m_pQuestionManager = m_pGame->GetQuestionManager();

	return(true);
}

void CSystemClass::CreateRegisterWindow()
{
	WNDCLASSEX wc;

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	// Register the window class.
	if(!RegisterClassEx(&wc))
	{
		MessageBox(0, L"Registering Windows Class Failed", 0, 0);
		PostQuitMessage(0);
	}

	// Determine the resolution of the clients desktop screen.
	m_iWidth = 1600;//GetSystemMetrics(SM_CXSCREEN);
	m_iHeight = 1024;//GetSystemMetrics(SM_CYSCREEN);

	// Create the window with the screen settings and get the handle to it.
	m_hWnd = CreateWindowEx(NULL, m_applicationName, m_applicationName, 
						WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, m_iWidth, m_iHeight,
						NULL, NULL, m_hInstance, NULL);

	// check if creating the window failed
	if(!m_hWnd)
	{
		MessageBox(0, L"Creating Windows Class Failed", 0, 0);
		PostQuitMessage(0);
	}

	// Move window to the center of the screen

	RECT rect;
	GetWindowRect ( m_hWnd, &rect ) ;

	int xPos = (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_iWidth / 2);
	int yPos = (GetSystemMetrics(SM_CYSCREEN) / 2) - (m_iHeight / 2);

	SetWindowPos( m_hWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
}

void CSystemClass::Run()
{
	MSG msg;

	// Initialise the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			ExecuteFrame();
		}
	}
}

void CSystemClass::ExecuteFrame()
{
	// tick the timer
	m_pTimer->Tick();

	// process updates
	Process();

	// render scene
	m_pGame->Render();
}

void CSystemClass::Process()
{
	// Check if the user pressed escape and wants to exit the application.
	if(GetAsyncKeyState(VK_ESCAPE))
	{
		// delete used classes
		//DeleteOBJ(m_pGame);
		//DeleteOBJ(m_pTimer);
		PostQuitMessage(0);
		return;
	}

	m_pGame->Process(m_pTimer->GetDeltaTime(), m_pTimer->GetGameTime());
}

CGame* CSystemClass::GetGame()
{
	return(m_pGame);
}

CTimer* CSystemClass::GetTimer()
{
	return(m_pTimer);
}

CSystemClass* CSystemClass::GetInstance()
{
	if (s_pApplication == nullptr)
	{
		s_pApplication = new CSystemClass;
	}

	return (s_pApplication);
}

void CSystemClass::DestroyInstance()
{
	DeleteOBJ(s_pApplication);
}

LRESULT CALLBACK CSystemClass::MessageHandler(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch(_uiMsg)
	{
		// Key up / down
		case WM_MOUSEMOVE:
		{
			int iMouseX = LOWORD(_lParam);
			int iMouseY = HIWORD(_lParam);

			// update the button static variable values for X & Y
			CButton::SetMousePosition(iMouseX - m_iWidth / 2, iMouseY - m_iHeight / 2);
		}
		break;

		case WM_LBUTTONDOWN:
		{
			// if mouse clicked, update button static variable
			CButton::SetMouseState(true);
		}
		break;

		case WM_LBUTTONUP:
		{
			// if mouse un-clicked, update button static variable
			CButton::SetMouseState(false);
		}
		break;

		case WM_KEYDOWN:
		{
			char cKey = char(_wParam);
			
			if((cKey >= 'a' && cKey <= 'z') || (cKey >= 'A' && cKey <= 'Z') || cKey == ' ' || cKey == 8) // (cKey >= '0' && cKey <= '9') || 
			{
				m_pQuestionManager->SetKeyDown(cKey);
			}

			
			if(!(GetAsyncKeyState(VK_F1) & 0x8000))
			{
				m_pGame->SetKeyDown(true);
			}
		}
		break;
		
		case WM_KEYUP:
		{
			m_pGame->SetKeyDown(false);
		}
		break;

		default:
		{
			break;
		}
	}
	
	return(DefWindowProc(_hwnd, _uiMsg, _wParam, _lParam));
}

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch(_uiMsg)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return(CSystemClass::GetInstance()->MessageHandler(_hwnd, _uiMsg, _wParam, _lParam));
		}
	}
}
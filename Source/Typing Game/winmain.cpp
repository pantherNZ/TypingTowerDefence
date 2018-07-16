// Library Includes:

// Local Includes:
#include "Utility.h"
#include "System.h"

// Initialise pointer of instance
CSystemClass* CSystemClass::s_pApplication = nullptr;

// Winmain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Seed rand
	srand((unsigned int)(time(0)));

	// Create system class instance 
	CSystemClass* pSystemClass = CSystemClass::GetInstance();

	// Initialise system class
	if(pSystemClass->Initialise(hInstance))
	{
		// Run the game
		pSystemClass->Run();
	}

	// Destroy system class instance
	CSystemClass::DestroyInstance();

	return(0);
}
#include <windows.h>
#include "System.h"

//******************************************************************

int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow )
{
	// Normalement faudrait au moins faire un GetInstance de
	// tous les singletons ici, �a appelle le constructeur, et
	// �a �vite de se retrouver avec un singleton pas initialis�
	// en plein jeu (m�me le must serait d'avoir une m�thode Initialize
	// pour les singletons, et ne rien faire dans le constructeur).
	InputManager::GetInstance()/*->Initialize()*/;
	Renderer::GetInstance()/*->Initialize()*/;
	//..
	
	//TODO
	
	return System::GetInstance()->MainLoop();
}

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
	// tous les singletons ici, ça appelle le constructeur, et
	// ça évite de se retrouver avec un singleton pas initialisé
	// en plein jeu (même le must serait d'avoir une méthode Initialize
	// pour les singletons, et ne rien faire dans le constructeur).
	InputManager::GetInstance()/*->Initialize()*/;
	Renderer::GetInstance()/*->Initialize()*/;
	//..
	
	//TODO
	
	return System::GetInstance()->MainLoop();
}

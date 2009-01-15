#include	<windows.h>
#include	"Core/System.h"

//******************************************************************

/***********************************************************
 * Fonction principale.
 * @param[in]	hInstance		:
 * @param[in]	hPrevInstance	:
 * @param[in]	lpCmdLine		:
 * @param[in]	nCmdShow		:
 * @return	
 **********************************************************/
int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow )
{
	int loopResult;
	
	// Initialisation du System
	System::GetInstance()->Initialize();

	// Boucle principale..
	loopResult = System::GetInstance()->MainLoop();
	
	return EXIT_SUCCESS;
}

//******************************************************************

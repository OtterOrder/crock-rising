
#include	<Core/System.h>
#include	<Game/Game.h>

//******************************************************************

#define		LEVEL_start				0x9f79558f
#define		LEVEL_alafraise			0xa77df32f

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

	// Création du System
	System::GetInstance();
	
	// On indique le niveau de démarrage
	Game::GetInstance()->ChangeLevel( LEVEL_alafraise );

	// On lance la boucle principale..
	loopResult = System::GetInstance()->MainLoop();

	// Destruction du System
	System::GetInstance()->Destroy();

	return EXIT_SUCCESS;
}

//******************************************************************

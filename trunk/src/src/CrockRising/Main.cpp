
#include	<Core/System.h>
#include	<Game/Game.h>

#include	"Backup.h"

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

	// Nom du jeu
	Game::GetInstance()->SetGameName( "Crock Rising" );

	// Création/enregistrement de la sauvegarde
	CRBackup *pBackup = new CRBackup;
	Game::GetInstance()->StoreBackup( pBackup );
	
	// On indique le niveau de démarrage
	Game::GetInstance()->SetStartLevel( LEVEL_alafraise );

	// Création et initialisation du System
	System::GetInstance()->SetFullScreen( true, pBackup->GetResolution().x, pBackup->GetResolution().y );
	System::GetInstance()->Init();

	// On lance la boucle principale..
	loopResult = System::GetInstance()->MainLoop();

	// Destruction du System
	System::GetInstance()->Destroy();

	return EXIT_SUCCESS;
}

//******************************************************************

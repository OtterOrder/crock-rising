#include	<Game/Level.h>

// Logos
#include	"LevelAlafraise.h"

// Menus
#include	"levelDefeat.h"
#include	"levelGameMenu.h"
#include	"LevelHighScores.h"
#include	"LevelMainMenu.h"
#include	"LevelSettings.h"
#include	"LevelVictory.h"

// Jeux
#include	"LevelCanyon.h"

//******************************************************************

/***********************************************************
 * Créé un objet du type spécifié par l'ID.
 * @param[in]	levelID	: ID du niveau
 * @return	pointeur sur l'instance de l'objet
 **********************************************************/
Level* Level::NewLevel( crc32 levelID )
{
	Level *pLevel = NULL;

	switch( levelID )
	{
		// Logos
		case LEVEL_alafraise:	pLevel = new LevelAlafraise( levelID );		break;
		
		// Menus
		case LEVEL_defeat:		pLevel = new LevelDefeat( levelID );		break;
		case LEVEL_gamemenu:	pLevel = new LevelGameMenu( levelID );		break;
		case LEVEL_highscores:	pLevel = new LevelHighScores( levelID );	break;
		case LEVEL_mainmenu:	pLevel = new LevelMainMenu( levelID );		break;
		case LEVEL_settings:	pLevel = new LevelSettings( levelID );		break;
		case LEVEL_victory:		pLevel = new LevelVictory( levelID );		break;

		// Jeux
		case LEVEL_canyon:		pLevel = new LevelCanyon( levelID );		break;
		
		default:				pLevel = new Level( LEVEL_level );
	}
	return pLevel;
}

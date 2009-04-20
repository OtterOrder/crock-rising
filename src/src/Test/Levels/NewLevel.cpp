#include	<Game/Level.h>

#include	"LevelStart.h"
#include	"LevelDemo_20_4.h"

//******************************************************************

/***********************************************************
 * Créé un objet du type spécifié par l'ID.
 * @param[in]	levelID	: ID du niveau
 * @return	pointeur sur l'instance de l'objet
 **********************************************************/
Level* Level::NewLevel( crc32 levelID )
{
	Level *pLevel;
	
	switch( levelID )
	{
		case LEVEL_start:		pLevel = new LevelStart( levelID );		break;
		case LEVEL_demo_20_4:	pLevel = new LevelDemo_20_4( levelID );	break;
		
		default:				pLevel = new Level( LEVEL_level );
	}
	return pLevel;
}

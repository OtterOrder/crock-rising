#include	"Game/Level.h"

#include	"LevelStart.h"

//******************************************************************

/***********************************************************
 * Créé un objet du type spécifié par l'ID.
 * @param[in]	levelID	: ID du niveau
 * @return	pointeur sur l'instance de l'objet
 **********************************************************/
Level* Level::NewLevel( crc32 levelID )
{
	switch( levelID )
	{
		case LEVEL_start:	return new LevelStart( levelID );
		
		default:			return new Level( levelID );
	}
}

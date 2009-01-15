#include	"Game/Level.h"

#include	"LevelStart.h"

//******************************************************************

/***********************************************************
 * Cr�� un objet du type sp�cifi� par l'ID.
 * @param[in]	levelID	: ID du niveau
 * @return	pointeur sur l'instance de l'objet
 **********************************************************/
Level* Level::NewLevel( crc32 levelID )
{
	Level *pLevel;
	
	switch( levelID )
	{
		case LEVEL_start:	pLevel = new LevelStart( levelID );		break;
		
		default:			pLevel = new Level( LEVEL_level );
	}
	return pLevel;
}

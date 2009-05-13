#ifndef		_Level_H
#define		_Level_H

//******************************************************************

#include	"Core/Types/Crc32.h"

//******************************************************************

// Note: le CRC32 du niveau est calcul� sans tenir compte
// de pr�fixe LEVEL_. Par exemple, 0x9aeacc13 est le CRC32
// de "level". Cependant, on rajoute le pr�fixe au define
// histoire de savoir de quoi il s'agit.

#define		LEVEL_level			0x9aeacc13

//******************************************************************

class Level
{
public:

	Level						( crc32 levelID );		// Constructeur
	virtual ~Level				( void );				// Destructeur

	virtual void	Init		( void );				// Initialisation
	virtual void	Update		( void );				// Update du niveau

	crc32			GetLevelID	( void ) const;			// Donne l'ID du niveau

	// Cette m�thode permet de cr�er un objet du bon type
	// en fonction du levelID, � d�finir dans le jeu..
	static Level*	NewLevel	( crc32 levelID );


protected:

	crc32		m_LevelID;		// ID du niveau

};

//******************************************************************

/*
Exemple pour la m�thode NewLevel :
{
	switch( levelID )
	{
		case LEVEL_lol:		return new LevelLol( levelID );
		//..

		default:			return new Level( levelID );
	}
}
*/

//******************************************************************
#endif		//_Level_H

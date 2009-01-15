#ifndef		_Game_H
#define		_Game_H

//******************************************************************

#include	"Core/Singleton.h"
#include	"Core/Types/Crc32.h"

//******************************************************************

class Level;

class Game : public Singleton< Game >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< Game >;

public:

	// =========================================================
	// Méthodes publiques

	void	Update			( void );			// Update du jeu
	
	void	ChangeLevel		( crc32 levelID );	// Change de niveau
	crc32	GetLevelID		( void );			// Donne l'ID du niveau courant


protected:

	// =========================================================
	// Données protégées
	
	Level	*m_CurrentLevel;		// Niveau courant


	// =========================================================
	// Méthodes protégées

	Game			( void );		// Constructeur
	virtual ~Game	( void );		// Destructeur

};

//******************************************************************
#endif		//_Game_H

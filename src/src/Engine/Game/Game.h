#ifndef		_Game_H
#define		_Game_H

//******************************************************************

#include	<list>

#include	"Core/Singleton.h"
#include	"Core/Types/Crc32.h"

//******************************************************************

class Level;
class Object;

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
	crc32	GetLevelID		( void ) const;		// Donne l'ID du niveau courant
	crc32	GetPrevLevelID	( void ) const;		// Donne l'ID du niveau précédent


protected:

	// =========================================================
	// Types protégés

	typedef std::list< Object* >::iterator		ObjIt;		// Itérateur d'objet

	// =========================================================
	// Données protégées
	
	Level					*m_CurrentLevel;	// Niveau courant
	crc32					m_PrevLevelID;		// ID du niveau précédent

	std::list< Object* >	*m_ObjList;			// Liste des objets 3d

	// =========================================================
	// Méthodes protégées

	Game			( void );		// Constructeur
	virtual ~Game	( void );		// Destructeur

};

//******************************************************************
#endif		//_Game_H

#ifndef		_Game_H
#define		_Game_H

//******************************************************************

#include	<list>

#include	"Core/Singleton.h"
#include	"Core/Types/Crc32.h"

//******************************************************************

class Level;
class Object;
class Object2D;

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
	// Types protégés

	typedef std::list< Object* >::iterator		ObjIt;		// Itérateur d'objet
	typedef std::list< Object2D* >::iterator	Obj2DIt;	// Itérateur d'objet 2D

	// =========================================================
	// Données protégées
	
	Level					*m_CurrentLevel;		// Niveau courant

	std::list< Object* >	*m_ObjList;				// Liste des objets 3d
	std::list< Object2D* >	*m_Obj2DList;			// Liste des objets 2d


	// =========================================================
	// Méthodes protégées

	Game			( void );		// Constructeur
	virtual ~Game	( void );		// Destructeur

};

//******************************************************************
#endif		//_Game_H

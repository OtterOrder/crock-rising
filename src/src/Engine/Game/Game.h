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
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< Game >;

public:

	// =========================================================
	// M�thodes publiques

	void	Update			( void );			// Update du jeu
	
	void	ChangeLevel		( crc32 levelID );	// Change de niveau
	crc32	GetLevelID		( void ) const;		// Donne l'ID du niveau courant
	crc32	GetPrevLevelID	( void ) const;		// Donne l'ID du niveau pr�c�dent


protected:

	// =========================================================
	// Types prot�g�s

	typedef std::list< Object* >::iterator		ObjIt;		// It�rateur d'objet

	// =========================================================
	// Donn�es prot�g�es
	
	Level					*m_CurrentLevel;	// Niveau courant
	crc32					m_PrevLevelID;		// ID du niveau pr�c�dent

	std::list< Object* >	*m_ObjList;			// Liste des objets 3d

	// =========================================================
	// M�thodes prot�g�es

	Game			( void );		// Constructeur
	virtual ~Game	( void );		// Destructeur

};

//******************************************************************
#endif		//_Game_H

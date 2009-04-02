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

	//-- Structures publiques

	struct Backup
	{
		// A dériver..
		// La sauvegarde sert à ce qu'on veut. On peut y stocker toutes les
		// données globales du jeu. Il serait également judicieux d'y ajouter
		// une interface de chargement/sauvegarde dans un fichier, etc.

		virtual ~Backup(){}
	};

	//-- Méthodes publiques

	void	Update			( void );			// Update du jeu

	void	StoreBackup		( Backup *pBackup );
	Backup*	GetBackup		( void ) const;
	
	void	ChangeLevel		( crc32 levelID );	// Change de niveau
	crc32	GetLevelID		( void ) const;		// Donne l'ID du niveau courant
	crc32	GetPrevLevelID	( void ) const;		// Donne l'ID du niveau précédent

protected:

	//-- Types protégés

	typedef std::list< Object* >::iterator		ObjIt;		// Itérateur d'objet

	//-- Données protégées
	
	Backup					*m_Backup;			// Sauvegarde du jeu (ce qu'on veut)
	
	Level					*m_CurrentLevel;	// Niveau courant
	crc32					m_PrevLevelID;		// ID du niveau précédent

	std::list< Object* >	*m_ObjList;			// Liste des objets 3d

	//-- Méthodes protégées

	Game			( void );		// Constructeur
	virtual ~Game	( void );		// Destructeur

};

//******************************************************************
#endif		//_Game_H

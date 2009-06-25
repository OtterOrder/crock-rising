#ifndef		_Game_H
#define		_Game_H

//******************************************************************

#include	<list>

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

	void				Start			( void );				// Lance le niveau de départ
	void				Update			( void );				// Update du jeu

	void				SetGameName		( const std::string &gameName ); // Change le nom du jeu
	const std::string&	GetGameName		( void ) const;			// Donne le nom du jeu

	void				StoreBackup		( Backup *pBackup );
	Backup* const		GetBackup		( void ) const;
	
	void				SetStartLevel	( crc32 levelID );		// Positionne le niveau de départ
	void				ChangeLevel		( crc32 levelID );		// Change de niveau
	Level* const		GetLevel		( void ) const;			// Donne le niveau courant
	crc32				GetLevelID		( void ) const;			// Donne l'ID du niveau courant
	crc32				GetPrevLevelID	( void ) const;			// Donne l'ID du niveau précédent

	void				Pause			( void );
	void				Resume			( void );
	bool				IsPaused		( void ) const;

protected:

	//-- Données protégées
	
	std::string				m_GameName;			// Nom du jeu
	
	Backup					*m_Backup;			// Sauvegarde du jeu (ce qu'on veut)
	
	Level					*m_CurrentLevel;	// Niveau courant
	Level					*m_PrevLevel;		// Niveau à supprimer après un changement de niveau
	crc32					m_PrevLevelID;		// ID du niveau précédent
	crc32					m_StartLevelID;		// ID du niveau de départ

	bool					m_IsPaused;			// Si le jeu est en pause

	//-- Méthodes protégées

	Game			( void );		// Constructeur
	virtual ~Game	( void );		// Destructeur

};

//******************************************************************
#endif		//_Game_H

#ifndef		_InputManager_H
#define		_InputManager_H

//******************************************************************

#include	<libCore/Singleton/Singleton.h>
#include	<libCore/Types/Vector.h>

#include	<windows.h>
#include	<list>

//******************************************************************

class InputManager : public Singleton< InputManager >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< InputManager >;

public:

	// =========================================================
	// Méthodes publiques

	// Note: pour vérifier une lettre, on envoie directement
	// le caractère (par exemple 'A'). Pour les touches spéciales,
	// on utilise les defines de WINAPI qui commencent par VK_
	// (en général, sinon voir dans winuser.h).
	
	bool		IsKeyTriggered		( int key ) const;		// Vérifie si la touche est appuyée
	bool		IsKeyPressed		( int key ) const;		// Vérifie si la touche est maintenue appuyée
	bool		IsKeyReleased		( int key ) const;		// Vérifie si la touche est relachée

	bool		IsMouseTriggered	( int button ) const;	//
	bool		IsMousePressed		( int button ) const;	//
	bool		IsMouseReleased		( int button ) const;	//
	
	Point2f		GetMousePosition	( void ) const;			// Donne la position de la souris
	Vector2f	GetMouseVector		( void ) const;			// Donne le vecteur de la souris

	void		Update				( void );				// Update

	// Fonction de rappel des événements
	LRESULT CALLBACK	EventsCallback	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


protected:

	// =========================================================
	// Enum et stuctures protégées

	enum ItemState
	{
		ITEM_TRIGGERED,
		ITEM_PRESSED,
		ITEM_RELEASED
	};

	struct Item // Désigne une touche ou un bouton
	{
		int			m_Code;			// Code de l'item
		ItemState	m_State;		// Etat de l'item
	};


	// =========================================================
	// Données protégées

	std::list< Item >	m_Keys;					// Liste des touches
	std::list< Item >	m_MouseButtons;			// Liste des boutons de souris
	
	Point2f				m_MouseOldPosition;		// Ancienne position de la souris
	Point2f				m_MousePosition;		// Position de la souris

	
	// =========================================================
	// Méthodes protégées

	InputManager					( void );			// Constructeur
	virtual ~InputManager			( void ){}			// Destructeur

};


//******************************************************************
#endif		// _InputManager_H

#ifndef		_System_H
#define		_System_H

#include	<windows.h>
#include	<d3d9.h>

#include	"Core/Singleton.h"

//******************************************************************

class System : public Singleton< System >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< System >;

public:

	// =========================================================
	// Méthodes publiques

	int		MainLoop	( void );	// Boucle principale de l'appli

	static LRESULT CALLBACK	EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );// Fonction de rappel des événements


protected:

	// =========================================================
	// Méthodes protégées
	
	System				( void );		// Constructeur
	~System				( void );		// Destructeur
	
	HRESULT	InitWindow	( void );		// Initialisation fenêtre windows

	
	// =========================================================
	// Données protégées
	
	HINSTANCE		m_Instance;			// Instance de l'application
	HICON			m_Icon;

};

//******************************************************************
#endif		// _System_H

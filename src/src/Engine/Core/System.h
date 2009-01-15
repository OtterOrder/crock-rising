#ifndef		_System_H
#define		_System_H

#include	<windows.h>
#include	<d3d9.h>

#include	"Core/Singleton.h"

//******************************************************************

class System : public Singleton< System >
{
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< System >;

public:

	// =========================================================
	// M�thodes publiques

	int		MainLoop	( void );	// Boucle principale de l'appli

	static LRESULT CALLBACK	EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );// Fonction de rappel des �v�nements


protected:

	// =========================================================
	// M�thodes prot�g�es
	
	System				( void );		// Constructeur
	~System				( void );		// Destructeur
	
	HRESULT	InitWindow	( void );		// Initialisation fen�tre windows

	
	// =========================================================
	// Donn�es prot�g�es
	
	HINSTANCE		m_Instance;			// Instance de l'application
	HICON			m_Icon;

};

//******************************************************************
#endif		// _System_H

#ifndef		_InputManager_H
#define		_InputManager_H

//******************************************************************

#include	<windows.h>
#include	<libCore/Singleton/Singleton.h>

//******************************************************************

class InputManager : public Singleton< InputManager >
{
		// Indispensable car Singleton doit pouvoir accéder aux
		// constructeur/destructeur, qui sont en protected.
		friend class Singleton< InputManager >;
	
	public:

		// =========================================================
		// Méthodes publiques

		static LRESULT	EventsCallback	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// Fonction de rappel des événements


	private:

		// =========================================================
		// Méthodes privées

		InputManager					( void ){}			// Constructeur
		~InputManager					( void ){}			// Destructeur

};


//******************************************************************
#endif		// _InputManager_H

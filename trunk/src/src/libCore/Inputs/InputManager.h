#ifndef		_InputManager_H
#define		_InputManager_H

//******************************************************************

#include	<windows.h>
#include	<libCore/Singleton/Singleton.h>

//******************************************************************

class InputManager : public Singleton< InputManager >
{
		// Indispensable car Singleton doit pouvoir acc�der aux
		// constructeur/destructeur, qui sont en protected.
		friend class Singleton< InputManager >;
	
	public:

		// =========================================================
		// M�thodes publiques

		static LRESULT	EventsCallback	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// Fonction de rappel des �v�nements


	private:

		// =========================================================
		// M�thodes priv�es

		InputManager					( void ){}			// Constructeur
		~InputManager					( void ){}			// Destructeur

};


//******************************************************************
#endif		// _InputManager_H

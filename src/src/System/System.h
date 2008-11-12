#ifndef		_System_H
#define		_System_H

#include <windows.h>
#include <libRenderer/Renderer.h>
#include <d3d9.h>

//******************************************************************

class System : public Singleton< System >
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< System >;

	public:

		//----------------------------------------------------------
        // Méthodes publiques
        //----------------------------------------------------------

		int MainLoop(); //Boucle principale
		LRESULT InputProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



	protected:

		//----------------------------------------------------------
        // Données membres
        //----------------------------------------------------------

		HINSTANCE m_Instance;  // Instance de l'application

};

//******************************************************************
#endif		// _System_H

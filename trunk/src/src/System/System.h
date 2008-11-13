#ifndef		_System_H
#define		_System_H

#include <windows.h>
#include <libRenderer/Renderer.h>
#include <libCore/Inputs/InputManager.h>
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

		HRESULT InitWindow(); //Initialisation fenêtre windows


		int MainLoop(); //Boucle principale




	protected:

		//----------------------------------------------------------
        // Données membres
        //----------------------------------------------------------

		HINSTANCE m_Instance;  // Instance de l'application
		HICON	  m_hIcon;

};

//******************************************************************
#endif		// _System_H

#ifndef		_System_H
#define		_System_H

#include <windows.h>
#include <libRenderer/Renderer.h>
#include <d3d9.h>

//******************************************************************

class System : public Singleton< System >
{
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< System >;

	public:

		//----------------------------------------------------------
        // M�thodes publiques
        //----------------------------------------------------------

		int MainLoop(); //Boucle principale



	protected:

		//----------------------------------------------------------
        // Donn�es membres
        //----------------------------------------------------------

		Renderer * m_Renderer; 

		HINSTANCE m_Instance;  // Instance de l'application

};

//******************************************************************
#endif		// _System_H

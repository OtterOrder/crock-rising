#ifndef		_Renderer_H
#define		_Renderer_H

//******************************************************************

#include "d3dapp.h"
#include <libCore/Singleton/Singleton.h>

class Renderer : public Singleton< Renderer >, public CD3DApplication 
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< Renderer >;

	public:

		//----------------------------------------------------------
        // Callback DirectX
        //----------------------------------------------------------

		HRESULT OnCreateDevice()                           { return S_OK; }
		HRESULT OnResetDevice()                            { return S_OK; }
		HRESULT Render()                                   { return S_OK; }
		HRESULT OnLostDevice()                             { return S_OK; }
		HRESULT OnDestroyDevice()                          { return S_OK; }

};

//******************************************************************
#endif		// _Renderer_H

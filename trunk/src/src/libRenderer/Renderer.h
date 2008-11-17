#ifndef		_Renderer_H
#define		_Renderer_H

//******************************************************************

#include "d3dapp.h"
#include <libCore/Singleton/Singleton.h>
#include <libObjects/Camera.h>

class Renderer : public Singleton< Renderer >, public CD3DApplication 
{
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< Renderer >;

	public:

		//----------------------------------------------------------
        // Callback DirectX
        //----------------------------------------------------------

		HRESULT BeforeCreateDevice();
		HRESULT OnCreateDevice();                           
		HRESULT OnResetDevice();
		HRESULT Render();                                   
		HRESULT OnLostDevice();
		HRESULT OnDestroyDevice();                          
		HRESULT AfterDestroyDevice()                       { return S_OK; }

		void SetCamera( Camera* cam );

	private:
		Camera* m_Camera; //Une Copie de la camera


};

//******************************************************************
#endif		// _Renderer_H

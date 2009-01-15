#ifndef		_Renderer_H
#define		_Renderer_H

//******************************************************************

#include "D3DApp/d3dapp.h"
#include "Core/Singleton.h"
#include "Objects/Camera.h"

#include "Core/Inputs/InputManager.h"
#include "Resources/Mesh.h"

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

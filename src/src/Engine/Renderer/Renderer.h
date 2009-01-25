#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "D3DApp/d3dapp.h"
#include "Core/Singleton.h"
#include "Objects/Object.h"
#include "Objects/SceneObject.h"
#include "Objects/Camera.h"
#include "Core/Inputs/InputManager.h"
#include <vector>


//===========================================================================//
// Classe de base pour créer une application DX                              //
//===========================================================================//
class Renderer : public Singleton< Renderer >, public CD3DApplication 
{
	friend class Singleton< Renderer >;

	Renderer();
	virtual ~Renderer() {}

public:
    //===========================================================================//
	// Callback DirectX							                                 //
	//===========================================================================//
	HRESULT BeforeCreateDevice();
	HRESULT OnCreateDevice();
	HRESULT OnResetDevice();
	HRESULT FrameMove(float fElapsedTime);
	HRESULT Render();
	HRESULT OnLostDevice();
	HRESULT OnDestroyDevice();
	HRESULT AfterDestroyDevice();

	void SetCamera( Camera* cam );

protected:

	//===========================================================================//
	// Types protégées				                                             //
	//===========================================================================//
	
	typedef std::list< SceneObject* >::iterator ScObjIt;
	
	//===========================================================================//
	// Données membres protégées				                                 //
	//===========================================================================//
	LPDIRECT3DVERTEXBUFFER9				m_pGridVB;
	std::list< SceneObject* >			*m_ScObjList;	// Liste des objets de scène

	Camera* m_Camera; //Une Copie de la camera
};

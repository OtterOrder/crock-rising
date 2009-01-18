#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "D3DApp/d3dapp.h"
#include "../Core/Singleton.h"
#include "../Objects/Object.h"
#include "../Objects/Camera.h"
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

protected:
	//===========================================================================//
	// Données membres protégées				                                 //
	//===========================================================================//
	LPDIRECT3DVERTEXBUFFER9 m_pGridVB;
	std:: vector <Object *> m_ListObj;
};

#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Core/Singleton.h"
#include	"D3DApp/d3dapp.h"
#include	<list>

class SceneObject;
class Camera;
class Object2D;

//===========================================================================//
// Classe de base pour cr�er une application DX                              //
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
	// Types prot�g�es				                                             //
	//===========================================================================//
	
	typedef std::list< SceneObject* >::iterator ScObjIt;
	typedef std::list< Object2D* >::iterator Obj2DIt;
	
	//===========================================================================//
	// Donn�es membres prot�g�es				                                 //
	//===========================================================================//
	LPDIRECT3DVERTEXBUFFER9 m_pGridVB; // ??
	
	std::list< SceneObject* >	*m_ScObjList; // Liste des objets de sc�ne
	std::list< Object2D* >		*m_Obj2DList; // Liste des objets 2d

	Camera *m_Camera; // Pointeur sur la camera

};

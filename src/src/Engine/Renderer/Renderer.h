#ifndef		_Renderer_H
#define		_Renderer_H

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Core/Singleton.h"
#include	"D3DApp/d3dapp.h"
#include	<list>

class SceneObject;
class Camera;
class Object2D;
class Skybox;

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
	void SetSkybox( Skybox* skybox);

protected:

	//===========================================================================//
	// Types protégées				                                             //
	//===========================================================================//
	
	typedef std::list< SceneObject* >::iterator ScObjIt;
	typedef std::list< Object2D* >::iterator Obj2DIt;
	
	//===========================================================================//
	// Données membres protégées				                                 //
	//===========================================================================//
	LPDIRECT3DVERTEXBUFFER9 m_pGridVB; // ?? = VB de la grille
	
	std::list< SceneObject* >	*m_ScObjList; // Liste des objets de scène
	std::list< Object2D* >		*m_Obj2DList; // Liste des objets 2d

	Camera *m_Camera; // Pointeur sur la camera
	Skybox *m_Skybox;

};
#endif

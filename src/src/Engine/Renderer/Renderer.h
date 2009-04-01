#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Core/Singleton.h"
#include	"Core/Types/Color.h"
#include	"D3DApp/d3dapp.h"
#include	<list>

class SceneObject;
class Camera;
class Object2D;
class Light;
class Skybox;

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
	void SetSkybox( Skybox* skybox);

	// Infos sur la fen�tre
	int	GetWindowWidth() const;
	int	GetWindowHeight() const;

	// Acc�s/modif de la couleur de fond
	void SetClearColor( const Color4f &color );
	Color4f GetClearColor() const;

protected:

	//===========================================================================//
	// Types prot�g�es				                                             //
	//===========================================================================//
	
	typedef std::list< SceneObject* >::iterator ScObjIt;
	typedef std::list< Object2D* >::iterator Obj2DIt;
	typedef std::list< Light* >::iterator LightIt;
	
	//===========================================================================//
	// Donn�es membres prot�g�es				                                 //
	//===========================================================================//
	LPDIRECT3DVERTEXBUFFER9 m_pGridVB; // ?? = VB de la grille
	
	std::list< SceneObject* >	*m_ScObjList; // Liste des objets de sc�ne
	std::list< Object2D* >		*m_Obj2DList; // Liste des objets 2d
	std::list< Light* >			*m_LightList; // Liste des lumi�res

	Camera	*m_Camera;		// Pointeur sur la camera
	Skybox	*m_Skybox;
	
	Color4f	m_ClearColor;	// Couleur de fond

};

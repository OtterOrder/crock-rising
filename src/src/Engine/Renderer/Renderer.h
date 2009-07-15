#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Core/Singleton.h"
#include	"Core/Math.h"
#include	"D3DApp/d3dapp.h"
#include	<list>

class SceneObject;
class SceneObject2D;
class Camera;
class Light;
class Skybox;
class ShadowMap;

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
	HRESULT Render();
	HRESULT OnLostDevice();
	HRESULT OnDestroyDevice();
	HRESULT AfterDestroyDevice();

	void SetCamera( Camera* cam );
	void SetSkybox( Skybox* skybox);
	void SetShadowMap( Light * ShadowLight);
	ShadowMap* GetShadowMap(){return m_ShadowMap;}

	// Infos sur la fenêtre
	u32	GetWindowWidth() const;
	u32	GetWindowHeight() const;
	Skybox* GetSkybox(){ return m_Skybox; }

	// Accès/modif de la couleur de fond
	void SetClearColor( const Color4f &color );
	Color4f GetClearColor() const;

	D3DXMATRIX GetViewProjMatrix ();
protected:	
	//===========================================================================//
	// Données membres protégées				                                 //
	//===========================================================================//
	LPDIRECT3DVERTEXBUFFER9 m_pGridVB; // ?? = VB de la grille
	
	std::list< SceneObject* >	*m_ScObjList; // Liste des objets de scène
	std::list< SceneObject2D* >	*m_Obj2DList; // Liste des objets 2d
	std::list< Light* >			*m_LightList; // Liste des lumières

	Camera	    *m_Camera;		// Pointeur sur la camera
	Skybox		*m_Skybox;
	ShadowMap   *m_ShadowMap;
	
	Color4f	m_ClearColor;	// Couleur de fond
	bool	m_UseShadowMap; // Activation du shadow mapping

};

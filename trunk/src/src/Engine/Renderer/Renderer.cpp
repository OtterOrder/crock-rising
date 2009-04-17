//===========================================================================//
// Include                                                                   //
//===========================================================================//
#define		NOMINMAX
#define		DEVCAMERA

#include	"Renderer.h"
#include	"Physics/Physicalizer.h"

#include	"Core/Inputs/InputManager.h"
#include	"Objects/SceneObject.h"
#include	"Objects/2D/Object2D.h"
#include	"Objects/Camera.h"
#include	"Objects/Skybox.h"
#include	"Objects/Light.h"

#include	"PostProcesses/PostRenderer.h"
#include	"Shadow Map/ShadowMap.h"

//===========================================================================//

#define		DEFAULT_CLEAR_COLOR		(Color4f(0.2f,0.23f,0.75f,1.f))

//===========================================================================//
// FVF par défaut                                                            //
//===========================================================================//
struct DEFAULT_VERTEX
{
	FLOAT x,y,z;
	DWORD COLOR;
};
#define DEFAULT_FVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)

Renderer::Renderer()
{
	m_pGridVB		= NULL;
	m_Camera		= NULL;
	m_Skybox		= NULL;
	m_ClearColor	= DEFAULT_CLEAR_COLOR;

	// On récupère les listes d'objets
	m_ScObjList	= &SceneObject::RefList;
	m_Obj2DList	= &Object2D::RefList;
	m_LightList	= &Light::RefList;
	m_UseShadowMap = false;
}

//===========================================================================//
// Callback appelé avant la création du device                                //
//===========================================================================//
HRESULT Renderer::BeforeCreateDevice()
{
	m_pStatsFont = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
    if( m_pStatsFont == NULL )
        return E_FAIL;

	//m_DevCamera.SetViewParams(&D3DXVECTOR3(0.f, -100.f, 0.f), &D3DXVECTOR3(0.f, 0.f, 0.f));
	return S_OK;

}

//===========================================================================//
// Callback appelé juste après création du device                            //
//===========================================================================//
HRESULT Renderer::OnCreateDevice()
{
	SceneObject::ScObjIt	scobj;
	Object2D::Obj2DIt obj2d;
	HRESULT	hr;

	if( FAILED( hr = m_pStatsFont->InitDeviceObjects( m_pd3dDevice ) ) )
        return hr;

	scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->InitObject();
		++scobj;
	}

	//-- Objets 2D
	
	obj2d = m_Obj2DList->begin();
	while( obj2d != m_Obj2DList->end() )
	{
		(*obj2d)->InitDxData();
		++obj2d;
	}

	// Lumière par défaut : lumière directionnelle
	//Light * DefaultLight=new DirectionalLight();
	//Light * DefaultLight2=new PointLight();
	//Light * DefaultLight2=new SpotLight();
	//DefaultLight2->SetLightAngle(0.8f);

	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLOR4F( Color4f(0.f,0.f,0.f,1.f) ), 1.0f, 0);

	return S_OK;
}

//===========================================================================//
// Callback appelé juste après le reset du device                            //
//===========================================================================//
HRESULT Renderer::OnResetDevice()
{
	m_pStatsFont->RestoreDeviceObjects();
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

#ifdef DEVCAMERA
	m_DevCamera.SetProjParams(D3DX_PI/4, (float)m_d3dsdBackBuffer.Width/m_d3dsdBackBuffer.Height, 2.0f, 4000.f);
#endif

	DEFAULT_VERTEX sommets[]=
	{
	{-20.f, 0.f, 20.f, 0x00000000}, 
	{20.f, 0.f, 20.f, 0x00000000},
	{-20.f, 0.f, 10.f, 0x00000000}, 
	{20.f, 0.f, 10.f, 0x00000000}, 
	{-20.f, 0.f, 0.f, 0x00000000}, 
	{20.f, 0.f, 0.f, 0x00000000}, 
	{-20.f, 0.f, -10.f, 0x00000000}, 
	{20.f, 0.f, -10.f, 0x00000000}, 
	{-20.f, 0.f, -20.f, 0x00000000}, 
	{20.f, 0.f, -20.f, 0x00000000}, 

	{20.f, 0.f, -20.f, 0x00000000}, 
	{20.f, 0.f, 20.f, 0x00000000},
	{10.f, 0.f, -20.f, 0x00000000}, 
	{10.f, 0.f, 20.f, 0x00000000}, 
	{0.0f, 0.f, -20.f, 0x00000000}, 
	{0.0f, 0.0f, 20.f, 0x00000000}, 
	{-10.f, 0.f, -20.f, 0x00000000}, 
	{-10.f, 0.f, 20.f, 0x00000000}, 
	{-20.f, 0.f, -20.f, 0x00000000}, 
	{-20.f, 0.f, 20.f, 0x00000000}, 

	{ 0.f, 0.f,  0.f, 0x00ff0000},	//axe X ... et sa serait sympa de ne pas l'effacer
	{ 5.f, 0.f,  0.f, 0x00ff0000},

	{ 0.f, 0.f,  0.f, 0x0000ff00},	//axe Y
	{ 0.f, 5.f,  0.f, 0x0000ff00},

	{ 0.f, 0.f,  0.f, 0x000000f0},	//axe Z
	{ 0.f, 0.f,  5.f, 0x000000f0},

	{ -10.0f, -10.0f, 10.0f, 0x00ff0000 }, //face 1
	{ 10.0f, -10.0f, 10.0f, 0x00ff0000 },
	{ -10.0f, 10.0f, 10.0f, 0x00ff0000 },
	{ -10.0f, 10.0f, 10.0f, 0x00ff0000 },
	{ 10.0f, -10.0f, 10.0f, 0x00ff0000 },
	{ 10.0f, 10.0f, 10.0f, 0x00ff0000 }
	
	};

	if(FAILED(m_pd3dDevice->CreateVertexBuffer(
		sizeof(sommets), //nbSommets*sizeof(DEFAULT_VERTEX) -> non automatique
		D3DUSAGE_WRITEONLY,
		DEFAULT_FVF,
		D3DPOOL_DEFAULT,
		&m_pGridVB,
		NULL))
	)
		return E_FAIL;

	void * psommets;

	if(FAILED(m_pGridVB->Lock(0, sizeof(sommets), (void **)&psommets, 0)))
		return E_FAIL;

	memcpy(psommets,sommets,sizeof(sommets));

	m_pGridVB->Unlock();

	SceneObject::ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->InitDeviceData();
		++scobj;
	}

	//-- Objets 2D
	
	Object2D::Obj2DIt obj2d = m_Obj2DList->begin();
	while( obj2d != m_Obj2DList->end() )
	{
		(*obj2d)->InitDxData();
		++obj2d;
	}

	if(m_Skybox)
		m_Skybox->Init();
//*
	//-- Post Processes
	LPDIRECT3DSURFACE9 pBackBuffer;
	m_pd3dDevice->GetRenderTarget(0 , &pBackBuffer);
	PostRenderer::GetInstance()->Create(m_pd3dDevice, (u32)GetWindowWidth(), (u32)GetWindowHeight());
	PostRenderer::GetInstance()->SetBackBuffer(pBackBuffer);
//*/
	return S_OK;
}

//===========================================================================//
// Callback appelé à chaque frame pour le rendu                              //
//===========================================================================//
HRESULT Renderer::Render()
{
	m_pd3dDevice->SetRenderTarget(0, PostRenderer::GetInstance()->GetSceneRenderSurface());

	SceneObject::ScObjIt scobj;
	Object2D::Obj2DIt obj2d, lastObj2d;
	
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLOR4F( m_ClearColor ), 1.0f, 0);

	m_pd3dDevice->SetVertexShader(NULL);
	m_pd3dDevice->SetPixelShader(NULL);


	m_pd3dDevice->BeginScene();

	D3DXMATRIX MatWorld;
	D3DXMatrixIdentity(&MatWorld);

	//-- Affichage Skybox
	if(m_Skybox)
	{

		#ifdef DEVCAMERA
		m_Skybox->SetTransform(m_DevCamera.GetViewMatrix(), m_DevCamera.GetProjMatrix(), *m_DevCamera.GetEyePt());
		#else
		m_Skybox->SetTransform(&m_Camera->GetViewMatrix(), &m_Camera->GetProjMatrix(), m_Camera->GetPosition() );
		#endif
		m_Skybox->Draw();
	}

	//-- Affichage des objets 3D
	
	scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		#ifdef DEVCAMERA
		(*scobj)->SetTransform(m_DevCamera.GetViewMatrix(), m_DevCamera.GetProjMatrix(), *m_DevCamera.GetEyePt());
		#else
		(*scobj)->SetTransform(&m_Camera->GetViewMatrix(), &m_Camera->GetProjMatrix(), m_Camera->GetPosition() );
		#endif
		(*scobj)->Draw();
		++scobj;
	}

	//-- Affichage des objets 2D

	obj2d		= m_Obj2DList->begin();
	lastObj2d	= m_Obj2DList->end();

	// Tri par priorité croissante.. /!\ A optimiser si besoin
	m_Obj2DList->sort( Object2D::ComparePriority );

	// Filtre texture pour la 2D
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );

	while( obj2d != lastObj2d )
	{
		if( !(*obj2d)->IsHidden() )
		{
			(*obj2d)->Draw();
		}
		++obj2d;
	}

	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
	
	//-- ?
	
	// Affichage grille (Pipe line par défaut)
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &MatWorld);
	//m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_Camera->GetViewMatrix());
	//m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_Camera->GetProjMatrix() );

	#ifdef DEVCAMERA
	m_pd3dDevice->SetTransform(D3DTS_VIEW, m_DevCamera.GetViewMatrix());
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, m_DevCamera.GetProjMatrix() );
	#else
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_Camera->GetViewMatrix());
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_Camera->GetProjMatrix() );
	#endif

	/*m_pd3dDevice->SetFVF(DEFAULT_FVF);

	m_pd3dDevice->SetStreamSource(0, m_pGridVB, 0, sizeof(DEFAULT_VERTEX));

	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 13);*/
	
	//Affichage information frames
	m_pStatsFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
	
	m_pd3dDevice->EndScene();

	return S_OK;
}

//===========================================================================//
// Callback appelé à chaque fois que l'on perd le device                     //
//===========================================================================//
HRESULT Renderer::OnLostDevice()
{
	SceneObject::ScObjIt scobj;
	Object2D::Obj2DIt obj2d;
	
	m_pStatsFont->InvalidateDeviceObjects();
	m_pGridVB->Release();

	if(m_Skybox)
	{
		m_Skybox->LostDevice();
	}

	//-- Objets 3D
	
	scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->DeleteDeviceData();
		++scobj;
	}

	//-- Objets 2D

	obj2d = m_Obj2DList->begin();
	while( obj2d != m_Obj2DList->end() )
	{
		(*obj2d)->ClearDxData();
		++obj2d;
	}

	PostRenderer::GetInstance()->Release();

	return S_OK;
}

//===========================================================================//
// Callback appelé à chaque fois que l'on détruit le device                  //
//===========================================================================//
HRESULT Renderer::OnDestroyDevice()
{
	SceneObject::ScObjIt scobj;
	Object2D::Obj2DIt obj2d;
	
	m_pStatsFont->DeleteDeviceObjects();

	//-- Objets 3D
	
	scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->DeleteData();
		++scobj;
	}

	//-- Objets 2D
	
	obj2d = m_Obj2DList->begin();
	while( obj2d != m_Obj2DList->end() )
	{
		(*obj2d)->ClearDxData();
		++obj2d;
	}

	if(m_UseShadowMap)
		ShadowMap::GetInstance()->Release();

	PostRenderer::GetInstance()->Destroy();

	return S_OK;
}

//===========================================================================//
// Callback appelé après avoir détruit le device                             //
//===========================================================================//
HRESULT Renderer::AfterDestroyDevice()
{
	// Suppression liste d'objets 3d
	SceneObject::ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		delete (*scobj);
		++scobj;
	}

	m_ScObjList->clear();

	// Suppression liste de lumières
	Light::LightIt it=m_LightList->begin();
	while(it != m_LightList->end())
	{
		delete (*it);
		++it;
	}

	m_LightList->clear();

	return S_OK;
}


void Renderer::SetCamera( Camera* cam )
{
	m_Camera = cam;
}

void Renderer::SetSkybox(Skybox *skybox)
{
	m_Skybox = skybox;

}

void Renderer::SetShadowMap(Light *ShadowLight)
{
	if(!ShadowLight)
		return;

	m_UseShadowMap=true;
	ShadowMap::GetInstance()->SetShadowMap(ShadowLight);
}

//**********************************************************
// Largeur de la fenêtre en pixels.
//**********************************************************
int Renderer::GetWindowWidth() const
{
	return (int)m_d3dsdBackBuffer.Width;
}

//**********************************************************
// Hauteur de la fenêtre en pixels..
//**********************************************************
int Renderer::GetWindowHeight() const
{
	return (int)m_d3dsdBackBuffer.Height;
}

//**********************************************************
// Change la couleur de fond.
//**********************************************************
void Renderer::SetClearColor( const Color4f &color )
{
	m_ClearColor = color;
}

//**********************************************************
// Donne la couleur de fond.
//**********************************************************
Color4f Renderer::GetClearColor() const
{
	return m_ClearColor;
}

//**********************************************************
//
//**********************************************************
D3DXMATRIX Renderer::GetViewProjMatrix ()
{
	D3DXMATRIX ViewProjMatrix;

	D3DXMATRIX* View;
	D3DXMATRIX* Proj;
	#ifdef DEVCAMERA
	View = (D3DXMATRIX*)m_DevCamera.GetViewMatrix();
	Proj = (D3DXMATRIX*)m_DevCamera.GetProjMatrix();
	#else
	View = &(m_Camera->GetViewMatrix());
	Proj = &(m_Camera->GetProjMatrix());
	#endif

	D3DXMatrixMultiply(&ViewProjMatrix, View, Proj);

	return ViewProjMatrix;
}

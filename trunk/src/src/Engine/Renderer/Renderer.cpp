//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Renderer.h"

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
	m_pGridVB = NULL;

	// On récupère la liste des objets de scène
	m_ScObjList = &SceneObject::RefList;
}

//===========================================================================//
// Callback appelé avant la création du device                                //
//===========================================================================//
HRESULT Renderer::BeforeCreateDevice()
{
	m_pStatsFont = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
    if( m_pStatsFont == NULL )
        return E_FAIL;

	return S_OK;

}

//===========================================================================//
// Callback appelé juste après création du device                            //
//===========================================================================//
HRESULT Renderer::OnCreateDevice()
{
	ScObjIt	scobj;
	HRESULT	hr;

	if( FAILED( hr = m_pStatsFont->InitDeviceObjects( m_pd3dDevice ) ) )
        return hr;

	scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->InitObject();
		++scobj;
	}
	
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
	{ 0.f, 0.f,  5.f, 0x000000f0}
	
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

	ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->InitDeviceData();
		++scobj;
	}
	
	return S_OK;
}

//===========================================================================//
// Callback animation						                                 //
//===========================================================================//
HRESULT Renderer::FrameMove(float fElapsedTime)
{
	ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->FrameMove(fElapsedTime);
		++scobj;
	}
	return S_OK;

}

//===========================================================================//
// Callback appelé à chaque frame pour le rendu                              //
//===========================================================================//
HRESULT Renderer::Render()
{
    m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);

	m_pd3dDevice->SetVertexShader(NULL);
	m_pd3dDevice->SetPixelShader(NULL);

	D3DXMATRIX MatWorld;
	D3DXMatrixIdentity(&MatWorld);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &MatWorld);

	ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->SetTransform(&MatWorld, &m_Camera->GetViewMatrix(), &m_Camera->GetProjMatrix());
		(*scobj)->Draw();
		++scobj;
	}
	
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_Camera->GetViewMatrix());
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_Camera->GetProjMatrix() );

	m_pd3dDevice->BeginScene();

		m_pd3dDevice->SetFVF(DEFAULT_FVF);

		m_pd3dDevice->SetStreamSource(0, m_pGridVB, 0, sizeof(DEFAULT_VERTEX));

		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 13);
		
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
	m_pStatsFont->InvalidateDeviceObjects();
	m_pGridVB->Release();

	ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->DeleteDeviceData();
		++scobj;
	}
	return S_OK;
}

//===========================================================================//
// Callback appelé à chaque fois que l'on détruit le device                  //
//===========================================================================//
HRESULT Renderer::OnDestroyDevice()
{
	ScObjIt scobj;
	
	m_pStatsFont->DeleteDeviceObjects();
	
	scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		(*scobj)->DeleteData();
		++scobj;
	}
	return S_OK;
}

//===========================================================================//
// Callback appelé après avoir détruit le device                             //
//===========================================================================//
HRESULT Renderer::AfterDestroyDevice()
{
	ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		// 9a c'est super foireux, normalement on doit
		// faire ça dans le destructeur du Level !
		//delete (*scobj);
		++scobj;
	}
	return S_OK;
}


void Renderer::SetCamera( Camera* cam )
{
	m_Camera = cam;
}

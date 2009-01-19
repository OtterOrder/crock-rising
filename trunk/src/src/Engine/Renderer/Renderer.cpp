//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Renderer.h"

//===========================================================================//
// FVF par d�faut                                                            //
//===========================================================================//
struct DEFAULT_VERTEX
{
	FLOAT x,y,z;
	DWORD COLOR;
};
#define DEFAULT_FVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)

Renderer::Renderer()
{
	m_pGridVB=NULL;
	for(int i=0;i < (int)m_ListObj.size(); i++)
	{
		m_ListObj[i]=NULL;
	}
	
}

//===========================================================================//
// Callback appel� avant la cr�ation du device                                //
//===========================================================================//
HRESULT Renderer::BeforeCreateDevice()
{
	
	freopen_s( &log, "stdout.txt", "w", stdout );

	m_pStatsFont = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
    if( m_pStatsFont == NULL )
        return E_FAIL;

	return S_OK;

}

//===========================================================================//
// Callback appel� juste apr�s cr�ation du device                            //
//===========================================================================//
HRESULT Renderer::OnCreateDevice()
{
	HRESULT hr;

	if( FAILED( hr = m_pStatsFont->InitDeviceObjects( m_pd3dDevice ) ) )
        return hr;

	m_Camera.SetViewParams(&D3DXVECTOR3(0.f, 50.f, -100.f), &D3DXVECTOR3(0.f, 0.f, 0.f));

	for(int i=0;i < (int)m_ListObj.size(); i++)
	{
		m_ListObj[i]->InitObject();
	}
	
	return S_OK;

}

//===========================================================================//
// Callback appel� juste apr�s le reset du device                            //
//===========================================================================//
HRESULT Renderer::OnResetDevice()
{
	m_pStatsFont->RestoreDeviceObjects();
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	m_Camera.SetProjParams(D3DX_PI/4, (float)m_d3dsdBackBuffer.Width/m_d3dsdBackBuffer.Height, 2.0f, 4000.f);

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
	};

	if(FAILED(m_pd3dDevice->CreateVertexBuffer(
		20*sizeof(DEFAULT_VERTEX),
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

	for(int i=0;i < (int)m_ListObj.size(); i++)
	{
		m_ListObj[i]->InitDeviceData();
	}
	
	return S_OK;
}

//===========================================================================//
// Callback animation						                                 //
//===========================================================================//
HRESULT Renderer::FrameMove(float fElapsedTime)
{
	for(int i=0;i < (int)m_ListObj.size(); i++)
	{
		m_ListObj[i]->FrameMove(fElapsedTime);
	}
	return S_OK;

}

//===========================================================================//
// Callback appel� � chaque frame pour le rendu                              //
//===========================================================================//
HRESULT Renderer::Render()
{
    m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);

	D3DXMATRIX MatWorld;
	D3DXMatrixIdentity(&MatWorld);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &MatWorld);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, m_Camera.GetViewMatrix());
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION,  m_Camera.GetProjMatrix());

	for(int i=0;i < (int)m_ListObj.size(); i++)
	{
		m_ListObj[i]->SetTransform(&MatWorld, m_Camera.GetViewMatrix(), m_Camera.GetProjMatrix(), *m_Camera.GetEyePt());
		m_ListObj[i]->Draw();
	}

	m_pd3dDevice->SetVertexShader(NULL);
	m_pd3dDevice->SetPixelShader(NULL);
	
	
	
	
	//Test
	if( InputManager::GetInstance()->IsKeyPressed( 'Z' ) )
	{
		Vector3f pos = m_Camera->GetPosition();
		Vector3f target	 = m_Camera->GetTarget();
		Vector3f vectDir = target - pos;
		
		float valAbsVectDir = sqrt( vectDir.x*vectDir.x + vectDir.y*vectDir.y + vectDir.z*vectDir.z );
		float angleY = m_Camera->GetOrientationYRad();
		
		float c = cos( angleY );
		pos.z =  pos.z + fabs(c);		//fixme : test

		if( valAbsVectDir > 170.0f )
			m_Camera->SetPosition( pos );
	}

	if( InputManager::GetInstance()->IsKeyPressed( 'S' ) )
	{
		Vector3f pos = m_Camera->GetPosition();
		Vector3f target	 = m_Camera->GetTarget();
		Vector3f vectDir = target - pos;
		
		float valAbsVectDir = sqrt( vectDir.x*vectDir.x + vectDir.y*vectDir.y + vectDir.z*vectDir.z );
		float angleY = m_Camera->GetOrientationYRad();
		
		float c = cos( angleY );
		pos.z =  pos.z - fabs(c);		

		if( valAbsVectDir < 500.0f )
			m_Camera->SetPosition( pos );
	}

	//Init pos
	if( InputManager::GetInstance()->IsKeyPressed( 'A' ) )
	{
		m_Camera->SetPosition( Vector3f( 0.0f, 50.0f, -170.0f )  );
	}

	//orientation
	Point2f point = InputManager::GetInstance()->GetMouseOffset();
	const int sensibilite = 15;
	int offsetCursor;

	if( point.x != 0 ) 
	{
		offsetCursor = (int)point.x%sensibilite; 
		m_Camera->SetOrientationY( -offsetCursor );
	}
	if( point.y != 0 ) 
	{
		offsetCursor = (int)point.y%sensibilite; 
		m_Camera->SetOrientationX( offsetCursor );
	}

	m_Camera->UpdateMatrixView();
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_Camera->GetMatrixView());

	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_Camera->GetFillMatrixProjection() );

	m_pd3dDevice->BeginScene();

		m_pd3dDevice->SetFVF(DEFAULT_FVF);

		m_pd3dDevice->SetStreamSource(0, m_pGridVB, 0, sizeof(DEFAULT_VERTEX));

		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 10);
		
		//Affichage information frames
		m_pStatsFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
	
	m_pd3dDevice->EndScene();


	return S_OK;
}

//===========================================================================//
// Callback appel� � chaque fois que l'on perd le device                     //
//===========================================================================//
HRESULT Renderer::OnLostDevice()
{
	m_pStatsFont->InvalidateDeviceObjects();
	m_pGridVB->Release();

	for(int i=0;i < (int)m_ListObj.size(); i++)
	{
		m_ListObj[i]->DeleteDeviceData();
	}

	return S_OK;
}

//===========================================================================//
// Callback appel� � chaque fois que l'on d�truit le device                  //
//===========================================================================//
HRESULT Renderer::OnDestroyDevice()
{
	m_pStatsFont->DeleteDeviceObjects();

	for(int i=0;i < (int)m_ListObj.size(); i++)
	{
		m_ListObj[i]->DeleteData();
	}


	return S_OK;
}

//===========================================================================//
// Callback appel� apr�s avoir d�truit le device                             //
//===========================================================================//
HRESULT Renderer::AfterDestroyDevice()
{
	for(int i=0;i < (int)m_ListObj.size(); i++)
	{
		delete m_ListObj[i];
	}

	return S_OK;
}

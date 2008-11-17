#include "Renderer.h"
#include "d3dfont.h"


////////////////////////////////////////
//VARIABLES GLOBALES TEMPORAIRES (tests)
////////////////////////////////////////

struct DEFAULT_VERTEX
{
	FLOAT x,y,z;
	DWORD COLOR;
};

#define DEFAULT_FVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)



LPDIRECT3DVERTEXBUFFER9 g_pVB=NULL;

////////////////////////////////////////////
//FIN VARIABLES GLOBALES
///////////////////////////////////////////

HRESULT Renderer::BeforeCreateDevice()
{
	m_pStatsFont = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
    if( m_pStatsFont == NULL )
        return E_FAIL;

	return S_OK;


}

HRESULT Renderer::OnCreateDevice()
{

	HRESULT hr;

	if( FAILED( hr = m_pStatsFont->InitDeviceObjects( m_pd3dDevice ) ) )
        return hr;


	return S_OK;


}

HRESULT Renderer::OnResetDevice()
{

	m_pStatsFont->RestoreDeviceObjects();

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	DEFAULT_VERTEX sommets[]=
	{
	{-1.0f, 0.0f, 1.0f, 0xffff0000}, 
	{1.0f, 0.0f, 1.0f, 0xffff0000},
	{-1.0f, 0.0f, 0.0f, 0xffff0000}, 
	{1.0f, 0.0f, 0.0f, 0xffff0000}, 
	{-1.0f, 0.0f, -1.0f, 0xffff0000}, 
	{1.0f, 0.0f, -1.0f, 0xffff0000}, 

	{-1.0f, 0.0f, 1.0f, 0xffff0000},
	{-1.0f, 0.0f, -1.0f, 0xffff0000}, 
	{1.0f, 0.0f, 1.0f, 0xffff0000},
	{1.0f, 0.0f, -1.0f, 0xffff0000}, 
	{0.0f, 0.0f, 1.0f, 0xffff0000}, 
	{0.0f, 0.0f, -1.0f, 0xffff0000} 
	
	
	};

	if(FAILED(m_pd3dDevice->CreateVertexBuffer(
		12*sizeof(DEFAULT_VERTEX),
		NULL,
		DEFAULT_FVF,
		D3DPOOL_DEFAULT,
		&g_pVB,
		NULL))
	)
		return E_FAIL;

	void * psommets;

	if(FAILED(g_pVB->Lock(0, sizeof(sommets), (void **)&psommets, 0)))
		return E_FAIL;

	memcpy(psommets,sommets,sizeof(sommets));

	g_pVB->Unlock();

	return S_OK;

}


HRESULT Renderer::Render()
{
	
    m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);

	
	D3DXMATRIX matrixTemp;
	D3DXMatrixIdentity(&matrixTemp);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matrixTemp);

	m_Camera->SetPosition( Vector3f(0.0f, 2.0f, -2.0f) );
	D3DXMatrixLookAtLH( &matrixTemp, &m_Camera->GetPosition(), 
					    &m_Camera->GetTarget(), &m_Camera->GetUp() );
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matrixTemp);

	D3DXMatrixPerspectiveFovLH(&matrixTemp, m_Camera->GetFOV(), m_Camera->GetRatio(), 
											m_Camera->GetZNear(),m_Camera->GetZFar() );
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matrixTemp);

	m_pd3dDevice->BeginScene();

		m_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(DEFAULT_VERTEX));

		m_pd3dDevice->SetFVF(DEFAULT_FVF);

		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 6); 
		
		m_pStatsFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );

	m_pd3dDevice->EndScene();

	


	return S_OK;


}

HRESULT Renderer::OnLostDevice()
{

	m_pStatsFont->InvalidateDeviceObjects();
	g_pVB->Release();
	return S_OK;


}


HRESULT Renderer::OnDestroyDevice()
{

	m_pStatsFont->DeleteDeviceObjects();
	return S_OK;


}

void Renderer::SetCamera( Camera* cam )
{
	//Version de test
	m_Camera = cam; //à verifier; à ne surtout pas détruire pour l'instant
}
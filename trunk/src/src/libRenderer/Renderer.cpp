#include "Renderer.h"


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

HRESULT Renderer::OnCreateDevice()
{

	


	return S_OK;


}

HRESULT Renderer::OnResetDevice()
{
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
	
    m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	D3DXMATRIX maMatriceDuMonde;
	D3DXMatrixIdentity(&maMatriceDuMonde);

	D3DXMATRIX maMatriceDeVue;
	D3DXVECTOR3 vEye(0.0f, 2.0f, -2.0f);
	D3DXVECTOR3 vLookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&maMatriceDeVue, &vEye, &vLookAt, &vUp);

	D3DXMATRIX maMatriceDeProjection;
	D3DXMatrixPerspectiveFovLH(&maMatriceDeProjection, D3DX_PI/4.0f, 800.0f/600.0f, 0.005f, 100.0f);
	
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &maMatriceDuMonde);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &maMatriceDeVue);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &maMatriceDeProjection);

	m_pd3dDevice->BeginScene();

		m_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(DEFAULT_VERTEX));

		m_pd3dDevice->SetFVF(DEFAULT_FVF);

		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 6); 


	m_pd3dDevice->EndScene();



	return S_OK;


}

HRESULT Renderer::OnLostDevice()
{

	
	g_pVB->Release();
	return S_OK;


}


HRESULT Renderer::OnDestroyDevice()
{

	return S_OK;


}

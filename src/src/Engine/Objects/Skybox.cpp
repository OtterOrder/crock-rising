
#include "Skybox.h"
#include "Renderer/Renderer.h"

#pragma comment( lib, "dxguid.lib" )


struct SKYBOX_VERTEX
{
	FLOAT x,y,z;
};
#define SKYBOX_FVF (D3DFVF_XYZ)

Skybox::Skybox(const std::string& Name)
{
	m_StrTex=Name;
	m_pSkyVB=NULL;
}

Skybox::Skybox()
{
	m_StrTex="default.dds";
}

Skybox::~Skybox()
{
	delete m_pSkyShader;

}

HRESULT Skybox::Init()
{
	m_pDevice=Renderer::GetInstance()->m_pd3dDevice;
	SKYBOX_VERTEX sommets[24]=
	{
		// Front quad, NOTE: All quads face inward
		{-10.0f, -10.0f,  10.0f},
		{-10.0f,  10.0f,  10.0f},
		{ 10.0f, -10.0f,  10.0f},
		{ 10.0f,  10.0f,  10.0f},
		
		// Back quad
		{ 10.0f, -10.0f, -10.0f},
		{ 10.0f,  10.0f, -10.0f},
		{-10.0f, -10.0f, -10.0f},
		{-10.0f,  10.0f, -10.0f},
		
		// Left quad
		{-10.0f, -10.0f, -10.0f},
		{-10.0f,  10.0f, -10.0f},
		{-10.0f, -10.0f,  10.0f},
		{-10.0f,  10.0f,  10.0f},
		
		// Right quad
		{ 10.0f, -10.0f,  10.0f},
		{ 10.0f,  10.0f,  10.0f},
		{ 10.0f, -10.0f, -10.0f},
		{ 10.0f,  10.0f, -10.0f},

		// Top quad
		{-10.0f,  10.0f,  10.0f},
		{-10.0f,  10.0f, -10.0f},
		{ 10.0f,  10.0f,  10.0f},
		{ 10.0f,  10.0f, -10.0f},
		
		// Bottom quad
		{-10.0f, -10.0f, -10.0f},
		{-10.0f, -10.0f,  10.0f},
		{ 10.0f, -10.0f, -10.0f},
		{ 10.0f, -10.0f,  10.0f}


	};

	if(FAILED(m_pDevice->CreateVertexBuffer(
		sizeof(sommets),
		D3DUSAGE_WRITEONLY,
		SKYBOX_FVF,
		D3DPOOL_DEFAULT,
		&m_pSkyVB,
		NULL))
	)
		return E_FAIL;

	void * psommets;

	if(FAILED(m_pSkyVB->Lock(0, sizeof(sommets), (void **)&psommets, 0)))
		return E_FAIL;

	memcpy(psommets,sommets,sizeof(sommets));

	m_pSkyVB->Unlock();

	// Shader Skybox
	m_pSkyShader=new Shader();
	m_pSkyShader->Load("skybox.fx", NULL);

	// Texture Skybox 
	std::string str="../../data/texture/"+m_StrTex;

	if(FAILED((D3DXCreateCubeTextureFromFile( m_pDevice, str.c_str(), &m_pEnvTex ))))
	{
		return E_FAIL;

	}

	return S_OK;

}

void Skybox::SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos)
{
	D3DXMATRIX MatWorldView;
	D3DXMATRIX mWorldViewProjection;

	D3DXMATRIXA16 mView = *view;
    mView._41 =  0.0f;
	mView._42 = -0.4f; 
	mView._43 =  0.0f;

    D3DXMatrixScaling( &m_WorldMatrix, 100.0f, 100.0f, 100.0f );
    D3DXMatrixMultiply( &MatWorldView, &m_WorldMatrix, &mView );
	D3DXMatrixMultiply(&mWorldViewProjection, &MatWorldView, proj);

	m_pSkyShader->m_pEffect->SetMatrix( "g_mWorld", &m_WorldMatrix);
	m_pSkyShader->m_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection);
	m_pSkyShader->m_pEffect->SetValue("g_vCamPos", CamPos, sizeof(D3DXVECTOR3));

}


HRESULT Skybox::Draw()
{
	m_pSkyShader->m_pEffect->SetTechnique( "RenderSkybox" );

	m_pSkyShader->m_pEffect->Begin(0, 0);

	m_pSkyShader->m_pEffect->BeginPass(0);

	if (m_pSkyVB)
	{
		for ( ULONG i = 0; i < 6; ++i )
		{
			m_pDevice->SetFVF(SKYBOX_FVF);
			
			m_pDevice->SetStreamSource(0, m_pSkyVB, 0, sizeof(SKYBOX_VERTEX));

			m_pSkyShader->m_pEffect->SetTexture("g_txEnvMap", m_pEnvTex);

			m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, i * 4, 2 );
		}
	}

	m_pSkyShader->m_pEffect->EndPass();

	m_pSkyShader->m_pEffect->End();

	return S_OK;

}

HRESULT Skybox::LostDevice()
{
	m_pSkyVB->Release();
	m_pSkyShader->m_pEffect->Release();
	m_pEnvTex->Release();

	return S_OK;

}
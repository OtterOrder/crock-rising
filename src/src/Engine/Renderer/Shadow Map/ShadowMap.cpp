
//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "ShadowMap.h"
#include <Objects/SceneObject.h>


ShadowMap::ShadowMap()
{

}

ShadowMap::~ShadowMap()
{
	delete m_pShadowShader;

}

void ShadowMap::ResetDevice()
{
	m_pShadowShader->m_pEffect->OnResetDevice();

}

void ShadowMap::LostDevice()
{
	m_pShadowShader->m_pEffect->OnLostDevice();

}

void ShadowMap::InitShadowMap()
{

	m_ScObjList	= &SceneObject::RefList;

	m_pDevice=Renderer::GetInstance()->m_pd3dDevice;

	m_pDevice->CreateTexture( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pShadowMap, NULL );
	m_pDevice->CreateDepthStencilSurface( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepth, NULL );
	//Matrice de la lumière
	D3DXMATRIX mView;
	D3DXVECTOR3 vUp=D3DXVECTOR3(0.0f, 1.0f, 0.0f );




	//D3DXMatrixLookAtLH( &mView, &m_ShadowLight->GetLightPosition(), &m_ShadowLight->GetLightDirection(), &vUp );

	D3DXMatrixLookAtLH( &mView, &D3DXVECTOR3( -20.0f, 20.0f, -20.0f ), &D3DXVECTOR3(  20.0f,  0.0f,  20.0f ), &D3DXVECTOR3(   0.0f,  1.0f,   0.0f ) );

	D3DXMATRIX mProj;
	//Matrice de projection
	D3DXMatrixPerspectiveFovLH( &mProj, D3DXToRadian(30.0f), 1.0f, 1.0f, 1024.0f );

	m_mLightViewProj = mView * mProj;

	//Shader pour le rendu de la z-map
	m_pShadowShader=new Shader();
	m_pShadowShader->Load("depth.fx", NULL);

	m_pShadowMap->GetSurfaceLevel( 0, &m_pShadowSurf );


}

void ShadowMap::SetShadowMap(Light * ShadowLight)
{
	m_ShadowLight=ShadowLight;

	InitShadowMap();

}

void ShadowMap::Render()
{
	// On enregistre l'ancienne RT
	m_pDevice->GetRenderTarget( 0, &m_pOldColorRT );
	m_pDevice->GetDepthStencilSurface( &m_pOldDepthRT );

	// Render Target shadow map
	m_pDevice->SetRenderTarget( 0, m_pShadowSurf );
	m_pDevice->SetDepthStencilSurface( m_pShadowDepth );

	m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0 );

	m_pShadowShader->m_pEffect->SetTechnique( "RenderShadow" );

	m_pShadowShader->m_pEffect->Begin(0, 0);

	m_pShadowShader->m_pEffect->BeginPass(0);

	SceneObject::ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		m_pShadowShader->m_pEffect->SetMatrix( "g_mWorldView", &m_mLightViewProj);
		(*scobj)->DrawShadow();
		++scobj;
	}

	m_pShadowShader->m_pEffect->EndPass();

	m_pShadowShader->m_pEffect->End();

	m_pDevice->SetRenderTarget( 0, m_pOldColorRT );
	m_pDevice->SetDepthStencilSurface( m_pOldDepthRT );
	SAFE_RELEASE( m_pOldColorRT );
	SAFE_RELEASE( m_pOldDepthRT );

	D3DXSaveTextureToFile("test.jpg", D3DXIFF_JPG, m_pShadowMap, NULL);


}

void ShadowMap::Release()
{
	m_pShadowMap->Release();
	m_pShadowSurf->Release();
	m_pShadowDepth->Release();
	//m_pOldColorRT->Release();
	//m_pOldDepthRT->Release();

}

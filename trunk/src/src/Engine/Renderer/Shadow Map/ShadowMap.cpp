
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
	m_pDevice->CreateTexture( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowMap, NULL );
	m_pDevice->CreateDepthStencilSurface( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepth, NULL );
	m_pShadowShader->GetEffect()->OnResetDevice();
	m_pShadowMap->GetSurfaceLevel( 0, &m_pShadowSurf );
}

void ShadowMap::LostDevice()
{
	m_pShadowShader->GetEffect()->OnLostDevice();
	m_pShadowMap->Release();
	m_pShadowSurf->Release();
	m_pShadowDepth->Release();

}

void ShadowMap::InitShadowMap()
{

	m_ScObjList	= &SceneObject::RefList;

	m_pDevice=Renderer::GetInstance()->m_pd3dDevice;

	D3DXVECTOR3 vUp=D3DXVECTOR3(0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &m_mLightView, &m_ShadowLight->GetLightPosition(), &m_ShadowLight->GetLightDirection(), &vUp );
	//Matrice de projection
	D3DXMatrixPerspectiveFovLH( &m_mLightProj, D3DXToRadian(60.0f), 1.f, 1.f, 1024.0f );

	//Shader pour le rendu de la z-map
	m_pShadowShader=new Shader();
	m_pShadowShader->Load("depth.fx", NULL);

	// Generate the texture matrix
	float fTexOffs = 0.5 + (0.5 / (float)SHADOW_MAP_SIZE);
	m_mTexProj=D3DXMATRIX( 0.5f,     0.0f,     0.0f, 0.0f,
		0.0f,     -0.5f,    0.0f, 0.0f,
		0.0f,     0.0f,     1.0f, 0.0f,
		fTexOffs, fTexOffs, 0.0f, 1.0f );


	ResetDevice();
	

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

	//// Render Target shadow map
	m_pDevice->SetRenderTarget( 0, m_pShadowSurf );
	m_pDevice->SetDepthStencilSurface( m_pShadowDepth );

	m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, 1.f), 1.0f, 0 );

	SceneObject::ScObjIt scobj = m_ScObjList->begin();
	while( scobj != m_ScObjList->end() )
	{
		m_mLightViewProj = m_mLightView * m_mLightProj;
		m_pShadowShader->GetEffect()->SetMatrix( "g_mViewProj", &m_mLightViewProj);
		(*scobj)->DrawShadow();
		++scobj;
	}

	m_pDevice->SetRenderTarget( 0, m_pOldColorRT );
	m_pDevice->SetDepthStencilSurface( m_pOldDepthRT );
	SAFE_RELEASE( m_pOldColorRT );
	SAFE_RELEASE( m_pOldDepthRT );

	//D3DXSaveTextureToFile("test.jpg", D3DXIFF_JPG, m_pShadowMap, NULL);


}

void ShadowMap::Release()
{

}

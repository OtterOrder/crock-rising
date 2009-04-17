
//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "ShadowMap.h"


ShadowMap::ShadowMap()
{

}

ShadowMap::~ShadowMap()
{

}

void ShadowMap::InitShadowMap()
{
	m_pDevice=Renderer::GetInstance()->m_pd3dDevice;

	m_pDevice->CreateTexture( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowMap, NULL );

	//Matrice de la lumière
	D3DXMATRIX matView;
	D3DXVECTOR3 vUp=D3DXVECTOR3(0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &m_ShadowLight->GetLightPosition(), &m_ShadowLight->GetLightDirection(), &vUp );

}

void ShadowMap::SetShadowMap(Light * ShadowLight)
{
	m_ShadowLight=ShadowLight;

	InitShadowMap();

}

void ShadowMap::Release()
{
	m_pShadowMap->Release();

}

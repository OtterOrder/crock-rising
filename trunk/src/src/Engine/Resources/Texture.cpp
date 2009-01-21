//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Texture.h"
#include "../Renderer/Renderer.h"

//===========================================================================//
// Constructeur Texture                                                      //
//===========================================================================//
Texture::Texture()
{
	m_pTex = NULL; // Init du pointeur vers la tex
}

Texture::~Texture()
{
	m_pTex->Release();

}


HRESULT Texture::Load( const char* sPath)
{			

	LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;

	if( FAILED(D3DXCreateTextureFromFileEx(pDevice,
											sPath,
											D3DX_DEFAULT,
											D3DX_DEFAULT,
											D3DX_DEFAULT,
											0,
											D3DFMT_A8R8G8B8,
											D3DPOOL_MANAGED,
											D3DX_DEFAULT,
											D3DX_DEFAULT,
											0,
											NULL,
											NULL,
											&m_pTex)) )   //Pointeur sur la tex

		return S_FALSE;

	return S_OK;
}
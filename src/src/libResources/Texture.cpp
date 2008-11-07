#include "Texture.h"

//TODO
//******************************************************************
Texture::Texture()
{
	m_pDevice = NULL;
	m_pTex = NULL;				//Init du pointeur vers la tex
}

Texture::Texture(TCHAR strTexName, LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = NULL;
	m_pTex = NULL;				//Init du pointeur vers la tex
	
	SetDevice(pDevice);
	if( FAILED(D3DXCreateTextureFromFile(m_pDevice, (LPCSTR)strTexName, &m_pTex)) )
		m_pTex = NULL;
}

Texture::~Texture()
{
}


ResourceResult Texture::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{ 
	if (pDevice != NULL)
		return RES_FAILED;

	m_pDevice = pDevice;
	return RES_SUCCEED;
}
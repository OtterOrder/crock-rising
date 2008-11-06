#include "Texture.h"

//TODO
//******************************************************************
Texture::Texture()
{
	m_pDevice = NULL;
}

Texture::~Texture()
{
}

TextureResult Texture::Load( crc32 resource, TCHAR strTexName)
{
	LPDIRECT3DTEXTURE9 pTex;

	pTex = NULL;				//Init du pointeur vers la tex

	if( FAILED(D3DXCreateTextureFromFile(m_pDevice, (LPCSTR)strTexName, &pTex)) )
		return TEX_FAILED ;

	return TEX_SUCCEED;
}

TextureResult Texture::Release( crc32 resource )
{
	return TEX_FAILED;
}

TextureResult Texture::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{ 
	if (pDevice != NULL)
		return TEX_FAILED;

	m_pDevice = pDevice;
	return TEX_SUCCEED;
}
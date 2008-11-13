#include "Texture.h"

//TODO
//******************************************************************
Texture::Texture()
{
	m_pTex = NULL;				//Init du pointeur vers la tex
}

Texture::~Texture()
{
}

ResourceResult Texture::Initialize(crc32 resource)
{			
	m_pDevice = NULL;//Renderer::GetDevice();

	char *strTexName ;
	sprintf(strTexName, "..\\data\\textures\\%u", resource);

	if( FAILED(D3DXCreateTextureFromFileEx(m_pDevice,
											(LPCSTR)strTexName, 
											D3DX_DEFAULT, //Largeur
											D3DX_DEFAULT, //Hauteur
											D3DX_DEFAULT, //MipLevel, à voir si changement
											0,			  //Usage (render target, 
											D3DFMT_R8G8B8,//A?
											D3DPOOL_DEFAULT,
											D3DX_DEFAULT, //Filter
											D3DX_DEFAULT, //MipFilter
											0,			  //ColorKey
											NULL,		  //pSrcInfo
											NULL,		  //pPalette
											&m_pTex)) )   //Pointeur sur la tex
		return RES_FAILED;

	return RES_SUCCEED;
}



ResourceResult Texture::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{ 
	return RES_FAILED;
}

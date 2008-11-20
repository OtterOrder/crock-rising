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

ResourceResult Texture::Initialize( crc32			resource,
									UINT			Width, 
									UINT			Height,
									UINT			MipLevels,
									DWORD			Usage,
									D3DFORMAT		Format, 
									D3DPOOL			Pool,
									DWORD			Filter,
									DWORD			MipFilter, 
									D3DCOLOR		ColorKey,
									D3DXIMAGE_INFO	*pSrcInfo,
									PALETTEENTRY		*pPalette )
{			
	m_pDevice = NULL;//Renderer::GetDevice();

	char *strTexName ;
	sprintf(strTexName, "..\\..\\data\\textures\\%u", resource);

	if( FAILED(D3DXCreateTextureFromFileEx(m_pDevice,
											strTexName,
											Width,
											Height,
											MipLevels,
											Usage,
											Format,
											Pool,
											Filter,
											MipFilter,
											ColorKey,
											pSrcInfo,
											pPalette,
											&m_pTex)) )   //Pointeur sur la tex
		return RES_FAILED;

	return RES_SUCCEED;
}



ResourceResult Texture::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{ 
	return RES_FAILED;
}

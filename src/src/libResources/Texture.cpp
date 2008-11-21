#include "Texture.h"


Texture::Texture()
{
	m_pTex = NULL;				//Init du pointeur vers la tex
}

Texture::~Texture()
{
}

/***********************************************************
 * Charge la ressource.
 * @param[in]	resource : crc32 de la ressource, tous les paramètres de chargement de textures
 * @return	le résultat du chargement
 **********************************************************/
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

	LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;

	char sMeshPath[128];
	sprintf(sMeshPath, "..\\..\\data\\textures\\%u.dds", resource);

	if( FAILED(D3DXCreateTextureFromFileEx(pDevice,
											sMeshPath,
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
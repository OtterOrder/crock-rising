#ifndef		_TextureManager_H
#define		_TextureManager_H

//******************************************************************

#include	"ResourceManager.h"
#include	"Texture.h"

//******************************************************************

class TextureManager : public ResourceManager
{
		TextureManager(){}
		~TextureManager(){}

	public:	
		
		ResourceResult	Load		( crc32			resource,
									UINT			Width		= D3DX_DEFAULT, 
									UINT			Height		= D3DX_DEFAULT,
									UINT			MipLevels	= D3DX_DEFAULT,
									DWORD			Usage		= 0,
									D3DFORMAT		Format		= D3DFMT_R8G8B8, 
									D3DPOOL			Pool		= D3DPOOL_DEFAULT,
									DWORD			Filter		= D3DX_DEFAULT,
									DWORD			MipFilter	= D3DX_DEFAULT, 
									D3DCOLOR		ColorKey	= 0,
									D3DXIMAGE_INFO	*pSrcInfo	= NULL,
									PALETTEENTRY 	*pPalette	= NULL);		// Charge la ressource
																						//le nom du fichier n'est qu'a spécifier qu'au
																						//premier chargement, après on utilise le crc32

		ResourceResult	Release		( crc32 resource );		// Supprime la ressource

};

//******************************************************************
#endif		// _TextureManager_H

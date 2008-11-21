#ifndef		_Texture_H
#define		_Texture_H

//******************************************************************

#include	"Resource.h"
#include	<libRenderer/Renderer.h>

//******************************************************************


//TODO
class Texture: public Resource
{
	public:
		
		LPDIRECT3DDEVICE9		m_pDevice;		//Device utilisé par le renderer
		LPDIRECT3DTEXTURE9		m_pTex;			//Pointeur vers la texture

		Texture(void);
		~Texture();

		ResourceResult			Initialize	( crc32			resource,
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
											PALETTEENTRY	*pPalette );
};

//******************************************************************
#endif		// _Texture_H

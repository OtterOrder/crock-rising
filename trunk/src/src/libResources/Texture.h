#ifndef		_Texture_H
#define		_Texture_H

//******************************************************************

#include	"Resource.h"

//******************************************************************


//TODO
class Texture: public Resource
{
	public:
		
		Texture(void);
		~Texture();

		ResourceResult			Initialize	(crc32			resource,
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
											PALETTEENTRY 	*pPalette	= NULL);

		ResourceResult			SetDevice	( LPDIRECT3DDEVICE9 pDevice ); //Modifie le device grace au renderer
		LPDIRECT3DTEXTURE9		GetTex		(){ return m_pTex; }

	private:

		LPDIRECT3DDEVICE9		m_pDevice;		//Device utilisé par le renderer
		LPDIRECT3DTEXTURE9		m_pTex;			//Pointeur vers la texture
};

//******************************************************************
#endif		// _Texture_H

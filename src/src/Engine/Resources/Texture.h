#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Resource.h"

//===========================================================================//
// Enums                                                                     //
//===========================================================================//

enum TextureType
{
	TEX_MESH,
	TEX_NORMALMAP,
	TEX_BUMPMAP,
	TEX_DISPLACEMENTMAP
};

//===========================================================================//
// Classe texture                                                            //
//===========================================================================//
class Texture: public Resource
{
	public:

		Texture();
		~Texture();

		ResourceResult			Load	(std::string resource);

				
		LPDIRECT3DDEVICE9		m_pDevice;		// Device utilisé par le renderer
		LPDIRECT3DTEXTURE9		m_pTex;			// Pointeur vers la texture
};



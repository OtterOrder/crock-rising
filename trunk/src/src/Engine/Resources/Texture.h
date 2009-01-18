#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Resource.h"

//===========================================================================//
// Classe texture                                                            //
//===========================================================================//
class Texture: public Resource
{
	public:

		Texture();
		~Texture();

		HRESULT			Load	(const char * sPath);

				
		LPDIRECT3DDEVICE9		m_pDevice;		// Device utilisé par le renderer
		LPDIRECT3DTEXTURE9		m_pTex;			// Pointeur vers la texture
};



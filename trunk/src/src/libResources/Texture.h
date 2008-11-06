#ifndef		_Texture_H
#define		_Texture_H

//******************************************************************

#include	"Resource.h"

//******************************************************************

enum TextureResult
{
	TEX_SUCCEED,
	TEX_FAILED

};

//TODO
class Texture
{
	public:
		
		Texture(void);
		~Texture();

		TextureResult	Load		( crc32 resource, TCHAR strTexName );		// Charge la ressource
		TextureResult	Release		( crc32 resource );		// Supprime la ressource

		TextureResult  SetDevice	( LPDIRECT3DDEVICE9 pDevice ); //Modifie le device grace au renderer
		crc32			GetCrc32	();							   //Récupère le CRC32

	private:

		LPDIRECT3DDEVICE9      m_pDevice;
		crc32				   m_Crc32;
};

//******************************************************************
#endif		// _Texture_H

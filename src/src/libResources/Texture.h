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

		ResourceResult			Initialize	(crc32 resource);
		ResourceResult			SetDevice	( LPDIRECT3DDEVICE9 pDevice ); //Modifie le device grace au renderer
		LPDIRECT3DTEXTURE9		GetTex		(){ return m_pTex; }

	private:

		LPDIRECT3DDEVICE9		m_pDevice;		//Device utilisé par le renderer
		LPDIRECT3DTEXTURE9		m_pTex;			//Pointeur vers la texture
};

//******************************************************************
#endif		// _Texture_H

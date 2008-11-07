#ifndef		_TextureManager_H
#define		_TextureManager_H

//******************************************************************

#include	"ResourceManager.h"
#include	"Texture.h"

//******************************************************************

class TextureManager : public ResourceManager
{
	public:	
		
		ResourceResult	Load		( crc32 resource, TCHAR strTexName, LPDIRECT3DDEVICE9 pDevice );		// Charge la ressource
		ResourceResult	Release		( crc32 resource );		// Supprime la ressource

	protected:
		std::list< Texture* >		m_TextureList;		// Liste des ressources chargées

};

//******************************************************************
#endif		// _TextureManager_H

#ifndef		_TextureManager_H
#define		_TextureManager_H

//******************************************************************

#include	"ResourceManager.h"
#include	"Texture.h"

//******************************************************************

class TextureManager : public ResourceManager
{
	public:	
		
		ResourceResult	Load		( crc32 resource, LPDIRECT3DDEVICE9 pDevice, TCHAR strTexName=NULL);		// Charge la ressource
																						//le nom du fichier n'est qu'a sp�cifier qu'au
																						//premier chargement, apr�s on utilise le crc32
		ResourceResult	Release		( crc32 resource );		// Supprime la ressource

	protected:
		std::list< Texture* >		m_TextureList;		// Liste des ressources charg�es

};

//******************************************************************
#endif		// _TextureManager_H

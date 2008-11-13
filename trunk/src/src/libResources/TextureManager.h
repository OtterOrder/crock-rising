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
		
		ResourceResult	Load		( crc32 resource);		// Charge la ressource
																						//le nom du fichier n'est qu'a spécifier qu'au
																						//premier chargement, après on utilise le crc32
		ResourceResult	Release		( crc32 resource );		// Supprime la ressource

};

//******************************************************************
#endif		// _TextureManager_H

#ifndef		_TextureManager_H
#define		_TextureManager_H

//******************************************************************

#include	"ResourceManager.h"

//******************************************************************

class TextureManager : public ResourceManager
{
	public:

		ResourceResult	Load		( crc32 resource );		// Charge la ressource
		ResourceResult	Release		( crc32 resource );		// Supprime la ressource

};

//******************************************************************
#endif		// _TextureManager_H

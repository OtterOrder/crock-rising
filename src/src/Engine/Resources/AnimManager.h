#ifndef		_AnimManager_H
#define		_AnimManager_H

//******************************************************************

#include	"ResourceManager.h"

//******************************************************************

class AnimManager : public ResourceManager
{
	public:

		ResourceResult	Load		( crc32 resource );		// Charge la ressource
		ResourceResult	Release		( crc32 resource );		// Supprime la ressource

};

//******************************************************************
#endif		// _AnimManager_H

#ifndef		_MeshManager_H
#define		_MeshManager_H

//******************************************************************

#include	"ResourceManager.h"

//******************************************************************

class MeshManager : public ResourceManager
{
	public:
		MeshManager (void)	{};
		~MeshManager ()		{};

		ResourceResult	Load		( crc32 Resource, LPDIRECT3DDEVICE9 pDevice);		// Charge la ressource
		ResourceResult	Release		( crc32 Resource );		// Supprime la ressource
};

//******************************************************************
#endif		// _MeshManager_H

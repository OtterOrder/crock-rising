#ifndef		_Mesh_H
#define		_Mesh_H

//******************************************************************

#include	"Resource.h"

#include	<TinyXml/tinyxml.h>

//******************************************************************

class Mesh : public Resource
{
	public:
		
		Mesh(void);
		Mesh(crc32 Resource, LPDIRECT3DDEVICE9 pDevice);
		~Mesh();

	private:

		LPDIRECT3DDEVICE9		m_pDevice;		//Device utilisé par le renderer

		LPDIRECT3DVERTEXBUFFER9	m_pVB;			// Vertex Buffer de la ressource
		LPDIRECT3DINDEXBUFFER9	m_pIB;			// Index Buffer de la ressource
};

//******************************************************************
#endif		// _Mesh_H

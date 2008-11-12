#include "Mesh.h"

//******************************************************************

Mesh::Mesh()
{
	m_pDevice = NULL;

	m_pVB = NULL;
	m_pIB = NULL;
}

Mesh::Mesh(crc32 resource, LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = NULL;
}

Mesh::~Mesh()
{
}
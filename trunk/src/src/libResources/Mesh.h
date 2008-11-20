#ifndef		_Mesh_H
#define		_Mesh_H

//******************************************************************
#include	<string>

#include	"Resource.h"

#include	<libRenderer/Renderer.h>
#include	<libCore/Types/Vector.h>

#include	"MeshLoader.h"

using namespace std;
//******************************************************************

class Mesh : public Resource
{
public:

	Mesh(void);
	~Mesh();

	ResourceResult			Load		(crc32 resource);

private:
	int		m_iNbVertices;
	int		m_iNbIndex;

	Vertex		*m_VertexBuffer;					// Tableau de vertex
	int			*m_IndexBuffer;					// Tableau d'indexation des faces

	LPDIRECT3DDEVICE9		m_pDevice;		// Device utilisé par le renderer

	LPDIRECT3DVERTEXBUFFER9	m_pVB;			// Vertex Buffer de la ressource
	LPDIRECT3DINDEXBUFFER9	m_pIB;			// Index Buffer de la ressource
};

//******************************************************************
#endif		// _Mesh_H

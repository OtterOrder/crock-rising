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

#define		Mesh_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0)

class Mesh : public Resource
{
public:

	Mesh(void);
	virtual ~Mesh();

	ResourceResult			Load		(crc32 resource);

	LPDIRECT3DVERTEXBUFFER9	m_pVB;			// Vertex Buffer de la ressource
	LPDIRECT3DINDEXBUFFER9	m_pIB;			// Index Buffer de la ressource

	int		m_iNbVertices;
	int		m_iNbIndex;

	ResourceResult			FillD3DBuffers ();
	void					ReleaseD3DBuffers();
private:

	Vertex		*m_VertexBuffer;					// Tableau de vertex
	int			*m_IndexBuffer;					// Tableau d'indexation des faces
};

//******************************************************************
#endif		// _Mesh_H

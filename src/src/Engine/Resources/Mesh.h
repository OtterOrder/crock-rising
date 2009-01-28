#ifndef		_Mesh_H
#define		_Mesh_H

//******************************************************************


#include	"Resource.h"
#include	"Core/Types/Vector.h"
#include	"MeshLoader.h"
#include	<string>

//******************************************************************

#define		Mesh_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0)

class Mesh : public Resource
{
public:

	Mesh(void);
	virtual ~Mesh();

	ResourceResult Load( std::string resource, ResourceParam param );

	LPDIRECT3DVERTEXBUFFER9	m_pVB;			// Vertex Buffer de la ressource
	LPDIRECT3DINDEXBUFFER9	m_pIB;			// Index Buffer de la ressource

	int		m_iNbVertices;
	int		m_iNbIndex;

	IDirect3DVertexDeclaration9*	m_decl;			// Vertex declaration de la ressource

	D3DXVECTOR3					m_Position;			// Position de départ
	D3DXVECTOR4					m_Rotation;			// Rotation de départ
	D3DXVECTOR3					m_Scale;			// Scale de départ

	ResourceResult			FillD3DBuffers ();
	void					ReleaseD3DBuffers();



private:

	Vertex		*m_VertexBuffer;					// Tableau de vertex
	int			*m_IndexBuffer;					// Tableau d'indexation des faces
};

//******************************************************************
#endif		// _Mesh_H

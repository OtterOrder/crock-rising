#include "Mesh.h"
#include "Renderer/Renderer.h"

#include	"Renderer/Renderer.h"

//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
Mesh::Mesh()
{
	m_iNbVertices = 0;
	m_iNbIndex	  = 0;

	m_pVB = NULL;
	m_pIB = NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
Mesh::~Mesh()
{
	ReleaseD3DBuffers ();

	if ( m_VertexBuffer )
		delete [] m_VertexBuffer;

	if ( m_IndexBuffer )
		delete [] m_IndexBuffer;

	m_decl->Release();
}


/***********************************************************
 * Supprime la ressource.
 * @param[in]	resource : crc32 de la ressource
 * @param[?]	param : ce qu'on veut..
 * @return	le résultat du chargement
 **********************************************************/
ResourceResult Mesh::Load( std::string resource, ResourceParam param )
{
	std::string str="..\\..\\data\\models\\"+resource;

	MeshLoader meshLoader;

	if ( meshLoader.Load(str.c_str(), m_VertexBuffer, m_SVertexBuffer, m_IndexBuffer, m_iNbVertices, m_iNbIndex, m_decl, m_Skinned) == RES_SUCCEED)
	{
		if ( FillD3DBuffers () == RES_SUCCEED)
			return RES_SUCCEED;
	}

	return RES_FAILED;
}

ResourceResult	Mesh::FillD3DBuffers ()
{
	ReleaseD3DBuffers ();

	if ((m_VertexBuffer || m_SVertexBuffer) && m_IndexBuffer)
	{
		LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;

		UINT BufferSize;
		if (!m_Skinned)
			BufferSize = sizeof(Vertex)*m_iNbVertices;
		else
			BufferSize = sizeof(SkinnedVertex)*m_iNbVertices;
	
		pDevice->CreateVertexBuffer (	BufferSize, 
										NULL, 
										0, 
										D3DPOOL_DEFAULT, 
										&m_pVB, 
										NULL);

		void * pVertexBuffer;

		if(FAILED(m_pVB->Lock(0, BufferSize, (void **)&pVertexBuffer, 0)))
			return RES_FAILED;

		if (!m_Skinned)
			memcpy(pVertexBuffer, m_VertexBuffer, BufferSize);
		else
			memcpy(pVertexBuffer, m_SVertexBuffer, BufferSize);

		m_pVB->Unlock();


		
		BufferSize = sizeof(int)*m_iNbIndex;

		pDevice->CreateIndexBuffer (	BufferSize,
										D3DUSAGE_WRITEONLY,
										D3DFMT_INDEX32,
										D3DPOOL_DEFAULT,
										&m_pIB,
										NULL);
		void * pIndexBuffer;

		if(FAILED(m_pIB->Lock(0, BufferSize, (void **)&pIndexBuffer, 0)))
			return RES_FAILED;

		memcpy(pIndexBuffer, m_IndexBuffer, BufferSize);

		m_pIB->Unlock();

		UINT numElements;
		m_decl->GetDeclaration( m_VBdecl, &numElements);

		return RES_SUCCEED;
	}
	return RES_FAILED;
}

void Mesh::OrderIndices(int * ArrayOrder)
{
	// On met les indices dans l'ordre hiérarchique
	if(!m_bOrdered)
	{
		for(int i=0; i<m_iNbVertices; i++)
		{
			m_SVertexBuffer[i].m_Bones.x=(float)ArrayOrder[(int)m_SVertexBuffer[i].m_Bones.x];
			m_SVertexBuffer[i].m_Bones.y=(float)ArrayOrder[(int)m_SVertexBuffer[i].m_Bones.y];
			m_SVertexBuffer[i].m_Bones.z=(float)ArrayOrder[(int)m_SVertexBuffer[i].m_Bones.z];
			m_SVertexBuffer[i].m_Bones.w=(float)ArrayOrder[(int)m_SVertexBuffer[i].m_Bones.w];
		}
		m_bOrdered=true;
	}

}

void	Mesh::ReleaseD3DBuffers()
{
	if ( m_pVB )
		m_pVB->Release(),	m_pVB = NULL;
	if ( m_pIB )
		m_pIB->Release(),	m_pIB = NULL;
}
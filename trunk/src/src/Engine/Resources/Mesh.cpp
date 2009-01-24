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
 * @return	le résultat du chargement
 **********************************************************/
ResourceResult Mesh::Load(std::string resource)
{
	std::string str="..\\..\\data\\models\\"+resource;

	MeshLoader meshLoader;

	if ( meshLoader.Load(str.c_str(), m_VertexBuffer, m_IndexBuffer, m_iNbVertices, m_iNbIndex, m_decl, m_Position, m_Rotation, m_Scale) == RES_SUCCEED)
	{
		if ( FillD3DBuffers () == RES_SUCCEED)
			return RES_SUCCEED;
	}

	return RES_FAILED;
}

ResourceResult	Mesh::FillD3DBuffers ()
{
	ReleaseD3DBuffers ();

	if (m_VertexBuffer && m_IndexBuffer)
	{
		LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;

		UINT BufferSize = sizeof(Vertex)*m_iNbVertices;

		pDevice->CreateVertexBuffer (	BufferSize, 
										NULL, 
										Mesh_FVF, 
										D3DPOOL_DEFAULT, 
										&m_pVB, 
										NULL);

		void * pVertexBuffer;

		if(FAILED(m_pVB->Lock(0, BufferSize, (void **)&pVertexBuffer, 0)))
			return RES_FAILED;

		memcpy(pVertexBuffer, m_VertexBuffer, BufferSize);

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

		return RES_SUCCEED;
	}
	return RES_FAILED;
}

void	Mesh::ReleaseD3DBuffers()
{
	if ( m_pVB )
		m_pVB->Release(),	m_pVB = NULL;
	if ( m_pIB )
		m_pIB->Release(),	m_pIB = NULL;
}
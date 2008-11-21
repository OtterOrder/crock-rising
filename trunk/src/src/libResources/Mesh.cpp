#include "Mesh.h"

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
}


/***********************************************************
 * Supprime la ressource.
 * @param[in]	resource : crc32 de la ressource
 * @return	le résultat du chargement
 **********************************************************/
ResourceResult Mesh::Load(crc32 resource)
{
	char sMeshPath[128];
	sprintf_s(sMeshPath, "..\\..\\data\\test\\%u.DAE", resource);

	MeshLoader meshLoader;

	if ( meshLoader.Load(sMeshPath, m_VertexBuffer, m_IndexBuffer, m_iNbVertices, m_iNbIndex) == RES_SUCCEED)
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
	else
		return RES_FAILED;
}
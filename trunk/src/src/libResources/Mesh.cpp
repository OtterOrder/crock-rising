#include "Mesh.h"

//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
Mesh::Mesh()
{
	m_pDevice = NULL;

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
	//m_pDevice = NULL;//Renderer::GetDevice();
	//Renderer::GetInstance()->m_pd3dDevice;

	char sMeshPath[128];
	sprintf_s(sMeshPath, "..\\..\\data\\test\\%u.DAE", resource);

	MeshLoader meshLoader;

	if ( meshLoader.Load(sMeshPath, m_VertexBuffer, m_IndexBuffer, m_iNbVertices, m_iNbIndex) == RES_SUCCEED)
	{
		m_pDevice = Renderer::GetInstance()->m_pd3dDevice;

		DWORD VB_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0;

		m_pDevice->CreateVertexBuffer (	m_iNbVertices*sizeof(Vertex), 
										NULL, VB_FVF, 
										D3DPOOL_DEFAULT, 
										&m_pVB, 
										NULL);

		void * pVertexBuffer;
		if(FAILED(m_pVB->Lock(0, sizeof(m_VertexBuffer), (void **)&pVertexBuffer, 0)))
			return RES_FAILED;

		memcpy(pVertexBuffer, m_VertexBuffer, sizeof(m_VertexBuffer));

		m_pVB->Unlock();

		return RES_SUCCEED;
	}
	else
		return RES_FAILED;
}
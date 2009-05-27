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

	m_IndexBuffer  = NULL;
	m_bOrdered = false;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
Mesh::~Mesh()
{
	Release();
	ReleaseD3DBuffers ();

	if ( m_IndexBuffer )
		delete [] m_IndexBuffer;

	if(m_decl)
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

	if ( meshLoader.Load(str.c_str(), m_VertexBuffer, m_IndexBuffer, m_iNbIndex, m_decl) == RES_SUCCEED)
	{
		if ( FillD3DBuffers () == RES_SUCCEED)
		{
			m_ReglagePivot = meshLoader.GetReglagePivot();
			return RES_SUCCEED;
		}
	}

	return RES_FAILED;
}

ResourceResult	Mesh::FillD3DBuffers ()
{
	ReleaseD3DBuffers ();
	m_iNbVertices=m_VertexBuffer.m_NbVertices;

	if (m_VertexBuffer.m_Datas && m_IndexBuffer)
	{
		LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;

		pDevice->CreateVertexBuffer (	m_VertexBuffer.m_Size, 
										NULL, 
										0, 
										D3DPOOL_DEFAULT, 
										&m_pVB, 
										NULL);

		void* pVertexBuffer;

		if(FAILED(m_pVB->Lock(0, m_VertexBuffer.m_Size, (void **)&pVertexBuffer, 0)))
			return RES_FAILED;

		memcpy(pVertexBuffer, m_VertexBuffer.m_Datas, m_VertexBuffer.m_Size);

		m_pVB->Unlock();


		
		u32 BufferSize = sizeof(int)*m_iNbIndex;

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
		if(m_VertexBuffer.m_VertexType==VertexBuffer::Skinned)
		{
			SkinnedVertex* pSVertexBuffer = (SkinnedVertex*)m_VertexBuffer.m_Datas;

			for( u32 i = 0; i < m_VertexBuffer.m_NbVertices; i++ )
			{
				pSVertexBuffer[i].m_Bones.x=(float)ArrayOrder[(int)pSVertexBuffer[i].m_Bones.x];
				pSVertexBuffer[i].m_Bones.y=(float)ArrayOrder[(int)pSVertexBuffer[i].m_Bones.y];
				pSVertexBuffer[i].m_Bones.z=(float)ArrayOrder[(int)pSVertexBuffer[i].m_Bones.z];
				pSVertexBuffer[i].m_Bones.w=(float)ArrayOrder[(int)pSVertexBuffer[i].m_Bones.w];
			}
		}
		else if(m_VertexBuffer.m_VertexType==VertexBuffer::SkinNormalMapped)
		{
			SkinNormalMappedVertex* pSVertexBuffer = (SkinNormalMappedVertex*)m_VertexBuffer.m_Datas;

			for( u32 i = 0; i < m_VertexBuffer.m_NbVertices; i++ )
			{
				pSVertexBuffer[i].m_Bones.x=(float)ArrayOrder[(int)pSVertexBuffer[i].m_Bones.x];
				pSVertexBuffer[i].m_Bones.y=(float)ArrayOrder[(int)pSVertexBuffer[i].m_Bones.y];
				pSVertexBuffer[i].m_Bones.z=(float)ArrayOrder[(int)pSVertexBuffer[i].m_Bones.z];
				pSVertexBuffer[i].m_Bones.w=(float)ArrayOrder[(int)pSVertexBuffer[i].m_Bones.w];
			}

		}

	}
	m_bOrdered=true;

}

void Mesh::ReleaseD3DBuffers()
{
	if ( m_pVB )
		m_pVB->Release(),	m_pVB = NULL;
	if ( m_pIB )
		m_pIB->Release(),	m_pIB = NULL;
}
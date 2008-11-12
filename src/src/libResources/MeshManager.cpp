#include "MeshManager.h"

#include "Mesh.h"

//******************************************************************

ResourceResult MeshManager::Load( crc32 Resource, LPDIRECT3DDEVICE9 pDevice )
{
	int i =0;

	// V�rifie si le mesh n'est pas d�ja dans la liste
	while(m_ResourcesList[i]->GetCrc32() != Resource && i < (int)m_ResourcesList.size())
		i++;
	
	// Si aucun Crc32 �gal � la ressource voulue n'a �t� trouv�
	if (i >= (int)m_ResourcesList.size())
	{
		Mesh* pMesh = new Mesh (Resource, pDevice);		// le ressource est charg�e
		
		m_ResourcesList.push_back (pMesh);
	}

	return RES_SUCCEED;
}

ResourceResult MeshManager::Release( crc32 Resource )
{
	//TODO

	return RES_FAILED;
}

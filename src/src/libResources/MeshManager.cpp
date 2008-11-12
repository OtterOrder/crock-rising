#include "MeshManager.h"

#include "Mesh.h"

//******************************************************************

ResourceResult MeshManager::Load( crc32 Resource, LPDIRECT3DDEVICE9 pDevice )
{
	int i =0;

	// Vérifie si le mesh n'est pas déja dans la liste
	while(m_ResourcesList[i]->GetCrc32() != Resource && i < (int)m_ResourcesList.size())
		i++;
	
	// Si aucun Crc32 égal à la ressource voulue n'a été trouvé
	if (i >= (int)m_ResourcesList.size())
	{
		Mesh* pMesh = new Mesh (Resource, pDevice);		// le ressource est chargée
		
		m_ResourcesList.push_back (pMesh);
	}

	return RES_SUCCEED;
}

ResourceResult MeshManager::Release( crc32 Resource )
{
	//TODO

	return RES_FAILED;
}

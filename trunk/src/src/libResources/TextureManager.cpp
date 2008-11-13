#include "TextureManager.h"
#include "Texture.h"

//******************************************************************


ResourceResult TextureManager::Load( crc32 resource)
{
	bool bNeedLoad = true;				//Vrai si la texture n'est pas dans la liste, on doit donc la charger.

	int i = 0;
	//Vérifie si la texture n'est pas déja dans la liste
	while(i != m_ResourcesList.size() && bNeedLoad == true)
	{
		//Test du crc32 resource, mais je vois pas encore trop comment
		if( ((Texture*) m_ResourcesList[i])->GetCrc32() == resource) bNeedLoad = false;
		i++;
	}

		//Dans le cas contraire, il la charge et la rajoute à la liste
	if(bNeedLoad)
	{
		Texture *pTex = new Texture();
		pTex->Initialize(resource);
		if(pTex->GetTex() == NULL)		//ERREUR AU CHARGEMENT
			return RES_FAILED;

		m_ResourcesList.push_back(pTex);
	}

	return RES_SUCCEED;
}

ResourceResult TextureManager::Release( crc32 resource )
{
	return RES_FAILED;
}
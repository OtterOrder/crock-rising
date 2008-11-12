#include "TextureManager.h"
#include "Texture.h"

//******************************************************************


ResourceResult TextureManager::Load( crc32 resource, LPDIRECT3DDEVICE9 pDevice, TCHAR strTexName )
{
	bool bNeedLoad = true;				//Vrai si la texture n'est pas dans la liste, on doit donc la charger.
	std::vector<Resource*>::iterator Iterator;
	Iterator = m_ResourcesList.begin();

	//V�rifie si la texture n'est pas d�ja dans la liste
	while(Iterator != m_ResourcesList.end() && bNeedLoad == true)
	{
		//Test du crc32 resource, mais je vois pas encore trop comment
		if(( (Texture*)(*Iterator) )->GetCrc32() == resource) bNeedLoad = false;
		Iterator++;
	}

		//Dans le cas contraire, il la charge et la rajoute � la liste
	if(bNeedLoad)
	{
		Texture *pTex = new Texture(strTexName, pDevice);
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
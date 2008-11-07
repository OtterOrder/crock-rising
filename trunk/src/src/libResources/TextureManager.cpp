#include "TextureManager.h"
#include "Texture.h"

//******************************************************************


ResourceResult TextureManager::Load( crc32 resource, TCHAR strTexName, LPDIRECT3DDEVICE9 pDevice )
{
	bool bNeedLoad = true;				//Vrai si la texture n'est pas dans la liste, on doit donc la charger.
	std::list<Texture*>::iterator Iterator;
	Iterator = m_TextureList.begin();

	//Vérifie si la texture n'est pas déja dans la liste
	while(Iterator != m_TextureList.end() && bNeedLoad == true)
	{
		//Test du crc32 resource, mais je vois pas encore trop comment
		if((*Iterator)->GetCrc32() == resource) bNeedLoad = false;
		Iterator++;
	}

		//Dans le cas contraire, il la charge et la rajoute à la liste
	if(bNeedLoad)
	{
		Texture *pTex = new Texture(strTexName, pDevice);
		if(pTex->GetTex() == NULL)		//ERREUR AU CHARGEMENT
			return RES_FAILED;

		m_TextureList.push_back(pTex);
	}

	return RES_SUCCEED;
}

ResourceResult TextureManager::Release( crc32 resource )
{
	return RES_FAILED;
}
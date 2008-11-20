#include "TextureManager.h"
#include "Texture.h"


/***********************************************************
 * Rajoute, si besoin est, une ressource à la liste.
 * @param[in]	resource : crc32 de la ressource, tous les paramètres de chargement de textures
 * @return	le résultat du chargement
 **********************************************************/
ResourceResult TextureManager::Load( crc32			resource,
									UINT			Width, 
									UINT			Height,
									UINT			MipLevels,
									DWORD			Usage,
									D3DFORMAT		Format, 
									D3DPOOL			Pool,
									DWORD			Filter,
									DWORD			MipFilter, 
									D3DCOLOR		ColorKey,
									D3DXIMAGE_INFO	*pSrcInfo,
									PALETTEENTRY		*pPalette )
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
		pTex->Initialize(resource,
							Width,
							Height,
							MipLevels,
							Usage,
							Format,
							Pool,
							Filter,
							MipFilter,
							ColorKey,
							pSrcInfo,
							pPalette);

		if(pTex == NULL)		//ERREUR AU CHARGEMENT
			return RES_FAILED;

		m_ResourcesList.push_back(pTex);
	}

	return RES_SUCCEED;
}


/***********************************************************
 * Libère la ressource.
 * @param[in]	resource : crc32 de la ressource
 * @return	le résultat de la libération
 **********************************************************/
ResourceResult TextureManager::Release( crc32 resource )
{
	return RES_FAILED;
}
#include "TextureManager.h"
#include "Texture.h"


/***********************************************************
 * Rajoute, si besoin est, une ressource � la liste.
 * @param[in]	resource : crc32 de la ressource, tous les param�tres de chargement de textures
 * @return	le r�sultat du chargement
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
	//V�rifie si la texture n'est pas d�ja dans la liste
	while(i != m_ResourcesList.size() && bNeedLoad == true)
	{
		//Test du crc32 resource, mais je vois pas encore trop comment
		if( ((Texture*) m_ResourcesList[i])->GetCrc32() == resource) bNeedLoad = false;
		i++;
	}

		//Dans le cas contraire, il la charge et la rajoute � la liste
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
 * Lib�re la ressource.
 * @param[in]	resource : crc32 de la ressource
 * @return	le r�sultat de la lib�ration
 **********************************************************/
ResourceResult TextureManager::Release( crc32 resource )
{
	return RES_FAILED;
}
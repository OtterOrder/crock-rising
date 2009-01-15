#include "ResourceManager.h"

#include "Resource.h"

/***********************************************************
 * Destructeur.
 **********************************************************/
ResourceManager::~ResourceManager( void )
{
	Clear();
}

/***********************************************************
 * Charge la ressource.
 * @param[in]	resource : crc32 de la ressource
 * @return	le r�sultat du chargement
 **********************************************************/
ResourceResult ResourceManager::Load( crc32 resource )
{
	return RES_FAILED;
}

/***********************************************************
 * Supprime la ressource.
 * @param[in]	resource : crc32 de la ressource
 * @return	le r�sultat de la suppression
 **********************************************************/
ResourceResult ResourceManager::Release( crc32 resource )
{
	return RES_FAILED;
}

/***********************************************************
 * Supprime toutes les ressources.
 * @return	le r�sultat de la suppression
 **********************************************************/
ResourceResult ResourceManager::Clear( void )
{
	Resource *pResource = NULL;
	
	while( !m_ResourcesList.empty() )
	{
		pResource = m_ResourcesList.back();
		m_ResourcesList.pop_back();
		delete pResource;
	}
	return RES_SUCCEED;
}

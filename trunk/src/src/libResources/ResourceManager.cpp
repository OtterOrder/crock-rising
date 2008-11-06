#include "ResourceManager.h"

#include "Resource.h"

//******************************************************************

ResourceResult ResourceManager::Load( crc32 resource )
{
	return RES_FAILED;
}


ResourceResult ResourceManager::Release( crc32 resource )
{
	return RES_FAILED;
}


ResourceResult ResourceManager::Clear( void )
{
	Resource *pResource = NULL;
	
	while( !m_ResourcesList.empty() )
	{
		pResource = m_ResourcesList.front();
		m_ResourcesList.pop_front();
		delete pResource;
	}
	return RES_SUCCEED;
}

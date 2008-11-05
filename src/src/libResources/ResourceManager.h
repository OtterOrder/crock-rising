#ifndef		_ResourceManager_H
#define		_ResourceManager_H

//******************************************************************

#include	<list>

#include	<libCore/Types/Crc32.h>
#include	<libCore/Singleton/Singleton.h>

using namespace std;

//******************************************************************

enum ResourceResult
{
	RES_SUCCEED,
	RES_FAILED,

};

//******************************************************************

class Resource;

//******************************************************************

class ResourceManager : public Singleton< ResourceManager >
{
	public:

		virtual ResourceResult	Load	( crc32 resource );		// Charge la ressource
		virtual	ResourceResult	Release	( crc32 resource );		// Supprime la ressource

		ResourceResult			Clear	( void );				// Supprime toutes les ressources

	protected:

		list< Resource* >		m_ResourcesList;		// Liste des ressources chargées

};

//******************************************************************
#endif		// _ResourceManager_H

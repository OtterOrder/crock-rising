#ifndef		_ResourceManager_H
#define		_ResourceManager_H

//******************************************************************

#include	<list>

#include	<libCore/Types/Crc32.h>
#include	<libCore/Singleton/Singleton.h>

#include	"Resource.h"


using namespace std;

//******************************************************************

class ResourceManager : public Singleton< ResourceManager >
{
	public:

		// =========================================================
		// Méthodes publiques
		
		virtual ResourceResult	Load	( crc32 resource );		// Charge la ressource
		virtual	ResourceResult	Release	( crc32 resource );		// Supprime la ressource

		ResourceResult			Clear	( void );				// Supprime toutes les ressources

	protected:

		// =========================================================
		// Données protégées
		
		list< Resource* >		m_ResourcesList;		// Liste des ressources chargées

};

//******************************************************************
#endif		// _ResourceManager_H

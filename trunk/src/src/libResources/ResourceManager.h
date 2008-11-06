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
		// M�thodes publiques
		
		virtual ResourceResult	Load	( crc32 resource );		// Charge la ressource
		virtual	ResourceResult	Release	( crc32 resource );		// Supprime la ressource

		ResourceResult			Clear	( void );				// Supprime toutes les ressources

	protected:

		// =========================================================
		// Donn�es prot�g�es
		
		list< Resource* >		m_ResourcesList;		// Liste des ressources charg�es

};

//******************************************************************
#endif		// _ResourceManager_H

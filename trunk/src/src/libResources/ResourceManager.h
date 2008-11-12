#ifndef		_ResourceManager_H
#define		_ResourceManager_H

//******************************************************************

#include	<vector>

#include	<libCore/Types/Crc32.h>
#include	<libCore/Singleton/Singleton.h>

#include	"Resource.h"

using namespace std;

//******************************************************************

class ResourceManager : public Singleton< ResourceManager >
{
	// Indispensable car Singleton doit pouvoir acc�der aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< ResourceManager >;

	public:

		// =========================================================
		// M�thodes publiques
		
		virtual ResourceResult	Load	( crc32 resource );		// Charge la ressource
		virtual	ResourceResult	Release	( crc32 resource );		// Supprime la ressource

		ResourceResult			Clear	( void );				// Supprime toutes les ressources


	protected:

		// =========================================================
		// Donn�es prot�g�es
		
		vector< Resource* >		m_ResourcesList;		// Liste des ressources charg�es


		// =========================================================
		// M�thodes prot�g�es

		ResourceManager					( void ){}
		virtual ~ResourceManager		( void );

};

//******************************************************************
#endif		// _ResourceManager_H

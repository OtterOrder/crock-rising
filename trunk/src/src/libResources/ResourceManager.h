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
	// Indispensable car Singleton doit pouvoir accéder aux
	// constructeur/destructeur, qui sont en protected.
	friend class Singleton< ResourceManager >;

	public:

		// =========================================================
		// Méthodes publiques
		
		virtual ResourceResult	Load	( crc32 resource );		// Charge la ressource
		virtual	ResourceResult	Release	( crc32 resource );		// Supprime la ressource

		ResourceResult			Clear	( void );				// Supprime toutes les ressources


	protected:

		// =========================================================
		// Données protégées
		
		vector< Resource* >		m_ResourcesList;		// Liste des ressources chargées


		// =========================================================
		// Méthodes protégées

		ResourceManager					( void ){}
		virtual ~ResourceManager		( void );

};

//******************************************************************
#endif		// _ResourceManager_H

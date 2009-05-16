#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include	"Resource.h"
#include	"Core/Singleton.h"
#include	"Core/Types/Crc32.h"
#include	<string>
#include	<map>

//===========================================================================//
// Classe permettant de manager l'ensemble des resources                     //
//===========================================================================//
class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton< ResourceManager >;

public:
	
	//=======================================================================//
	// Charge une resource					                                 //
	//=======================================================================//
	template< class T >
	T* Load( const std::string &resourceName, ResourceParam param = NULL )
	{
		// Recherche de la ressource
		TResourcesMap::const_iterator It = m_Resources.find( resourceName );

		// Si on l'a trouvé on la renvoie, sinon on l'alloue
		if( It != m_Resources.end() )
		{
			It->second->AddRef();
			return static_cast<T*>( It->second );
		}
		else
		{
			T *pResource	= new T();
			// pMom : pointeur de type de la classe mère :p
			Resource *pMom	= dynamic_cast<Resource*>( pResource );
			Add( resourceName, pMom );

			if( pMom->Load( resourceName, param ) != RES_SUCCEED )
			{
				// D'oh ! Faudrait ptêtre gérer ce cas ?
				// -> Echec du chargement de la ressource
			}
			pMom->AddRef();
			return pResource;
		}
	}
	
	//=======================================================================//
	// Retire une resource									                 //
	//=======================================================================//
	template< class T >
	void Remove( const std::string& resourceName )
	{
		// Recherche de la ressource dans la table
		TResourcesMap::iterator It = m_Resources.find(resourceName);

		// Si la ressource a été chargée
		if (It != m_Resources.end())
		{
			if( It->second->Release() == 0 )
			{
				// Retrait de la ressource de la liste
				m_Resources.erase(It);
			}
		}
	}

protected:
	
	ResourceManager();
    ~ResourceManager();

	//=======================================================================//
	// Types												                 //
	//=======================================================================//
	typedef std::map<std::string, Resource*> TResourcesMap;

	//=======================================================================//
	// Données membres										                 //
	//=======================================================================//
    TResourcesMap m_Resources; // Table contenant les ressources associées à leur nom de fichier

	//=======================================================================//
	// Méthodes protégées													 //
	//=======================================================================//
	void Add( const std::string& resource, Resource* Res );
	void Clear( void );

};

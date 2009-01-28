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

	public :
	//===========================================================================//
	// Charge une resource								                     //
	//===========================================================================//
    template <class T> T* Load( const std::string &resource, ResourceParam param = NULL )
	{
		// Recherche de la ressource
		TResourcesMap::const_iterator It = m_Resources.find(resource);

		// Si on l'a trouvé on la renvoie, sinon on l'alloue
		if (It != m_Resources.end())
		{
			It->second->AddRef();
			return static_cast<T*>(It->second);
		}
		else
		{
			T* Res=new T();
			Resource * Ptr=dynamic_cast<Resource*>( Res );
			Add( resource, Ptr );
			Ptr->Load( resource, param );
			Ptr->AddRef();
			return Res;
		}
	}

	//===========================================================================//
	// Ajoute une resource									                     //
	//===========================================================================//
    void Add(const std::string& resource, Resource* Res);

	//===========================================================================//
	// Retire une resource									                     //
	//===========================================================================//
    template <class T> void Remove(const std::string& resource)
	{
		// Recherche de la ressource dans la table
		TResourcesMap::iterator It = m_Resources.find(resource);

		// Si la ressource a été chargée
		if (It != m_Resources.end())
		{
			It->second->Release();
			// Retrait de la ressource de la liste
			m_Resources.erase(It);
		}
	}

private :
	
	void test();

    ResourceManager();
    ~ResourceManager();

	//===========================================================================//
	// Types												                     //
	//===========================================================================//
	typedef std::map<std::string, Resource*> TResourcesMap;

	//===========================================================================//
	// Données membres										                     //
	//===========================================================================//
    TResourcesMap m_Resources; // Table contenant les ressources associées à leur nom de fichier



};

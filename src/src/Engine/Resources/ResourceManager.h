#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Resource.h"
#include "../Core/Singleton.h"
#include "../Core/Types/Crc32.h"
#include <map>
#include <string>

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
    template <class T> T* Load(const std::string& resource)
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
			Resource * Ptr=dynamic_cast<Resource*>(Res);
			Add(resource, Ptr);
			Ptr->Load(resource);
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
    void Remove(const std::string& resource);

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

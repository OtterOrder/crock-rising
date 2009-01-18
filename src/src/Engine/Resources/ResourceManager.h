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
	// Récupère une resource								                     //
	//===========================================================================//
    template <class T> T* Get(const crc32& crc) const
	{
		 // Recherche de la ressource
		TResourcesMap::const_iterator It = m_Resources.find(crc);

		// Si on l'a trouvé on la renvoie, sinon on renvoie NULL
		if (It != m_Resources.end())
		{
			It->second->AddRef();
			return static_cast<T*>(It->second);
		}
		else
		{
			return NULL;
		}
	}

	//===========================================================================//
	// Charge une resource								                     //
	//===========================================================================//
    template <class T> T* Load(const crc32& crc)
	{
		// Recherche de la ressource
		TResourcesMap::const_iterator It = m_Resources.find(crc);

		// Si on l'a trouvé on la renvoie, sinon on renvoie NULL
		if (It != m_Resources.end())
		{
			It->second->AddRef();
			return static_cast<T*>(It->second);
		}
		else
		{
			T* Res=new T();
			Resource * Ptr=dynamic_cast<Resource*>(Res);
			Add(crc, Ptr);
			return Res;
		}
	}

	//===========================================================================//
	// Ajoute une resource									                     //
	//===========================================================================//
    void Add(const crc32& crc, Resource* Res);

	//===========================================================================//
	// Retire une resource									                     //
	//===========================================================================//
    void Remove(const crc32& crc);

private :
	
	void test();

    ResourceManager();
    ~ResourceManager();

	//===========================================================================//
	// Types												                     //
	//===========================================================================//
	typedef std::map<crc32, Resource*> TResourcesMap;

	//===========================================================================//
	// Données membres										                     //
	//===========================================================================//
    TResourcesMap m_Resources; // Table contenant les ressources associées à leur nom de fichier



};

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "ResourceManager.h"
#include <assert.h>

using namespace std;

//===========================================================================//
// Constructeur par défaut ResourceManager                                   //
//===========================================================================//
ResourceManager::ResourceManager()
{

}

//===========================================================================//
// Destructeur ResourceManager			                                     //
//===========================================================================//
ResourceManager::~ResourceManager()
{
    // S'il reste des ressources dans la liste, on le signale
    if (!m_Resources.empty())
    {

        for (TResourcesMap::const_iterator i = m_Resources.begin(); i != m_Resources.end(); ++i)
        {
			// Signalement

        }
    }
}

//===========================================================================//
// Ajoute une resource					                                     //
//===========================================================================//
void ResourceManager::Add(const std::string& resource, Resource* Res)
{
    assert(Res != NULL);

    // Si la ressource avait déjà été chargée, on le signale


    // Ajout de la ressource à la liste
    m_Resources[resource] = Res;
    Res->m_Name = resource;
}

void ResourceManager::test()
{
}

//===========================================================================//
// Retire une resource					                                     //
//===========================================================================//
void ResourceManager::Remove(const std::string& resource)
{
    // Recherche de la ressource dans la table
    TResourcesMap::iterator It = m_Resources.find(resource);

    // Si la ressource n'avait pas été chargée, on le signale
    //if (It == m_Resources.end())
        // Signalement

    // Retrait de la ressource de la liste
    m_Resources.erase(It);
}
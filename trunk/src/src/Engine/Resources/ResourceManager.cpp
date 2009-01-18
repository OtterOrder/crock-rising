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
void ResourceManager::Add(const crc32& crc, Resource* Res)
{
    assert(Res != NULL);

    // Si la ressource avait déjà été chargée, on le signale


    // Ajout de la ressource à la liste
    m_Resources[crc] = Res;
    Res->m_Crc32 = crc;
}

void ResourceManager::test()
{
}

//===========================================================================//
// Retire une resource					                                     //
//===========================================================================//
void ResourceManager::Remove(const crc32& crc)
{
    // Recherche de la ressource dans la table
    TResourcesMap::iterator It = m_Resources.find(crc);

    // Si la ressource n'avait pas été chargée, on le signale
    //if (It == m_Resources.end())
        // Signalement

    // Retrait de la ressource de la liste
    m_Resources.erase(It);
}
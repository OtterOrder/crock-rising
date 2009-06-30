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
	// Destruction de toutes les ressources restantes
	Clear();
	assert( m_Resources.empty() );
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

//===========================================================================//
// Détruit toutes les resources			                                     //
//===========================================================================//
void ResourceManager::Clear( void )
{
	if( !m_Resources.empty() )
	{
		for( TResourcesMap::iterator it = m_Resources.begin(); it != m_Resources.end(); it++ )
		{
			// On détruit la ressource quelque soit
			// le nombre de références restantes
			while( it->second->Release() > 0 );
		}
		m_Resources.clear();
	}
}

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "ResourceManager.h"
#include <assert.h>

using namespace std;

//===========================================================================//
// Constructeur par d�faut ResourceManager                                   //
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

    // Si la ressource avait d�j� �t� charg�e, on le signale


    // Ajout de la ressource � la liste
    m_Resources[resource] = Res;
    Res->m_Name = resource;
}

//===========================================================================//
// D�truit toutes les resources			                                     //
//===========================================================================//
void ResourceManager::Clear( void )
{
	if( !m_Resources.empty() )
	{
		for( TResourcesMap::iterator it = m_Resources.begin(); it != m_Resources.end(); it++ )
		{
			// On d�truit la ressource quelque soit
			// le nombre de r�f�rences restantes
			while( it->second->Release() > 0 );
		}
		m_Resources.clear();
	}
}

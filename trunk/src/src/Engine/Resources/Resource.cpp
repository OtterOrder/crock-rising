//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Resource.h"
#include "ResourceManager.h"


//===========================================================================//
// Constructeur par d�faut                                                   //
//===========================================================================//
Resource::Resource(): m_RefCount(0)
{

}

//===========================================================================//
// Destructeur Resource	                                                     //
//===========================================================================//
Resource::~Resource()
{
	ResourceManager::GetInstance()->Remove(m_Name);
}


//===========================================================================//
// Renvoie le nom associ� � la resource                                      //
//===========================================================================//
const std::string& Resource::GetName() const
{
    return m_Name;
}

//===========================================================================//
// Ajoute une r�f�rence � la resource                                        //
//===========================================================================//
void Resource::AddRef()
{
    ++m_RefCount;
}


//===========================================================================//
// Retire une r�f�rence sur la resource                                      //
//===========================================================================//
int Resource::Release()
{
    // D�cr�mentation du compteur de r�f�rences
    int RefCount = --m_RefCount;

    // S'il n'y a plus de r�f�rence sur la ressource, on la d�truit
    if (RefCount == 0)
        delete this;

    return RefCount;
}

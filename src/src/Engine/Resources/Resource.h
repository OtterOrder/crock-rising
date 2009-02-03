#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <d3dx9.h>
#include <stdio.h>
#include <string>
#include "Core/Types/Crc32.h"

//===========================================================================//
// Defines / enums / types
//===========================================================================//

enum ResourceResult
{
	RES_SUCCEED,
	RES_FAILED,
};

typedef void* ResourceParam; // Pointeur sur void, on peut s'en servir pour n'importe quoi

//===========================================================================//
// Classe g�n�rique d'une resource						                     //
//===========================================================================//
class Resource
{
public:
	
	Resource();
	virtual ~Resource() =0;
	
	virtual ResourceResult Load( std::string resource, ResourceParam param ) = 0 ; // Fonction virtuelle de chargement
	
	virtual void AddRef();	// Ajoute une r�f�rence sur la ressource
	int Release();			// Retire une r�f�rence sur la ressource

	const std::string& GetName() const; // Renvoie le nom associ� � la ressource

protected:
	
	// Amis
	friend class ResourceManager;
	
	// Donn�es prot�g�es
	std::string		m_Name;			// Nom de la ressource
	int				m_RefCount;		// Compteur de r�f�rences

private:

	// Copie interdite
	Resource( Resource& );
	void operator =( Resource& );

};

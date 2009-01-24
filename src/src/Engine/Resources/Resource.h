#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <d3dx9.h>
#include <stdio.h>
#include <string>
#include "../Core/Types/Crc32.h"

enum ResourceResult
{
	RES_SUCCEED,
	RES_FAILED,

};

//===========================================================================//
// Classe g�n�rique d'une resource						                     //
//===========================================================================//
class Resource
{
public :
    Resource();
    virtual ~Resource() = 0;

	//----------------------------------------------------------
    // Fonction virtuelle de chargement
    //----------------------------------------------------------
	virtual ResourceResult Load (std::string resource) {return RES_FAILED;}

    //----------------------------------------------------------
    // Renvoie le nom associ� � la ressource
    //----------------------------------------------------------
    const std::string& GetName() const;

    //----------------------------------------------------------
    // Ajoute une r�f�rence sur la ressource
    //----------------------------------------------------------
    virtual void AddRef();

    //----------------------------------------------------------
    // Retire une r�f�rence sur la ressource
    //----------------------------------------------------------
    int Release();

private :
    //----------------------------------------------------------
    // Amis
    //----------------------------------------------------------
    friend class ResourceManager;

    //----------------------------------------------------------
    // - Copie interdite -
    //----------------------------------------------------------
    Resource(Resource&);
    void operator =(Resource&);

    //----------------------------------------------------------
    // Donn�es membres
    //----------------------------------------------------------
	std::string		m_Name;		// CRC32 de la ressource
    int				m_RefCount;     // Compteur de r�f�rences
};

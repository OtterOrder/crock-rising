#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <d3dx9.h>
#include "../Resources/Resource.h"
#include <string>

//===========================================================================//
// Classe g�n�rique pour un shader                                           //
//===========================================================================//
class Shader : public Resource
{
	    
public :
	Shader();
	~Shader();

    HRESULT Load (const char * sPath);	// Charge le shader � partir d'un fichier

	//===========================================================================//
	// Changement des param�tres du shader                                       //
	//===========================================================================//
	void SetParameter(const std::string& Name, float Value) {}					// Change un param�tre du shader (float)
	void SetParameter(const std::string& Name, const D3DXVECTOR2& Value) {}		// Change un param�tre du shader (vector 2)
    void SetParameter(const std::string& Name, const D3DXVECTOR3& Value) {}		// Change un param�tre du shader (vector 3)
    void SetParameter(const std::string& Name, const D3DXVECTOR4& Value) {}		// Change un param�tre du shader (vector 4)
	void SetParameter(const std::string& Name, const D3DXMATRIX& Value) {}	    // Change un param�tre du shader (matrice)
	void SetParameter(const std::string& Name, const D3DXCOLOR& Value) {}		// Change un param�tre du shader (couleur)

	ID3DXEffect*				m_pEffect;		// Effect de l'objet
};
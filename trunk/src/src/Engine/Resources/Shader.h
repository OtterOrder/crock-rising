#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include <d3dx9.h>
#include "../Resources/Resource.h"
#include <string>

//===========================================================================//
// Classe générique pour un shader                                           //
//===========================================================================//
class Shader : public Resource
{
	    
public :
	Shader();
	~Shader();

    HRESULT Load (const char * sPath);	// Charge le shader à partir d'un fichier

	//===========================================================================//
	// Changement des paramètres du shader                                       //
	//===========================================================================//
	void SetParameter(const std::string& Name, float Value) {}					// Change un paramètre du shader (float)
	void SetParameter(const std::string& Name, const D3DXVECTOR2& Value) {}		// Change un paramètre du shader (vector 2)
    void SetParameter(const std::string& Name, const D3DXVECTOR3& Value) {}		// Change un paramètre du shader (vector 3)
    void SetParameter(const std::string& Name, const D3DXVECTOR4& Value) {}		// Change un paramètre du shader (vector 4)
	void SetParameter(const std::string& Name, const D3DXMATRIX& Value) {}	    // Change un paramètre du shader (matrice)
	void SetParameter(const std::string& Name, const D3DXCOLOR& Value) {}		// Change un paramètre du shader (couleur)

	ID3DXEffect*				m_pEffect;		// Effect de l'objet
};
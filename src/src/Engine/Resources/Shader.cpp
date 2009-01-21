//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Shader.h"
#include "../Renderer/Renderer.h"

//===========================================================================//
// Constructeur Shader                                                       //
//===========================================================================//
Shader::Shader()
{
	m_pEffect=NULL;
}

Shader::~Shader()
{
	if(m_pEffect)
		m_pEffect->Release();
	m_pEffect=NULL;
}

//===========================================================================//
// Chargement d'un shader                                                    //
//===========================================================================//
ResourceResult Shader::Load(std::string resource)
{
	std::string str="data\\shader\\"+resource;

	ID3DXBuffer  * err=NULL;
	if(FAILED(D3DXCreateEffectFromFile(Renderer::GetInstance()->m_pd3dDevice, str.c_str(), NULL, NULL, D3DXSHADER_NO_PRESHADER, NULL, &m_pEffect, &err )))
	{
		char* pError = reinterpret_cast<char*>(err->GetBufferPointer());
		OutputDebugString(pError);
		MessageBox(0, pError, "ERROR", MB_OK);
		err->Release();
		return RES_FAILED;
	}

	return RES_SUCCEED;
}

//===========================================================================//
// Changement de paramètres                                                  //
//===========================================================================//
// Change un paramètre du shader (float)
void Shader::SetParameter(const std::string& Name, float Value) 
{
}

// Change un paramètre du shader (vector 2)
void Shader::SetParameter(const std::string& Name, const D3DXVECTOR2& Value) 
{
}

// Change un paramètre du shader (vector 3)
void Shader::SetParameter(const std::string& Name, const D3DXVECTOR3& Value) 
{
}

// Change un paramètre du shader (vector 4)
void Shader::SetParameter(const std::string& Name, const D3DXVECTOR4& Value) 
{
}		

// Change un paramètre du shader (matrice)
void Shader::SetParameter(const std::string& Name, const D3DXMATRIX& Value) 
{
}	    

// Change un paramètre du shader (couleur)
void Shader::SetParameter(const std::string& Name, const D3DXCOLOR& Value) 
{
}		

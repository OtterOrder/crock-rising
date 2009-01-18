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
HRESULT Shader::Load(const char * sPath)
{
	ID3DXBuffer  * err=NULL;
	if(FAILED(D3DXCreateEffectFromFile(Renderer::GetInstance()->m_pd3dDevice, sPath, NULL, NULL, D3DXSHADER_NO_PRESHADER, NULL, &m_pEffect, &err )))
	{
		char* pError = reinterpret_cast<char*>(err->GetBufferPointer());
		OutputDebugString(pError);
		MessageBox(0, pError, "ERROR", MB_OK);
		err->Release();
	}

	return S_FALSE;
}

//===========================================================================//
// Changement de paramètres                                                  //
//===========================================================================//

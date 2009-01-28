//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Texture.h"
#include "../Renderer/Renderer.h"

//===========================================================================//
// Constructeur Texture
//===========================================================================//
Texture::Texture()
{
	m_pTex = NULL; // Init du pointeur vers la tex
}

//===========================================================================//
// Destructeur
//===========================================================================//
Texture::~Texture()
{
	m_pTex->Release();
}

//===========================================================================//
// Charge une texture.
// @param[in]	resource : nom du fichier
// @param[in]	param : spécifie le répertoire à explorer
//===========================================================================//
ResourceResult Texture::Load( std::string resource, ResourceParam param )
{
	m_Name = resource;
	
	if( param )
	{
		//TODO
	}
	
	return DxLoad( "../../data/texture/" + m_Name );
}

//===========================================================================//
// Charge une texture dans une structure DirectX.
// @param[in]	path : chemin complet vers le fichier
//===========================================================================//
ResourceResult Texture::DxLoad( std::string path )
{
	if( FAILED( D3DXCreateTextureFromFileEx(
		Renderer::GetInstance()->m_pd3dDevice,
		path.c_str(),
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		NULL,
		NULL,
		&m_pTex ) ) )
	{
		m_pTex = NULL;
		return RES_FAILED;
	}
	return RES_SUCCEED;
}

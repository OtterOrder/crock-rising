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
	if( m_pTex ) m_pTex->Release();
}

//===========================================================================//
// Charge une texture.
// @param[in]	resource : nom du fichier
// @param[in]	param : type de texture (TextureType)
//===========================================================================//
ResourceResult Texture::Load( std::string resource, ResourceParam param )
{
	std::string dir;
	//m_Name = resource;
	
	// Sélection du répertoire en fonction du type
	switch( (TextureType)(_int64)param ) // <-- Aaaalalala, c'est super naze ça !!!
	{
		case TEX_SPRITE:	dir = "../../data/sprite/";	break;
		default:			dir = "../../data/texture/";
	}
	
	// Chargement de la texture
	return DxLoad( dir + resource );
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

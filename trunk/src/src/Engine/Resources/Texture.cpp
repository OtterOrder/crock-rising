//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Texture.h"
#include "Core/System.h"
#include "Renderer/Renderer.h"

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
	if( m_pTex )
		m_pTex->Release();
}

//===========================================================================//
// Donne la largeur de l'image source.
//===========================================================================//
int Texture::GetSrcWidth() const
{
	return (int)m_SrcInfo.Width;
}

//===========================================================================//
// Donne la hauteur de l'image source.
//===========================================================================//
int Texture::GetSrcHeight() const
{
	return (int)m_SrcInfo.Height;
}

//===========================================================================//
// Charge une texture.
// @param[in]	resource : nom du fichier
// @param[in]	param : type de texture (TextureType)
//===========================================================================//
ResourceResult Texture::Load( std::string resource, ResourceParam param )
{
	std::string texDir;
	texDir = System::GetInstance()->GetDataPath();
	
	// Sélection du répertoire en fonction du type
	switch( (Texture::Type)(int)param )
	{
		case SPRITE:	texDir += "sprite/";	break;
		default:		texDir += "texture/";
	}
	
	// Chargement de la texture
	return DxLoad( texDir + resource );
}

//===========================================================================//
// Donne la texture DirectX.
// @return	Pointeur sur la texture DirectX
//===========================================================================//
IDirect3DTexture9* const Texture::GetTexture() const
{
	return m_pTex;
}

//===========================================================================//
// Change la texture DirectX.
// @param[in]	pTexture : Pointeur sur la texture DirectX
//===========================================================================//
void Texture::SetTexture( IDirect3DTexture9 *pTexture )
{
	if( m_pTex )
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
	m_pTex = pTexture;
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
		&m_SrcInfo,
		NULL,
		&m_pTex ) ) )
	{
		m_pTex = NULL;
		return RES_FAILED;
	}
	return RES_SUCCEED;
}

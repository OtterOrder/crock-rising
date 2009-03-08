#include	"Sprite.h"

//#include	<assert.h>
#include	<d3dx9.h>

//#include	"Core/Math.h"
#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Texture.h"

//******************************************************************

//**********************************************************
// Constructeur.
// @param[in]	spriteID : ID du sprite
//**********************************************************
/*Sprite::Sprite( crc32 spriteID )
: Object2D()
{
	m_TextureID = spriteID;
}*/

//**********************************************************
// Constructeur.
// @param[in]	path : chemin vers l'image (texture)
//**********************************************************
Sprite::Sprite( const char *path )
: Quad( 0.f, 0.f, Color4f( 0.f, 0.f, 0.f, 1.f ) )
{
	m_TextureName = path;
}

//**********************************************************
// Destructeur.
//**********************************************************
Sprite::~Sprite()
{
	ClearDxData();
}

//**********************************************************
// Affiche l'objet.
//**********************************************************
void Sprite::Draw()
{
	if( !m_IsDxReady )
		return;

	//TODO
}

//**********************************************************
// Initialise les données Dx.
//**********************************************************
void Sprite::InitDxData()
{
	if( m_IsDxReady )
		return;

	//TODO
	
	m_IsDxReady = true;
}

//**********************************************************
// Libère les données Dx.
//**********************************************************
void Sprite::ClearDxData()
{
	if( m_IsDxReady )
	{
		//TODO
	}
	m_IsDxReady = false;
}

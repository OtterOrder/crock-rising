#include	"Sprite.h"

#include	<assert.h>
#include	<d3dx9.h>

#include	"Core/System.h"
#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Texture.h"

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	spriteID : ID du sprite
 **********************************************************/
/*Sprite::Sprite( crc32 spriteID )
: Object2D()
{
	//TODO
}*/

/***********************************************************
 * Constructeur.
 * @param[in]	path : chemin vers l'image (texture)
 **********************************************************/
Sprite::Sprite( const char *path )
: Object2D()
{
	m_TextureName	= path;
	m_IsDxReady		= false;

	InitDxData();
}

/***********************************************************
 * Destructeur.
 **********************************************************/
Sprite::~Sprite()
{
	ClearDxData();
}

/***********************************************************
 * Affiche l'objet.
 **********************************************************/
void Sprite::Draw() const
{
	if( !m_IsDxReady )
		return;

	// L� on est s�r que les donn�es directx sont initialis�es..
	
	m_pDxSprite->Begin( 0 );

	m_pDxSprite->Draw(
		m_pDxTexture,
		NULL,
		NULL,
		&m_Position,
		D3DCOLOR_RGBA( 255, 255, 255, 255 )
	);

	m_pDxSprite->End();
}


/***********************************************************
 * Initialise les donn�es Dx.
 **********************************************************/
void Sprite::InitDxData()
{
	if( m_IsDxReady )
		return;

	//TODO: blinder cette m�thode (FAILED?)
	
	// Cr�ation du sprite Dx et r�cup�ration de la texture
	D3DXCreateSprite( Renderer::GetInstance()->m_pd3dDevice, &m_pDxSprite );
	m_pDxTexture = ResourceManager::GetInstance()->Load<Texture>( m_TextureName, (ResourceParam)TEX_SPRITE )->m_pTex;
	
	m_IsDxReady = true;
}

/***********************************************************
 * Lib�re les donn�es Dx.
 **********************************************************/
void Sprite::ClearDxData()
{
	if( m_IsDxReady )
	{
		ResourceManager::GetInstance()->Remove<Texture>( m_TextureName );
		m_pDxSprite->Release();
	}
	m_IsDxReady = false;
}

#include	"Sprite.h"

//#include	<assert.h>
#include	<d3dx9.h>

//#include	"Core/Math.h"
#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Texture.h"

//******************************************************************

/***********************************************************
 * Initialisation commune aux constructeurs.
 **********************************************************/
void Sprite::CommonInit()
{
	m_IsDxReady	= false;
}

/***********************************************************
 * Constructeur.
 * @param[in]	spriteID : ID du sprite
 **********************************************************/
/*Sprite::Sprite( crc32 spriteID )
: Object2D()
{
	CommonInit();
}*/

/***********************************************************
 * Constructeur.
 * @param[in]	path : chemin vers l'image (texture)
 **********************************************************/
Sprite::Sprite( const char *path )
: Object2D()
{
	CommonInit();
	m_TextureName = path;
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

	// Application des transformations
	D3DXMATRIX world;
	WorldMatrix( &world );
	m_pDxSprite->SetTransform( &world );

	m_pDxSprite->Begin( D3DXSPRITE_ALPHABLEND );
	
	// On dessine..
	m_pDxSprite->Draw(
		m_pDxTexture,
		NULL,			// on utilise toute l'image
		NULL,
		NULL,
		D3DCOLOR_COLOR4F( m_Color )
	);

	m_pDxSprite->End();
}

/***********************************************************
 * Initialise les données Dx.
 **********************************************************/
void Sprite::InitDxData()
{
	if( m_IsDxReady )
		return;

	//TODO: blinder cette méthode (FAILED?)
	
	// Création du sprite Dx et récupération de la texture
	D3DXCreateSprite( Renderer::GetInstance()->m_pd3dDevice, &m_pDxSprite );
	m_pDxTexture = ResourceManager::GetInstance()->Load<Texture>( m_TextureName, (ResourceParam)TEX_SPRITE )->m_pTex;
	
	m_IsDxReady = true;
}

/***********************************************************
 * Libère les données Dx.
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

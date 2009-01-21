#include	"Sprite.h"

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
Sprite::Sprite( crc32 spriteID )
: Object2D()
{
	//TODO

	m_TextureID = spriteID; // temporaire

	D3DXCreateSprite(
		Renderer::GetInstance()->m_pd3dDevice,
		&m_pSprite
	);
}

/***********************************************************
 * Destructeur.
 **********************************************************/
Sprite::~Sprite( void )
{
}

/***********************************************************
 * Affiche l'objet.
 **********************************************************/
void Sprite::Draw( void ) const
{
	m_pSprite->Begin( 0 );

	/*m_pSprite->Draw(
		ResourceManager::GetInstance()->Load<Texture>( m_TextureID )->m_pTex,
		NULL,
		NULL,
		&m_Position,
		m_Color
	);*/

	m_pSprite->End();
}

#include	"Sprite.h"

//#include	<assert.h>
#include	<d3dx9.h>

#include	"Core/Math.h"
#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Texture.h"

//******************************************************************

#define		SPRITE_DEFAULT_SCALE	(Vector3f( 1.f, 1.f, 1.f ))	// Echelle par défaut
#define		SPRITE_DEFAULT_ALPHA	(1.f)						// Alpha par défaut

//******************************************************************

/***********************************************************
 * Initialisation commune aux constructeurs.
 **********************************************************/
void Sprite::CommonInit()
{
	m_Scale		= SPRITE_DEFAULT_SCALE;
	m_Alpha		= SPRITE_DEFAULT_ALPHA;
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
	D3DXMATRIX rotat, trans, scale, world;
	
	D3DXMatrixIdentity( &rotat ); //------------------------------- TEMP, pas de rotation pour le moment
	D3DXMatrixTranslation( &trans, m_Position.x, m_Position.y, m_Position.z );
	D3DXMatrixScaling( &scale, m_Scale.x, m_Scale.y, m_Scale.z );

	world = scale * trans * rotat;
	m_pDxSprite->SetTransform( &world );

	m_pDxSprite->Begin( D3DXSPRITE_ALPHABLEND );
	
	// On dessine..
	m_pDxSprite->Draw(
		m_pDxTexture,
		NULL,			// on utilise toute l'image
		NULL,
		NULL,
		D3DCOLOR_RGBA( 255, 255, 255, (unsigned char)(m_Alpha*255) )
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

/***********************************************************
 * Change l'échelle.
 **********************************************************/
void Sprite::SetScale( float scaleX, float scaleY )
{
	m_Scale.x = scaleX;
	m_Scale.y = scaleY;
}

/***********************************************************
 * Change l'échelle.
 **********************************************************/
void Sprite::SetScale( const Vector2f &scale )
{
	SetScale( scale.x, scale.y );
}

/***********************************************************
 * Donne l'échelle.
 **********************************************************/
Vector2f Sprite::GetScale() const
{
	return Vector2f( m_Scale.x, m_Scale.y );
}

/***********************************************************
 * Change l'alpha (= transparence).
 * @param[in]	alpha : alpha (0->1)
 **********************************************************/
void Sprite::SetAlpha( float alpha )
{
	m_Alpha = MATH_Clamp( alpha, 0.f, 1.f );
}

/***********************************************************
 * Donne l'alpha.
 * @return	l'alpha (woohoo)
 **********************************************************/
float Sprite::GetAlpha() const
{
	return m_Alpha;
}

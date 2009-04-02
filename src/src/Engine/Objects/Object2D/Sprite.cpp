#include	"Sprite.h"

//#include	<assert.h>
#include	<d3dx9.h>

//#include	"Core/Math.h"
#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Texture.h"

//******************************************************************

#define		SPRITE_DEFAULT_COLOR	(Color4f( 1.f, 1.f, 1.f, 1.f ))

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
: Quad( 0, 0, SPRITE_DEFAULT_COLOR )
{
	m_TextureName	= path;
	m_pTexture		= NULL;
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
	if( !IsDxReady() )
	{
		// Si les données directx ne sont pas initialisées,
		// on le fait au premier affichage, ça évite de le
		// faire dans les constructeurs..
		InitDxData();
	}

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = Renderer::GetInstance()->m_pd3dDevice;
	
	pDevice->SetTexture( 0, m_pTexture->m_pTex );
	Quad::Draw();
	pDevice->SetTexture( 0, NULL );
}

//**********************************************************
// Initialise les données Dx.
//**********************************************************
void Sprite::InitDxData()
{
	Quad::InitDxData();

	// Chargement de la texture..
	m_pTexture = ResourceManager::GetInstance()->Load<Texture>(
		m_TextureName,
		(ResourceParam)Texture::SPRITE
	);

	// Initialisation de la taille du sprite
	m_Width		= m_pTexture->GetSrcWidth();
	m_Height	= m_pTexture->GetSrcHeight();
}

//**********************************************************
// Libère les données Dx.
//**********************************************************
void Sprite::ClearDxData()
{
	Quad::ClearDxData();

	// Déchargement de la texture..
	ResourceManager::GetInstance()->Remove<Texture>( m_TextureName );
	m_pTexture = NULL;
}

//**********************************************************
// Vérifie si les données Dx sont prètes.
//**********************************************************
bool Sprite::IsDxReady() const
{
	return Quad::IsDxReady()
		&& m_pTexture;
}

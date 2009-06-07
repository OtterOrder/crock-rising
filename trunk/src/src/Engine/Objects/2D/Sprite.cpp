#include	"Sprite.h"

#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Texture.h"

//******************************************************************

#define		SPRITE_DEFAULT_COLOR	(Color4f( 1.f, 1.f, 1.f, 1.f ))

//******************************************************************

//**********************************************************
// Constructeur avec paramètre.
// @param[in]	spriteName : Nom de l'image (texture)
//**********************************************************
Sprite::Sprite( const std::string &spriteName )
: Quad( 0, 0, SPRITE_DEFAULT_COLOR )
{
	m_SpriteName	= spriteName;
	m_Texture		= NULL;
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
		// Si les données dx n'existent pas, on les crées ici, ça évite
		// de le faire dans les constructeurs. On active le dirty et on
		// sort, on n'affichera qu'à la prochaine frame..
		InitDxData();
		activate_dirty();
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;
	
	// Rendu..
	pDevice->SetTexture( 0, m_Texture->m_pTex );

	// Optim: on n'appelle PAS Quad::Draw, ça reviendrait
	// au même mais avec un appel de fonction en plus
	pDevice->SetVertexDeclaration( m_VertexDeclaration );
	pDevice->SetStreamSource( 0, m_VertexBuffer, 0, sizeof(Vertex) );
	pDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );

	pDevice->SetTexture( 0, NULL );
}

//**********************************************************
// Initialise les données Dx.
//**********************************************************
void Sprite::InitDxData()
{
	Quad::InitDxData();

	// Chargement de la texture..
	m_Texture = ResourceManager::GetInstance()->Load<Texture>(
		m_SpriteName,
		(ResourceParam)Texture::SPRITE
	);

	// Initialisation de la taille du sprite
	m_Width = m_Texture->GetSrcWidth();
	m_Height = m_Texture->GetSrcHeight();
}

//**********************************************************
// Libère les données Dx.
//**********************************************************
void Sprite::ClearDxData()
{
	Quad::ClearDxData();
	
	// Déchargement de la texture..
	ResourceManager::GetInstance()->Remove<Texture>( m_SpriteName );
	m_Texture = NULL;
}

//**********************************************************
// Vérifie si les données Dx sont prètes.
//**********************************************************
bool Sprite::IsDxReady() const
{
	return Quad::IsDxReady()
		&& m_Texture;
}

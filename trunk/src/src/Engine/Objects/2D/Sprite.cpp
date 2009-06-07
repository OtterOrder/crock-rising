#include	"Sprite.h"

#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Texture.h"

//******************************************************************

#define		SPRITE_DEFAULT_COLOR	(Color4f( 1.f, 1.f, 1.f, 1.f ))

//******************************************************************

//**********************************************************
// Constructeur avec param�tre.
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
		// Si les donn�es dx n'existent pas, on les cr�es ici, �a �vite
		// de le faire dans les constructeurs. On active le dirty et on
		// sort, on n'affichera qu'� la prochaine frame..
		InitDxData();
		activate_dirty();
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;
	
	// Rendu..
	pDevice->SetTexture( 0, m_Texture->m_pTex );

	// Optim: on n'appelle PAS Quad::Draw, �a reviendrait
	// au m�me mais avec un appel de fonction en plus
	pDevice->SetVertexDeclaration( m_VertexDeclaration );
	pDevice->SetStreamSource( 0, m_VertexBuffer, 0, sizeof(Vertex) );
	pDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );

	pDevice->SetTexture( 0, NULL );
}

//**********************************************************
// Initialise les donn�es Dx.
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
// Lib�re les donn�es Dx.
//**********************************************************
void Sprite::ClearDxData()
{
	Quad::ClearDxData();
	
	// D�chargement de la texture..
	ResourceManager::GetInstance()->Remove<Texture>( m_SpriteName );
	m_Texture = NULL;
}

//**********************************************************
// V�rifie si les donn�es Dx sont pr�tes.
//**********************************************************
bool Sprite::IsDxReady() const
{
	return Quad::IsDxReady()
		&& m_Texture;
}

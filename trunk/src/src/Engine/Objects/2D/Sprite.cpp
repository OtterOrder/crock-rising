#include	"Sprite.h"

#include	"Renderer/Renderer.h"
#include	"Resources/ResourceManager.h"
#include	"Resources/Shader.h"
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
	OnLostDevice();
	OnDestroyDevice();
}

//**********************************************************
// Affiche l'objet.
//**********************************************************
void Sprite::Draw()
{
	if( !IsDrawable() )
	{
		// Si les donn�es dx n'existent pas, on les cr�es ici (�a �vite
		// de le faire dans les constructeurs) puis on sort, on n'affichera
		// qu'� la prochaine frame..
		OnCreateDevice();
		OnResetDevice();
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Renderer::GetInstance()->m_pd3dDevice;

	// Param�tres du shader
	m_Shader->GetEffect()->SetValue( "g_Color", (void*)&m_Color, sizeof(Color4f) );
	m_Shader->GetEffect()->SetBool( "g_IsTextured", true );
	m_Shader->GetEffect()->SetTexture( "g_Texture", m_Texture->GetTexture() );
	m_Shader->GetEffect()->SetTechnique( "RenderScene" );
	
	m_Shader->GetEffect()->Begin( 0, 0 );
	m_Shader->GetEffect()->BeginPass( 0 );

	// Rendu..
	pDevice->SetVertexDeclaration( m_VertexDeclaration );
	pDevice->SetStreamSource( 0, m_VertexBuffer, 0, sizeof(Vertex) );
	pDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );

	m_Shader->GetEffect()->EndPass();
	m_Shader->GetEffect()->End();
}

//**********************************************************
// Callback appel�e juste apr�s la cr�ation du device.
//**********************************************************
void Sprite::OnCreateDevice()
{
	Quad::OnCreateDevice();

	if( !m_Texture )
	{
		// Chargement de la texture..
		m_Texture = ResourceManager::GetInstance()->Load<Texture>(
			m_SpriteName,
			(ResourceParam)Texture::SPRITE
		);
		
		// Initialisation de la taille du sprite
		m_Width = m_Texture->GetSrcWidth();
		m_Height = m_Texture->GetSrcHeight();
	}
}

//**********************************************************
// Callback appel�e � chaque fois qu'on d�truit le device.
//**********************************************************
void Sprite::OnDestroyDevice()
{
	Quad::OnDestroyDevice();

	if( m_Texture )
	{
		// D�chargement de la texture..
		ResourceManager::GetInstance()->Remove<Texture>( m_SpriteName );
		m_Texture = NULL;
	}
}

//**********************************************************
// V�rifie que tout est pret pour l'affichage.
//**********************************************************
bool Sprite::IsDrawable() const
{
	return Quad::IsDrawable()
		&& m_Texture;
}
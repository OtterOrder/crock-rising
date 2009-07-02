#include	"Quad.h"

#include	"Renderer/Renderer.h"
#include	"Resources/Shader.h"

//******************************************************************

#define		QUAD_VERTICES			4
#define		QUAD_DEFAULT_POSITIONT	(Vector4f( 0.f, 0.f, 0.f, 1.f ))

#define		RENDERER				(Renderer::GetInstance())	// Raccourcis vers le Renderer

//******************************************************************

// Coordonn�es de texture par d�faut du quad
static Vector2f g_DefaultTexCoord[QUAD_VERTICES] =
{
	Vector2f( 0.f, 0.f ),
	Vector2f( 1.f, 0.f ),
	Vector2f( 1.f, 1.f ),
	Vector2f( 0.f, 1.f )
};

//******************************************************************

//**********************************************************
// Initialisation commune aux constructeurs.
//**********************************************************
void Quad::CommonInit()
{
	m_NbVertices = QUAD_VERTICES;
	m_Vertices = new Vertex[m_NbVertices];
	activate_dirty(); // Pour initialiser les sommets
}

//**********************************************************
// Constructeur.
// @param[in]	width	: Largeur
// @param[in]	height	: Hauteur
// @param[in]	color	: Couleur
//**********************************************************
Quad::Quad( u32 width, u32 height, const Color4f &color )
: SceneObject2D()
{
	m_Width		= width;
	m_Height	= height;
	m_Color		= color;
	CommonInit();
}

//**********************************************************
// Constructeur.
// @param[in]	size	: { Largeur, Hauteur }
// @param[in]	color	: Couleur
//**********************************************************
Quad::Quad( const Vector2i &size, const Color4f &color )
: SceneObject2D()
{
	m_Width		= (u32)size.x;
	m_Height	= (u32)size.y;
	m_Color		= color;
	CommonInit();
}

//**********************************************************
// Destructeur.
//**********************************************************
Quad::~Quad()
{
	if( m_Vertices )
		delete[] m_Vertices;
}

//**********************************************************
// Affiche l'objet.
//**********************************************************
void Quad::Draw()
{
	// Param�tres du shader
	m_Shader->GetEffect()->SetValue( "g_Color", (void*)&m_Color, sizeof(Color4f) );
	m_Shader->GetEffect()->SetBool( "g_IsTextured", false );
	m_Shader->GetEffect()->SetTechnique( "RenderScene" );
	
	m_Shader->GetEffect()->Begin( 0, 0 );
	m_Shader->GetEffect()->BeginPass( 0 );
	
	// Rendu..
	DrawWithoutEffect();

	m_Shader->GetEffect()->EndPass();
	m_Shader->GetEffect()->End();
}

//**********************************************************
// Affiche l'objet via le pipeline fixe de la cg.
//**********************************************************
void Quad::DrawWithoutEffect()
{
	LPDIRECT3DDEVICE9 pDevice = RENDERER->m_pd3dDevice;

	// Rendu..
	pDevice->SetVertexDeclaration( m_VertexDeclaration );
	pDevice->SetStreamSource( 0, m_VertexBuffer, 0, sizeof(Vertex) );
	pDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
}

//**********************************************************
// Change la taille.
// @param[in]	width	: Largeur en pixels
// @param[in]	height	: Hauteur en pixels
//**********************************************************
void Quad::SetSize( u32 width, u32 height )
{
	if( width != m_Width || height != m_Height )
	{
		m_Width = width;
		m_Height = height;
		activate_dirty();
	}
}

//**********************************************************
// Change la taille.
// @param[in]	size : { largeur, hauteur }
//**********************************************************
void Quad::SetSize( const Vector2i &size )
{
	SetSize( (u32)size.x, (u32)size.y );
}

//**********************************************************
// Change la largeur.
// @param[in]	width : Largeur en px
//**********************************************************
void Quad::SetWidth( u32 width )
{
	SetSize( width, m_Height );
}

//**********************************************************
// Change la hauteur.
// @param[in]	height : Hauteur en px
//**********************************************************
void Quad::SetHeight( u32 height )
{
	SetSize( m_Width, height );
}

//**********************************************************
// Donne la largeur de l'objet.
//**********************************************************
u32 Quad::GetWidth() const
{
	return m_Width;
}

//**********************************************************
// Donne la hauteur de l'objet.
//**********************************************************
u32 Quad::GetHeight() const
{
	return m_Height;
}

//**********************************************************
// D�finie l'objet comme background, c�d l'objet est de la
// taille de la fen�tre et a la priorit� minimum.
// @param[in]	background : Vrai = background
//**********************************************************
void Quad::SetAsBackground( bool background )
{
	if( background )
	{
		SetFlag( O2D_BACKGROUND );
		
		// Aucune transformation, point chaud
		// en (0,0) et priorit� minimum..
		SetPosition( 0, 0 );
		SetScale( 1.f, 1.f );
		SetRotation( 0.f );
		SetHotPoint( 0, 0 );
		SetPriority( 255 );
	}
	else
	{
		UnsetFlag( O2D_BACKGROUND );
	}
}

//**********************************************************
// V�rifie si le point est dans le quad. /!\ Maxi bidouille,
// ne tient pas compte de la rotation et du scale.
// @param[in]	posX : Coordonn�e x (px)
// @param[in]	posY : Coordonn�e y (px)
// @return	Vrai si le point est en collision
//**********************************************************
bool Quad::IsCollide( s32 posX, s32 posY )
{
	return posX >= m_Position.x
		&& posX <= m_Position.x + m_Width
		&& posY >= m_Position.y
		&& posY <= m_Position.y + m_Height;
}

//**********************************************************
// Recalcule les sommets de l'objet. Cette m�thode est
// appel�e quand le dirty est activ�.
// Configuration du quad :
//
//   v0 +-------+ v1
//      |       |
//      |       |
//      |       |
//   v3 +-------+ v2
//
//**********************************************************
void Quad::dirty_Refresh()
{
	// 0. Gestion des flags
	if( IsFlagSet( O2D_BACKGROUND ) )
	{
		m_Width = Renderer::GetInstance()->GetWindowWidth();
		m_Height = Renderer::GetInstance()->GetWindowHeight();
	}
	
	// 1. On r�initialise les sommets
	for( int i = 0; i < m_NbVertices; i++ )
	{
		m_Vertices[i].position	= QUAD_DEFAULT_POSITIONT;
		m_Vertices[i].texCoord	= g_DefaultTexCoord[i];
	}

	// 2. On place v1, v2 et v3 grace � largeur/hauteur
	m_Vertices[1].position.x += m_Width;
	m_Vertices[2].position.x += m_Width;
	m_Vertices[2].position.y += m_Height;
	m_Vertices[3].position.y += m_Height;

	D3DXMATRIX world;
	WorldMatrix( &world ); // = Matrice de transformation de l'objet

	for( int i = 0; i < m_NbVertices; i++ )
	{
		// 3. On d�cale les points en fonction du point chaud
		m_Vertices[i].position.x -= m_HotPoint.x;
		m_Vertices[i].position.y -= m_HotPoint.y;

		// 4. On applique la transformation
		D3DXVec4Transform(
			&m_Vertices[i].position,
			&m_Vertices[i].position,
			&world
		);
	}

	// 5. Copie dans le vertex buffer
	CopyVerticesToVertexBuffer();
}

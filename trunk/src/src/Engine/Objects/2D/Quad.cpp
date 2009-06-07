#include	"Quad.h"

#include	"Renderer/Renderer.h"

//******************************************************************

#define		QUAD_VERTICES			4
#define		QUAD_DEFAULT_POSITIONT	(Vector4f( 0.f, 0.f, 0.f, 1.f ))

//******************************************************************

// Coordonnées de texture par défaut du quad
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
// Recalcule les sommets de l'objet. Cette méthode est
// appelée quand le dirty est activé.
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
	// 1. On réinitialise les sommets
	for( int i = 0; i < m_NbVertices; i++ )
	{
		m_Vertices[i].position	= QUAD_DEFAULT_POSITIONT;
		m_Vertices[i].texCoord	= g_DefaultTexCoord[i];
		m_Vertices[i].color		= m_Color;
	}

	// 2. On place v1, v2 et v3 grace à largeur/hauteur
	m_Vertices[1].position.x += m_Width;
	m_Vertices[2].position.x += m_Width;
	m_Vertices[2].position.y += m_Height;
	m_Vertices[3].position.y += m_Height;

	D3DXMATRIX world;
	WorldMatrix( &world ); // = Matrice de transformation de l'objet

	for( int i = 0; i < m_NbVertices; i++ )
	{
		// 3. On décale les points en fonction du point chaud
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

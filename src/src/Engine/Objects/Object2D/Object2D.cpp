#include	"Object2D.h"

#include	<assert.h>
#include	"Renderer/Renderer.h"

using namespace std;

//******************************************************************

#define		O2D_DEFAULT_WIDTH			(0)
#define		O2D_DEFAULT_HEIGHT			(0)
#define		O2D_DEFAULT_COLOR			(Color4f( 1.f, 1.f, 1.f, 1.f ))
#define		O2D_DEFAULT_POSITION		(Vector3f( 0.f, 0.f, 0.f ))
#define		O2D_DEFAULT_SCALE			(Vector3f( 1.f, 1.f, 1.f ))
#define		O2D_DEFAULT_ROTATION		(Vector3f( 0.f, 0.f, 0.f ))
#define		O2D_DEFAULT_PRIORITY		(0)

//******************************************************************

list< Object2D* > Object2D::RefList;

//******************************************************************

// Coordonn�es de texture par d�faut d'un quad
static Vector2f g_QuadTexCoord[4] =
{
	Vector2f( 0.f, 0.f ),
	Vector2f( 1.f, 0.f ),
	Vector2f( 1.f, 1.f ),
	Vector2f( 0.f, 1.f )
};

//******************************************************************

//**********************************************************
// Constructeur par d�faut.
//**********************************************************
Object2D::Object2D()
{
	m_Width				= O2D_DEFAULT_WIDTH;
	m_Height			= O2D_DEFAULT_HEIGHT;
	m_Color				= O2D_DEFAULT_COLOR;
	m_HotPoint			= HOTPOINT_V0;
	m_HotPointIndex		= 0;
	m_Priority			= O2D_DEFAULT_PRIORITY;
	m_IsHidden			= false;
	
	m_Position			= O2D_DEFAULT_POSITION;
	m_Scale				= O2D_DEFAULT_SCALE;
	m_Rotation			= O2D_DEFAULT_ROTATION;
	
	// Enregistrement dans la liste
	Object2D::RefList.push_back( this );
}

//**********************************************************
// Destructeur.
//**********************************************************
Object2D::~Object2D()
{
	// Suppression de la liste
	Object2D::RefList.remove( this );
}

//**********************************************************
// Update, appel�e � chaque tour moteur. A surcharger.
//**********************************************************
void Object2D::Update()
{
}

//**********************************************************
// Change la taille.
// @param[in]	width : largeur en pixels
// @param[in]	height : hauteur en pixels
//**********************************************************
void Object2D::SetSize( int width, int height )
{
	m_Width = width;
	m_Height = height;
}

//**********************************************************
// Change la taille.
// @param[in]	size : { largeur, hauteur }
//**********************************************************
void Object2D::SetSize( const Vector2f &size )
{
	SetSize( (int)size.x, (int)size.y );
}

//**********************************************************
// Donne la largeur de l'objet.
//**********************************************************
int Object2D::GetWidth() const
{
	return m_Width;
}

//**********************************************************
// Donne la hauteur de l'objet.
//**********************************************************
int Object2D::GetHeight() const
{
	return m_Height;
}

//**********************************************************
// Change la couleur.
// @param[in]	color : RGBA (0->1)
//**********************************************************
void Object2D::SetColor( const Color4f &color )
{
	m_Color = color;
}

//**********************************************************
// Donne la couleur.
// @return	la couleur RGBA
//**********************************************************
Color4f Object2D::GetColor() const
{
	return m_Color;
}

//**********************************************************
// Change l'alpha (= transparence).
// @param[in]	alpha : alpha (0->1)
//**********************************************************
void Object2D::SetAlpha( float alpha )
{
	m_Color.a = alpha;
}

//**********************************************************
// Donne l'alpha.
// @return	l'alpha (woohoo)
//**********************************************************
float Object2D::GetAlpha() const
{
	return m_Color.a;
}

//**********************************************************
// Change la priorit� (ordre d'affichage).
// @param[in]	priority : priorit� entre 0 et 255
//				0 : devant, 255 : derri�re
//**********************************************************
void Object2D::SetPriority( int priority )
{
	m_Priority = MATH_Clamp( priority, 0, 255 );
}

//**********************************************************
// Donne la priorit�.
// @return	la priorit�
//**********************************************************
int Object2D::GetPriority() const
{
	return m_Priority;
}

//**********************************************************
// Rend l'objet visible.
//**********************************************************
void Object2D::Show()
{
	m_IsHidden = false;
}

//**********************************************************
// Rend l'objet invisible (Draw n'est plus appel�e).
//**********************************************************
void Object2D::Hide()
{
	m_IsHidden = true;
}

//**********************************************************
// V�rifie si l'objet est cach�.
// @return	vrai si l'objet est cach�, faux sinon
//**********************************************************
bool Object2D::IsHidden() const
{
	return m_IsHidden;
}

//**********************************************************
// Change la position.
// @param[in]	posX : coordonn�e x (pixels)
// @param[in]	posY : coordonn�e y (pixels)
//**********************************************************
void Object2D::SetPosition( float posX, float posY )
{
	m_Position.x = posX;
	m_Position.y = posY;
}

//**********************************************************
// Change la position.
// @param[in]	position : (x,y)
//**********************************************************
void Object2D::SetPosition( const Point2f &position )
{
	SetPosition( position.x, position.y );
}

//**********************************************************
// Change le point chaud de l'objet.
// @param[in]	hotPoint : type de point chaud
// @param[in]	vertex : index du vertex (utile si hotPoint
//				= HOTPOINT_INDEX)
//**********************************************************
/*void Object2D::SetHotPoint( HotPoint hotPoint, int vertex )
{
	if( hotPoint == HOTPOINT_INDEX )
	{
		// En mode INDEX, on v�rifie que le vertex existe
		assert( vertex >= 0 && vertex < 4 );
		m_HotPointIndex = vertex;
	}
	m_HotPoint = hotPoint;
}*/

//**********************************************************
// Donne la position.
// @return	position du l'objet
//**********************************************************
Point2f Object2D::GetPosition() const
{
	return Vector2f( m_Position.x, m_Position.y );
}

//**********************************************************
// Change l'�chelle.
//**********************************************************
void Object2D::SetScale( float scaleX, float scaleY )
{
	m_Scale.x = scaleX;
	m_Scale.y = scaleY;
}

//**********************************************************
// Change l'�chelle.
//**********************************************************
void Object2D::SetScale( const Vector2f &scale )
{
	SetScale( scale.x, scale.y );
}

//**********************************************************
// Donne l'�chelle.
//**********************************************************
Vector2f Object2D::GetScale() const
{
	return Vector2f( m_Scale.x, m_Scale.y );
}

//**********************************************************
// Change l'angle de rotation. La rotation s'effectue autour
// du point chaud de l'objet.
// @param[in]	angle : angle de rotation (radian)
//**********************************************************
void Object2D::SetRotation( float angle )
{
	m_Rotation.z = angle;
}

//**********************************************************
// Donne l'angle de rotation.
//**********************************************************
float Object2D::GetRotation() const
{
	return m_Rotation.z;
}

//**********************************************************
// Update tous les objets 2D.
//**********************************************************
void Object2D::UpdateAll()
{
	list< Object2D* >::iterator it = RefList.begin();
	
	while( it != RefList.end() )
	{
		(*it)->Update();
		++it;
	}
}

//**********************************************************
// Compare les priorit�s des objets. Permet de trier les
// objets par ordre de priorit� croissante (pour l'affichage).
//**********************************************************
bool Object2D::ComparePriority( const Object2D *o1, const Object2D *o2 )
{
	// o1 est avant o2 si sa priorit� est plus faible, la
	// priorit� la plus faible �tant 255, la plus forte 0.
	return o1->m_Priority > o2->m_Priority;
}

//**********************************************************
// Calcule la matrice de l'objet dans le rep�re world.
// @param[out]	matrix : matrice world de l'objet
//**********************************************************
void Object2D::WorldMatrix( D3DMATRIX *matrix ) const
{
	D3DXMATRIX trans, rotat, scale;
	D3DXMatrixTranslation( &trans, m_Position.x, m_Position.y, m_Position.z );
	D3DXMatrixRotationYawPitchRoll( &rotat, m_Rotation.x, m_Rotation.y, m_Rotation.z );
	D3DXMatrixScaling( &scale, m_Scale.x, m_Scale.y, m_Scale.z );
	*matrix = scale * rotat * trans;
}

//**********************************************************
// G�n�re les points d'un quad � partir des donn�es de
// l'objet. Les points sont organis�s comme sur le sch�ma
// ci-dessous.
//
//  v0 +--------+ v1
//     |        |
//     |        |
//     |        |
//  v3 +--------+ v2
//
// @param[out]	vertices : tableau de 4 Object2D::Vertex
//**********************************************************
void Object2D::GenQuad( Vertex *vertices ) const
{
	D3DXMATRIX world;
	WorldMatrix( &world );
	
	// Initialisation des points
	for( int vertex = 0; vertex < 4; vertex++ )
	{
		vertices[vertex].position	= Vector4f( 0.f, 0.f, 0.f, 1.f );
		vertices[vertex].color		= m_Color;
		vertices[vertex].texCoord	= g_QuadTexCoord[vertex];
	}

	// Les coordonn�es d�pendent du point chaud..
	switch( m_HotPoint )
	{
		case HOTPOINT_V0:
			vertices[1].position.x	+= m_Width;
			vertices[2].position.x	+= m_Width;
			vertices[2].position.y	+= m_Height;
			vertices[3].position.y	+= m_Height;
			break;

		default:
			// Aucun autre cas ne devrait �tre possible
			assert( false );
	}

	// On applique la transformation
	for( int vertex = 0; vertex < 4; vertex++ )
	{
		D3DXVec4Transform(
			&vertices[vertex].position,
			&vertices[vertex].position,
			&world
		);
	}
}

//**********************************************************
// G�n�re la vertex declaration correspondante � la
// structure du vertex.
// @param[out]	vElements : tableau de D3DVERTEXELEMENT9
//**********************************************************
void Object2D::Vertex::GenDeclaration( vector<D3DVERTEXELEMENT9> *vElements )
{
	D3DVERTEXELEMENT9 element;
	int offset = 0;

	// Propri�t�s communes
	element.Stream		= 0;
	element.Method		= D3DDECLMETHOD_DEFAULT;
	element.UsageIndex	= 0;

	// POSITION (xyzrhw)
	element.Offset	= offset;
	element.Type	= D3DDECLTYPE_FLOAT4;
	element.Usage	= D3DDECLUSAGE_POSITION;
	offset			+= sizeof(Vector4f);
	vElements		->push_back( element );

	// COLOR (rgba)
	element.Offset	= offset;
	element.Type	= D3DDECLTYPE_FLOAT4;
	element.Usage	= D3DDECLUSAGE_COLOR;
	offset			+= sizeof(Color4f);
	vElements		->push_back( element );

	// TEXCOORD (uv)
	element.Offset	= offset;
	element.Type	= D3DDECLTYPE_FLOAT2;
	element.Usage	= D3DDECLUSAGE_TEXCOORD;
	offset			+= sizeof(Vector2f);
	vElements		->push_back( element );

	// Element de fin
	D3DVERTEXELEMENT9 endElement = D3DDECL_END();
	vElements->push_back( endElement );
}

#include	"Object2D.h"

using namespace std;

//******************************************************************
// Valeurs d'initialisation par défaut :

#define		O2D_DEFAULT_SIZE	(Vector3f( 0.f, 0.f, 0.f ))
#define		O2D_DEFAULT_POS		(Vector3f( 0.f, 0.f, 0.f ))
#define		O2D_DEFAULT_SCALE	(Vector3f( 1.f, 1.f, 1.f ))
#define		O2D_DEFAULT_COLOR	(Color4f( 1.f, 1.f, 1.f, 1.f ))

//******************************************************************

list< Object2D* > Object2D::RefList;

//******************************************************************

//**********************************************************
// Constructeur par défaut.
//**********************************************************
Object2D::Object2D()
{
	m_Size		= O2D_DEFAULT_SIZE;
	m_Position	= O2D_DEFAULT_POS;
	m_Scale		= O2D_DEFAULT_SCALE;
	m_Color		= O2D_DEFAULT_COLOR;
	m_IsVisible	= true;
	
	// Enregistrement dans la liste
	Object2D::RefList.push_front( this );
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
// Update, appelée à chaque tour moteur. A surcharger.
//**********************************************************
void Object2D::Update()
{
}

//**********************************************************
// Change la position.
// @param[in]	posX : coordonnée x
// @param[in]	posY : coordonnée y
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
// Donne la position.
// @return	position du l'objet
//**********************************************************
Point2f Object2D::GetPosition() const
{
	return Vector2f( m_Position.x, m_Position.y );
}

//**********************************************************
// Change l'échelle.
//**********************************************************
void Object2D::SetScale( float scaleX, float scaleY )
{
	m_Scale.x = scaleX;
	m_Scale.y = scaleY;
}

//**********************************************************
// Change l'échelle.
//**********************************************************
void Object2D::SetScale( const Vector2f &scale )
{
	SetScale( scale.x, scale.y );
}

//**********************************************************
// Donne l'échelle.
//**********************************************************
Vector2f Object2D::GetScale() const
{
	return Vector2f( m_Scale.x, m_Scale.y );
}

//**********************************************************
// Change l'alpha (= transparence).
// @param[in]	alpha : alpha (0->1)
//**********************************************************
void Object2D::SetAlpha( float alpha )
{
	m_Color.a = MATH_Clamp( alpha, 0.f, 1.f );
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
// Rend visible/invisible l'objet.
// @param[in]	isVisible : visible si vrai
//**********************************************************
void Object2D::SetVisibility( bool isVisible )
{
	m_IsVisible = isVisible;
}

//**********************************************************
// Vérifie si l'objet est visible.
// @return	vrai si l'objet est visible, faux sinon
//**********************************************************
bool Object2D::IsVisible() const
{
	return m_IsVisible;
}

//**********************************************************
// Calcule la matrice de l'objet dans le repère world.
// @param[out]	matrix : matrice world de l'objet
//**********************************************************
void Object2D::WorldMatrix( D3DMATRIX *matrix ) const
{
	D3DXMATRIX rotat, trans, scale;
	D3DXMatrixIdentity( &rotat ); //-- Temp, pas de rotation pour le moment
	D3DXMatrixTranslation( &trans, m_Position.x, m_Position.y, m_Position.z );
	D3DXMatrixScaling( &scale, m_Scale.x, m_Scale.y, m_Scale.z );
	*matrix = scale * trans * rotat;
}

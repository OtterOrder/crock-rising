#include	"Object2D.h"

using namespace std;

//******************************************************************

#define		OBJECT2D_DEFAULT_Z		0.f		// Profondeur par défaut

//******************************************************************

list< Object2D* > Object2D::RefList;

//******************************************************************

/***********************************************************
 * Initialisation commune aux constructeurs.
 **********************************************************/
void Object2D::CommonInit()
{
	m_Position.z	= OBJECT2D_DEFAULT_Z;
	m_IsVisible		= true;
	
	Object2D::RefList.push_front( this ); // enregistrement dans la liste
}

/***********************************************************
 * Constructeur.
 **********************************************************/
Object2D::Object2D()
{
	CommonInit();
}

/***********************************************************
 * Destructeur.
 **********************************************************/
Object2D::~Object2D()
{
	Object2D::RefList.remove( this ); // suppression dans la liste
}

/***********************************************************
 * Update, appelée à chaque tour moteur.
 **********************************************************/
void Object2D::Update()
{
}

/***********************************************************
 * Change la position.
 * @param[in]	posX : coordonnée x
 * @param[in]	posY : coordonnée y
 **********************************************************/
void Object2D::SetPosition( float posX, float posY )
{
	m_Position.x = posX;
	m_Position.y = posY;
}

/***********************************************************
 * Change la position.
 * @param[in]	position : (x,y)
 **********************************************************/
void Object2D::SetPosition( const Point2f &position )
{
	m_Position.x = position.x;
	m_Position.y = position.y;
}

/***********************************************************
 * Donne la position.
 * @return	position du l'objet
 **********************************************************/
Point2f Object2D::GetPosition() const
{
	return Vector2f( m_Position.x, m_Position.y );
}

/***********************************************************
 * Rend visible/invisible l'objet.
 * @param[in]	isVisible : visible si vrai
 **********************************************************/
void Object2D::SetVisibility( bool isVisible )
{
	m_IsVisible = isVisible;
}

/***********************************************************
 * Vérifie si l'objet est visible.
 * @return	vrai si l'objet est visible, faux sinon
 **********************************************************/
bool Object2D::IsVisible() const
{
	return m_IsVisible;
}

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
	RefList.push_front( this ); // enregistrement dans la liste
}

/***********************************************************
 * Constructeur.
 **********************************************************/
Object2D::Object2D()
{
	CommonInit();
	m_Position.z = OBJECT2D_DEFAULT_Z;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
Object2D::~Object2D()
{
	RefList.remove( this ); // suppression dans la liste
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
 * Change la profondeur.
 * @param[in]	depth : profondeur = coordonnée z
 **********************************************************/
void Object2D::SetDepth( float depth )
{
	m_Position.z = depth;
}

/***********************************************************
 * Donne la profondeur.
 * @return	profondeur = coordonnée z de l'objet
 **********************************************************/
float Object2D::GetDepth()
{
	return m_Position.z;
}

/***********************************************************
 * Change la position 3D.
 * @param[in]	posX : coordonnée x
 * @param[in]	posY : coordonnée y
 * @param[in]	posZ : coordonnée z
 **********************************************************/
void Object2D::Set3DPosition( float posX, float posY, float posZ )
{
	m_Position.x = posX;
	m_Position.y = posY;
	m_Position.z = posZ;
}

/***********************************************************
 * Change la position 3D.
 * @param[in]	position : (x,y,z)
 **********************************************************/
void Object2D::Set3DPosition( const Point3f &position )
{
	m_Position = position;
}

/***********************************************************
 * Donne la position 3D.
 * @return	position 3D du l'objet
 **********************************************************/
Point3f Object2D::Get3DPosition() const
{
	return m_Position;
}

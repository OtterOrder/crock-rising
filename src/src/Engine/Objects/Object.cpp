#include "Object.h"

using namespace std;

//******************************************************************

list< Object* > Object::RefList;

//******************************************************************

/***********************************************************
 * Initialisation commune à tous les constructeurs.
 **********************************************************/
void Object::CommonInit( void )
{
	D3DXMatrixIdentity( &m_WorldMatrix );
	Object::RefList.push_front( this ); // enregistrement dans la liste
}

Object::Object( void )
{
	CommonInit();

	m_vPosition = Vector3f( 0.f, 0.f, 0.f );
	m_vAngleX = m_vAngleY = m_vAngleZ = 0;
}

Object::Object( float initPosX, float initPosY, float initPosZ )
{
	CommonInit();

	m_vPosition = Vector3f( initPosX, initPosY, initPosZ );
	m_vAngleX = m_vAngleY = m_vAngleZ = 0;
}

Object::Object( D3DXVECTOR3 pos )
{
	CommonInit();

	m_vPosition = pos;
	m_vAngleX = m_vAngleY = m_vAngleZ = 0;

}

Object::~Object( void )
{
	Object::RefList.remove( this ); // suppression dans la liste
}

/***********************************************************
 * Méthode appelée chaque tour moteur.
 **********************************************************/
void Object::Update()
{
	D3DXMATRIX result, rotX, rotY, rotZ, translation;

	D3DXMatrixRotationX( &rotX, D3DXToRadian( m_vAngleX ) );
	D3DXMatrixRotationY( &rotY, D3DXToRadian( m_vAngleY ) );
	D3DXMatrixRotationZ( &rotZ, D3DXToRadian( m_vAngleZ ) );

	D3DXMatrixTranslation( &translation, m_vPosition.x, m_vPosition.y, m_vPosition.z );

	// Si on met la matrice à identité ici on reset n'importe quelle transformation qui n'est pas effectuée avec
	// rotX, rotY, rotZ, vPosition etc...
	D3DXMatrixIdentity( &m_WorldMatrix );
	
	m_WorldMatrix._22 = 0.f;  m_WorldMatrix._23 = 1.f;
	m_WorldMatrix._32 = 1.f;  m_WorldMatrix._33 = 0.f;

	m_WorldMatrix *= (rotY * translation);
	
}

void Object::SetTranslation( float x, float y, float z )
{
	m_vPosition.x += x;
	m_vPosition.y += y;
	m_vPosition.z += z;
}

void Object::SetRotation( int angleX, int angleY, int angleZ )
{
	m_vAngleX += angleX;
	m_vAngleY += angleY;
	m_vAngleZ += angleZ;

	if( abs(m_vAngleX) > 359 ) m_vAngleX = 0;
	if( abs(m_vAngleY) > 359 ) m_vAngleY = 0;
	if( abs(m_vAngleZ) > 359 ) m_vAngleZ = 0;
}
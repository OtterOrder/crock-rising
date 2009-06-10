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

	//m_vPosition = Vector3f( 0.f, 0.f, 0.f );
	//m_vAngleX = m_vAngleY = m_vAngleZ = 0;
}

Object::Object( float initPosX, float initPosY, float initPosZ )
{
	CommonInit();

	//m_vPosition = Vector3f( initPosX, initPosY, initPosZ );
	//m_vAngleX = m_vAngleY = m_vAngleZ = 0;
}

Object::Object( D3DXVECTOR3 pos )
{
	CommonInit();

	//m_vPosition = pos;
	//m_vAngleX = m_vAngleY = m_vAngleZ = 0;

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

}

void Object::SetTranslation( float x, float y, float z )
{
	D3DXMATRIX translation;
	D3DXMatrixTranslation( &translation, x, y, z);
	SetTransform(&translation);
}

void Object::SetRotation( int angleX, int angleY, int angleZ )
{
	D3DXMATRIX rotX, rotY, rotZ;
	D3DXMatrixRotationX(&rotX,  D3DXToRadian(angleX));
	D3DXMatrixRotationY(&rotY,  D3DXToRadian(angleZ));
	D3DXMatrixRotationZ(&rotZ,  D3DXToRadian(angleY));

	D3DXMATRIX result=rotX*rotY*rotZ;
	ApplyTransform(&result);
}

Vector3f Object::GetPosition()
{ 
	return Vector3f(m_WorldMatrix(4, 1), m_WorldMatrix(4, 2), m_WorldMatrix(4, 3));

}

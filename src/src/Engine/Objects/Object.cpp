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

	m_vAngleX = m_vAngleY = m_vAngleZ = 0;
}

Object::Object( float initPosX, float initPosY, float initPosZ )
{
	CommonInit();

	m_vAngleX = m_vAngleY = m_vAngleZ = 0;
}

Object::Object( D3DXVECTOR3 pos )
{
	CommonInit();

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

}

void Object::SetTranslation( float x, float y, float z )
{
	D3DXMATRIX translation;
	D3DXMatrixTranslation( &translation, x, y, z);
	SetTransform(&translation);
}

void Object::SetRotation( int angleX, int angleY, int angleZ )
{
	m_vAngleX+=angleX;
	m_vAngleY+=angleY;
	m_vAngleZ+=angleZ;
	if(m_vAngleX > 360.f) m_vAngleX=0.f;
	if(m_vAngleY > 360.f) m_vAngleY=0.f;
	if(m_vAngleZ > 360.f) m_vAngleZ=0.f;
	D3DXMATRIX rotX, rotY, rotZ;
	D3DXMatrixRotationX(&rotX,  D3DXToRadian(angleX));
	D3DXMatrixRotationY(&rotY,  D3DXToRadian(angleY));
	D3DXMatrixRotationZ(&rotZ,  D3DXToRadian(angleZ));

	D3DXMATRIX result=rotX*rotY*rotZ;
	ApplyTransform(&result);
}

Vector3f Object::GetPosition()
{ 
	float x=m_WorldMatrix._41;
	float y=m_WorldMatrix._42;
	float z=m_WorldMatrix._43;
	return Vector3f(x, y, z);

}

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
	RefList.push_front( this ); // enregistrement dans la liste
}

Object::Object( void )
{
	CommonInit();
}

Object::Object( float initPosX, float initPosY, float initPosZ )
{
	CommonInit();

	m_WorldMatrix._41 = initPosX;
	m_WorldMatrix._42 = initPosY;
	m_WorldMatrix._43 = initPosZ;
}

Object::Object( D3DXVECTOR3 pos )
{
	CommonInit();

	m_WorldMatrix._41 = pos.x;
	m_WorldMatrix._42 = pos.y;
	m_WorldMatrix._43 = pos.z;
}

Object::~Object( void )
{
	RefList.remove( this ); // suppression dans la liste
}

/***********************************************************
 * Méthode appelée chaque tour moteur.
 **********************************************************/
void Object::Update()
{
}

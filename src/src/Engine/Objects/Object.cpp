//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Object.h"

//===========================================================================//
// Constructeurs Object							                             //
//===========================================================================//
Object::Object( void )
{
	D3DXMatrixIdentity( &m_WorldMatrix );
}

Object::Object( float initPosX, float initPosY, float initPosZ )
{
	D3DXMatrixIdentity( &m_WorldMatrix );
	m_WorldMatrix._41 = initPosX;
	m_WorldMatrix._42 = initPosY;
	m_WorldMatrix._43 = initPosZ;

}

Object::Object( D3DXVECTOR3 pos )
{
	D3DXMatrixIdentity( &m_WorldMatrix );
	m_WorldMatrix._41 = pos.x;
	m_WorldMatrix._42 = pos.y;
	m_WorldMatrix._43 = pos.z;
}

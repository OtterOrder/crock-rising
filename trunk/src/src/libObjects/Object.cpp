#include "Object.h"

//******************************************************************

Object::Object( void ){
	m_Position = Vector3f( 0.0f, 0.0f, 0.0f );
}

Object::Object( float initPosX, float initPosY, float initPosZ ){
	m_Position.x = initPosX;
	m_Position.y = initPosY;
	m_Position.z = initPosZ;
}

Object::Object( Vector3f pos ){
	SetPosition( pos );
}

Vector3f Object::GetPosition(){
	return m_Position;
}

void	 Object::SetPosition( Vector3f position ){
	m_Position = position;
}
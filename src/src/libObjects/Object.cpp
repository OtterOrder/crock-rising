#include "Object.h"

//******************************************************************

Object::Object( void ){
	m_Position.x = m_Position.y = m_Position.z = 0.0f;
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
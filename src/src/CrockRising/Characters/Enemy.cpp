#include "Enemy.h"

Enemy::Enemy()
{
	m_iLife = 100 ;

	m_pAnimated = new SceneObjectAnimated("Alien_Mesh.DAE","Alien_Anim.DAE",m_position);
	m_pAnimated->InitObject();
	m_pAnimated->InitDeviceData();
}

Enemy::~Enemy()
{
	delete m_pAnimated;
}

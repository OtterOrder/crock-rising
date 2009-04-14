#include "Enemy.h"

Enemy::Enemy()
{
	m_iLife = 100 ;
	m_position = D3DXVECTOR3(10.f,-17.f,0.f);
	m_direction = D3DXVECTOR3(0.f,0.f,0.f);

	m_pAnimated = new SceneObjectAnimated("Alien_Mesh.DAE","Alien_Anim.DAE",m_position);
	m_pAnimated->InitObject();
	m_pAnimated->InitDeviceData();
}

Enemy::~Enemy()
{
	delete m_pAnimated;
}

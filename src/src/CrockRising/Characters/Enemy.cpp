#include "Enemy.h"

Enemy::Enemy()
{
	m_iLife = 100 ;

	m_pAnimated = new SceneObjectAnimated("Alien_Mesh.DAE","Alien_Anim.DAE",D3DXVECTOR3(0.f,-17.f,0.f));
	m_pAnimated->InitObject();
	m_pAnimated->InitDeviceData();
}

Enemy::~Enemy()
{
	delete m_pAnimated;
}

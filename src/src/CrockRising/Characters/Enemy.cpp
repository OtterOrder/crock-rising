#include "Enemy.h"

/***********************************************************/
std::vector< Enemy* > Enemy::listEnemy;

/***********************************************************
* Constructeur
***********************************************************/
Enemy::Enemy()
{
	m_iLife = 100 ;

	m_pAnimated = new SceneObjectAnimated("Alien_Mesh.DAE","Alien_Anim.DAE",D3DXVECTOR3(0.f,-17.f,0.f));
	m_pAnimated->Init();
	
	Enemy::listEnemy.push_back(this);
}

/***********************************************************
* Destructeur
***********************************************************/
Enemy::~Enemy()
{
	delete m_pAnimated;


	std::vector<Enemy*>::iterator it;
	it = listEnemy.begin();
	while ( it != listEnemy.end() )
	{
		listEnemy.erase(it);
		++it;
	}
}

#include "Enemy.h"

/***********************************************************/
std::vector< Enemy* > Enemy::listEnemy;

/***********************************************************
* Constructeur
***********************************************************/
Enemy::Enemy(Vector3f position)
{
	m_iLife = 100 ;
	m_pAnimated = NULL;
	m_position = position;
	m_currentState = RUN;
	m_pInputManager = InputManager::GetInstance();

	Enemy::listEnemy.push_back(this);
}

/***********************************************************
* Destructeur
***********************************************************/
Enemy::~Enemy()
{
	std::vector<Enemy*>::iterator it;
	it = listEnemy.begin();
	while ( it != listEnemy.end() )
	{
		listEnemy.erase(it);
		++it;
	}
}


/***********************************************************************/
void Enemy::update()
{

	if ( m_pInputManager->IsKeyPressed('C'))
	{
		changeState(ATTACK);
	}
	if ( m_pInputManager->IsKeyPressed('V'))
	{
		changeState(DIE);
	}
	if ( m_pInputManager->IsKeyPressed('B'))
	{
		changeState(RUN);
	}
}

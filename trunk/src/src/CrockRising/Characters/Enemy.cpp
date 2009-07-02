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
	timeSinceLastPath = 0.f;

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
void Enemy::update( Enemy* list )
{
	if(m_currentState == DIE && m_pAnimated->IsAtEnd())
		DestroyPerso();
	

	//Sync arme sur la main du Hero
	D3DXMATRIX* armeMatrix = m_pArme->GetWorldMatrix();
	D3DXMATRIX animMatrix = m_pAnimated->GetMatrixTransformBone( m_idBone );
	*armeMatrix = animMatrix; //modificiation obj visuel
	m_pArme->SetPosition( animMatrix._41, animMatrix._42, animMatrix._43 );
}

void Enemy::Hit()
{
	changeState(HIT);
	decLife( 50 );
}

void Enemy::Die()
{
	changeState(DIE);	
	FreezeState();
}
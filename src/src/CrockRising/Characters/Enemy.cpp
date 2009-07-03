#include "Enemy.h"

#include	<Game/Game.h>

#define		LEVEL_victory			0x70bb005a
/***********************************************************/
std::vector< Enemy* > Enemy::listEnemy;
int Enemy::nbEnemy = 0;

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
	nbEnemy++;
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
		delete (*it)->getSceneObjectAnimated();
		listEnemy.erase(it);
		++it;
	}
}


/***********************************************************************/
void Enemy::update( Enemy* list )
{
	if(m_currentState == DIE && m_pAnimated->IsAtEnd())
	{
		DestroyPerso();
		if( nbEnemy <= 0 )
		{
			nbEnemy = 0;
			Game::GetInstance()->ChangeLevel( LEVEL_victory );
		}

	}
	

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
	nbEnemy--;
}
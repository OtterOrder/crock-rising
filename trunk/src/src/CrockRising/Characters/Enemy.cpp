#include "Enemy.h"

#include	<Game/Game.h>

#define		LEVEL_victory			0x70bb005a
/***********************************************************/
std::list< Enemy* > Enemy::RefList;
int Enemy::nbEnemy = 0;

/***********************************************************
* Constructeur
***********************************************************/
Enemy::Enemy(Vector3f pos)
: Perso( pos )
{
	m_iLife = 100 ;
	m_pAnimated = NULL;
	m_currentState = RUN;
	timeSinceLastPath = 0.f;

	Enemy::RefList.push_back(this);
	nbEnemy++;
}


/***********************************************************************/
void Enemy::update( )
{
	Perso::update();

	if(m_currentState == DIE && m_pAnimated->IsAtEnd())
		DestroyPerso();
	

}

void Enemy::Hit()
{
	changeState(HIT);
	decLife( 50 );
}

void Enemy::Die()
{
	changeState(DIE);	
	nbEnemy--;
}

void Enemy::DestroyPerso()
{
	Perso::DestroyPerso();
	Enemy::RefList.remove( this );
	if( Enemy::RefList.empty() ) Game::GetInstance()->ChangeLevel( LEVEL_victory );
}
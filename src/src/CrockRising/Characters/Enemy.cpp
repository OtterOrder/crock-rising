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

Enemy::Enemy(Vector3f position)
{
	m_iLife = 100 ;

	m_pAnimated = new SceneObjectAnimated("Alien_Mesh.DAE","Alien_Anim.DAE",position);
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

void Enemy::Init()
{

}

/********************************************************************
* En fonction du nouvel état, cette méthode configure les nouvelles 
* animations à lancer 
*********************************************************************/
void Enemy::changeState( PersoState newState )
{
	//if ( (m_currentState != STATIC && !m_pAnimated->IsAtEnd())
	//	 || newState == m_currentState )
	//	return;

	//m_currentState = newState;

	//switch ( m_currentState )
	//{
	//case WALK :
	//	break;
	//case RUN :
	//	m_pAnimated->SetAnim("Anim_Robot_Run.DAE");
	//	m_pAnimated->Play();
	//	m_pAnimated->SetLoop(true);
	//	m_pAnimated->SetAnimFPS(50.f);
	//	break;
	//case ATTACK : 
	//	//m_pAnimated->SetAnim("Anim_Robot_Attack.DAE");
	//	//m_pAnimated->Play();
	//	//m_pAnimated->SetLoop(true);
	//	//m_pAnimated->SetAnimFPS(50.f);
	//	break;
	//case STATIC :
	//	m_pAnimated->Stop();
	//	break;
	//}
}

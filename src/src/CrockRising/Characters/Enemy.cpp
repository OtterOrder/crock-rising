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

/**********************************************************
* Initialisation des données membres
**********************************************************/
void Enemy::Init()
{
	m_pAnimated = new SceneObjectAnimated("Alien_Mesh_2.DAE","Anim_Alien_Walk.DAE",m_position);
	m_pAnimated->Init();

	m_pAnimated->GetMaterial()->SetTexture("alien.jpg",Texture::DIFFUSE);
	m_pAnimated->GetMaterial()->SetTexture("alien_normal.png",Texture::NORMALMAP);
	m_pAnimated->SetShader("default_skinnormalmap.fx");

	m_pAnimated->SetLoop(false);
	m_pAnimated->SetAnimFPS(50.f);
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

/********************************************************************
* En fonction du nouvel état, cette méthode configure les nouvelles 
* animations à lancer 
*********************************************************************/
void Enemy::changeState( PersoState newState )
{
	if ( !m_pAnimated->IsAtEnd() )
		return;

	m_currentState = newState;

	switch ( m_currentState )
	{
	case RUN :
		m_pAnimated->SetAnim("Anim_Alien_Walk.DAE");
		m_pAnimated->Play();
		m_pAnimated->SetLoop(true);
		m_pAnimated->SetAnimFPS(50.f);
		break;
	case ATTACK : 
		m_pAnimated->SetAnim("Anim_Alien_Attack.DAE");
		m_pAnimated->Play();
		m_pAnimated->SetLoop(false);
		m_pAnimated->SetAnimFPS(50.f);
		break;
	case DIE :
		m_pAnimated->SetAnim("Anim_Alien_Die.DAE");
		m_pAnimated->Play();
		m_pAnimated->SetLoop(true);
		m_pAnimated->SetAnimFPS(30.f);
		break;
	case STATIC : 
		m_pAnimated->Stop();
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

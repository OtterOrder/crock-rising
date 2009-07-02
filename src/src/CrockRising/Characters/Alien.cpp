#include "Alien.h"

/***********************************************************
* Constructeur
***********************************************************/
Alien::Alien(Vector3f position)
: Enemy(position)
{
}

/**********************************************************
* Initialisation des données membres
**********************************************************/
void Alien::Init()
{
	m_pAnimated = new SceneObjectAnimated("Mesh_Alien.DAE","Anim_Alien_Walk.DAE",m_position);
	m_pAnimated->Init();

	m_pAnimated->GetMaterial()->SetTexture("alien.jpg",Texture::DIFFUSE);
	m_pAnimated->GetMaterial()->SetTexture("alien_normal.png",Texture::NORMALMAP);
	m_pAnimated->SetShader("default_skinnormalmap.fx");

	m_pAnimated->SetLoop(true);
	m_pAnimated->SetAnimFPS(50.f);
}


/********************************************************************
* En fonction du nouvel état, cette méthode configure les nouvelles 
* animations à lancer 
*********************************************************************/
void Alien::changeState( PersoState newState )
{
	if ( !m_pAnimated->IsAtEnd() && newState != HIT && newState != DIE)
		return;

	if( !m_StateFrozen )
	{
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
			m_pAnimated->SetLoop(false);
			m_pAnimated->SetAnimFPS(30.f);
			break;
		case HIT :
			m_pAnimated->SetAnim("Anim_Alien_Hit.DAE");
			m_pAnimated->Play();
			m_pAnimated->SetLoop(false);
			m_pAnimated->SetAnimFPS(30.f);
			break;
		case STATIC : 
			m_pAnimated->Stop();
		}
	}
}
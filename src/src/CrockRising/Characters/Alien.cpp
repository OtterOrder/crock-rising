#include "Alien.h"

/***********************************************************
* Constructeur
***********************************************************/
Alien::Alien(Vector3f position)
: Enemy( position )
{
	m_idBone = 22;
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
	
	m_pArme = new SceneObject( "dummy.dae", Vector3f( 5, 18, 0)); //en fn de la pos de m_pAnimated
	m_pArme->Init();
	m_pArme->SetObjectPhysical( "arme_mmegrise.DAE" ); //pas le bon group!!

	NxActor* a = physX::getActor( m_pArme->getEmpActor() );
	if( a )
	{
		a->getShapes()[0]->setGroup( GROUP_CONTROLLER );
		a->raiseBodyFlag( NX_BF_DISABLE_GRAVITY );
		a->raiseBodyFlag( NX_BF_FROZEN_ROT_X );
		a->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );
		a->raiseBodyFlag( NX_BF_FROZEN_ROT_Z );
		a->userData = new ActorUserData;
		((ActorUserData*)a->userData)->type = WEAPON;
		((ActorUserData*)a->userData)->PersoRef = this;
	}
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
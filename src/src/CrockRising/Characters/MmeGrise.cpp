#include "MmeGrise.h"
#include "../AIToolkit/AIManager.h"

/***********************************************************
* Constructeur
***********************************************************/
MmeGrise::MmeGrise(Vector3f position)
: Enemy( position )
{
	m_idBone = 14;
}

/**********************************************************
* Initialisation des données membres
**********************************************************/
void MmeGrise::Init()
{
	m_pAnimated = new SceneObjectAnimated("Mesh_Mmegrise.DAE","Anim_Mmegrise_Walk.DAE",m_position);
	m_pAnimated->Init();
	m_pAnimated->GetMaterial()->SetTexture("mmegrise_diffuse.jpg",Texture::DIFFUSE);

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
void MmeGrise::changeState( PersoState newState )
{
	if ( !m_pAnimated->IsAtEnd() && newState != HIT && newState != DIE)
		return;

	if( !m_StateFrozen )
	{
		m_currentState = newState;

		switch ( m_currentState )
		{
		case RUN :
			m_pAnimated->SetAnim("Anim_Mmegrise_Walk.DAE");
			m_pAnimated->Play();
			m_pAnimated->SetLoop(true);
			m_pAnimated->SetAnimFPS(50.f);
			break;
		case ATTACK : 
			m_pAnimated->SetAnim("Anim_Mmegrise_Attack.DAE");
			m_pAnimated->Play();
			m_pAnimated->SetLoop(false);
			m_pAnimated->SetAnimFPS(50.f);
			break;
		case DIE :
			m_pAnimated->SetAnim("Anim_Mmegrise_Die.DAE");
			m_pAnimated->Play();
			m_pAnimated->SetLoop(false);
			m_pAnimated->SetAnimFPS(30.f);
			break;
		case HIT :
			m_pAnimated->SetAnim("Anim_Mmegrise_Hit.DAE");
			m_pAnimated->SetAnimFPS(30);
			m_pAnimated->Play();
			m_pAnimated->SetLoop(false);
			break;
		case STATIC : 
			m_pAnimated->Stop();
		}
	}	
}
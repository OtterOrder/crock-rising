#include "Perso.h"
#include "Hero.h"
#include <iostream>
#include "Core/Macros.h"
#include "Sound/SoundSystem.h"
#include <NxQuat.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////
Perso::Perso(Vector3f pos)
: m_pAnimated(NULL), m_pArme(NULL),
	m_iLife(100), m_idBone(-1), m_currentState(STATIC),
	m_Position(pos)
{
}

//////////////////////////////////////////////////////////////////////////
Perso::~Perso()
{
	if(m_pAnimated) 
	{
		SceneObject::RefList.remove( m_pAnimated );
		delete m_pAnimated;
		m_pAnimated = NULL;
	}

	if(m_pArme) 
	{
		SceneObject::RefList.remove( m_pArme );
		delete m_pArme;
		m_pArme = NULL;
	}
	ReleaseSounds();
}

//////////////////////////////////////////////////////////////////////////
NxQuat MatrixToQuat( const D3DXMATRIX mat )
{
	NxMat33 Nxmat(	NxVec3( mat._11, mat._12, mat._13),
					NxVec3( mat._21, mat._22, mat._23),
					NxVec3( mat._31, mat._32, mat._33));

	//Nxmat.setTransposed();

	return NxQuat( Nxmat );
}

//////////////////////////////////////////////////////////////////////////
void Perso::decLife( const int decreaseLife ) 
{	 
	if ( (m_iLife - decreaseLife) <= 0 )
		Die();
	else 
		m_iLife -= decreaseLife;
}

//////////////////////////////////////////////////////////////////////////
void Perso::DestroyPerso()
{
	SceneObject::RefList.remove(m_pAnimated);
 	delete m_pAnimated;
	m_pAnimated = NULL;
	ReleaseSounds();
}


//////////////////////////////////////////////////////////////////////////
void Perso::changeState( State newState )
{
	//On ne change pas d'état si le perso meurt.
 	if(m_currentState == DIE)
		return;
	//On ne change pas si l'anim est en cours
	if( !m_pAnimated->IsAtEnd() &&
		//Situation pour casser l'anim :
		( newState == STATIC 
		|| (m_currentState == RUN && newState != ATTACK && newState != HIT && newState != DIE ))
		|| (m_currentState == ATTACK && newState != STATIC && newState != HIT && newState != DIE && newState != RUN ))
		return;

	//On ne change pas d'état si l'anim est pas finie
	//if(!m_pAnimated->IsAtEnd() && m_currentState != STATIC
	//	&& newState != HIT 
	//	&& newState != DIE )
	//	return;

	m_currentState = newState;

	switch ( m_currentState )
	{
	case RUN :		Animate( RUN);
		break;
	case ATTACK :	Animate( ATTACK );
		break;
	case DIE :		Animate( DIE );
		break;
	case HIT :		Animate( HIT );
		break;
	case STATIC :	Animate( STATIC);
	}
}

//////////////////////////////////////////////////////////////////////////
void Perso::update()
{
	PlayPersoSound( m_currentState );
	if(m_idBone >= 0)
	{
		D3DXMATRIX* armeMatrix = m_pArme->GetWorldMatrix();
		D3DXMATRIX animMatrix = m_pAnimated->GetMatrixTransformBone( m_idBone );
		*armeMatrix = animMatrix; //modificiation obj visuel

		NxMat33 m(	NxVec3( animMatrix._11, animMatrix._12, animMatrix._13),
					NxVec3( animMatrix._21, animMatrix._22, animMatrix._23),
					NxVec3( animMatrix._31, animMatrix._32, animMatrix._33));

		m_pArme->getActor()->setGlobalOrientation(m);

		m_pArme->SetPosition( animMatrix._41, animMatrix._42, animMatrix._43 );
	}
}



//////////////////////////////////////////////////////////////////////////
void Perso::PlayPersoSound( State state )
{
	SoundSystem::GetInstance()->SetListenerGain(1000.f);
	if( m_SoundMap.size() > 0 && m_SoundMap[state].size() > 0 )
	{

		int numSound = random(0,  m_SoundMap[state].size()-1);
		if( m_SoundMap[state][ numSound ] != NULL) 
		{
			if( !isPlayingSound() ) 
			{
				//A décommenter apres les test de son
				m_SoundMap[state][ numSound ]->SetPosition( m_Position );
				m_SoundMap[state][ numSound ]->Stop();
				m_SoundMap[state][ numSound ]->Play();
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void Perso::Animate( State state)
{
	if( m_AnimMap.size() > 0 && m_AnimMap[state].size() > 0 )
	{
		int numAnim = random(0,  m_AnimMap[state].size()-1);
		if( m_AnimMap[state][ numAnim ].anim_name != "" ) 
		{
			m_pAnimated->SetAnim	(m_AnimMap[state][ numAnim ].anim_name);
			m_pAnimated->SetAnimFPS	(m_AnimMap[state][ numAnim ].fps);
			m_pAnimated->SetLoop	(m_AnimMap[state][ numAnim ].loop);
			m_pAnimated->Play();
		}
	}

	PlayPersoSound( state );
}
//////////////////////////////////////////////////////////////////////////
void Perso::AddSound( State state, const std::string soundName )
{
	SoundObject* NewSound = new SoundObject(soundName);
	NewSound->SetDistance( 1.f );
	NewSound->SetPosition(0.f, 8.f, 0.f);
	m_SoundMap[state].push_back(NewSound);
}

void Perso::AddAnim( State state, const std::string animName, float fps, bool loop )
{
	m_AnimMap[state].push_back(AnimParam(animName, fps, loop));
}

//////////////////////////////////////////////////////////////////////////
void Perso::ReleaseSounds()
{
	int i = m_SoundMap[Perso::STATIC].size()-1;
	while( !m_SoundMap[Perso::STATIC].empty() )
	{
		releaseFromVector( m_SoundMap[Perso::STATIC], i); --i;
	}

	i = m_SoundMap[Perso::RUN].size()-1;
	while( !m_SoundMap[Perso::RUN].empty() )
	{
		releaseFromVector( m_SoundMap[Perso::RUN], i); --i;
	}

	i = m_SoundMap[Perso::HIT].size()-1;
	while( !m_SoundMap[Perso::HIT].empty() )
	{
		releaseFromVector( m_SoundMap[Perso::HIT], i); --i;
	}

	i = m_SoundMap[Perso::DIE].size()-1;
	while( !m_SoundMap[Perso::DIE].empty() )
	{
		releaseFromVector( m_SoundMap[Perso::DIE], i); --i;
	}

	i = m_SoundMap[Perso::ATTACK].size()-1;
	while( !m_SoundMap[Perso::ATTACK].empty() )
	{
		releaseFromVector( m_SoundMap[Perso::ATTACK], i); --i;
	}
}

void Perso::InitWeapon()
{
	if(m_pArme)
	{
		NxActor* pArmeActor = m_pArme->getActor();
		if( pArmeActor )
		{
			pArmeActor->getShapes()[0]->setGroup( GROUP_CONTROLLER );
			pArmeActor->raiseBodyFlag( NX_BF_DISABLE_GRAVITY );
			pArmeActor->raiseBodyFlag( NX_BF_FROZEN_ROT_X );
			pArmeActor->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );
			pArmeActor->raiseBodyFlag( NX_BF_FROZEN_ROT_Z );
			pArmeActor->userData = new ActorUserData;
			((ActorUserData*)pArmeActor->userData)->type = WEAPON;
			((ActorUserData*)pArmeActor->userData)->PersoRef = this;
		}
	}
}

bool Perso::isPlayingSound()
{
	unsigned int i = 0;
	bool playing = false;
	while( i < m_SoundMap[ m_currentState ].size() && !playing )
	{
		playing = m_SoundMap[ m_currentState ][ i ]->IsPlaying();
		++i;
	}

	return playing;
}

bool Perso::isPlayingAnim()
{
	unsigned int i = 0;
	bool playing = false;
// 	while( i < m_AnimMap[ m_currentState ].size() && !playing )
// 	{
// 		playing = m_AnimMap[ m_currentState ][ i ].();
// 		++i;
// 	}

	return playing;
}

//////////////////////////////////////////////////////////////////////////
void ContactReportCR::onContactNotify( NxContactPair& pair, NxU32 events )
{
	ActorUserData *ud1, *ud2;
	NxActor* Arme = pair.actors[1], *Victime = pair.actors[0];

	//Vérification de la présence des acteurs
	if(Arme && Victime)
	{
		ud1 = (ActorUserData*)(Arme->userData);
		ud2 = (ActorUserData*)(Victime->userData);
		//Si l'objet a bien un userdata
		if(ud1 && ud2)
		{
			Perso* perso1 = (Perso*)ud1->PersoRef;
			Perso* perso2 = (Perso*)ud2->PersoRef;
			if( perso1->getCurrentState() == Perso::ATTACK )
			{
				//Si l'objet qui frappe est une arme
				if( ud1->type == WEAPON )
				{
					perso2->Hit();
				}
			}
			else if( perso2->getCurrentState() == Perso::ATTACK )
			{
				//Si l'objet qui frappe est une arme
				if( ud2->type == WEAPON )
				{
					perso1->Hit();
				}
			}
		}
	}
}
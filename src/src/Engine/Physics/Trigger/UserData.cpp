#include "UserData.h"

ActorUserData::ActorUserData()
{
	OnEnterFunc = NULL; 
	OnLeaveFunc = NULL; 
	OnStayFunc  = NULL; 
}

Hero* ActorUserData::GetHero()
{
	return (Hero*)(PersoRef);
}

Enemy* ActorUserData::GetEnemy()
{
	return (Enemy*)(PersoRef);
}

void SetPersoRef(NxActor* pActor, void* Ref)
{
	pActor->userData = new ActorUserData;
	((ActorUserData*)(pActor->userData))->PersoRef = Ref ;
}

void SetTriggerFunctions(NxActorDesc &ActorDesc,
						 void (*OnEnterFunc)(), 
						 void (*OnLeaveFunc)(), 
						 void (*OnStayFunc)() )
{
	ActorDesc.userData = new ActorUserData;
	((ActorUserData*)(ActorDesc.userData))->OnEnterFunc = OnEnterFunc ;
	((ActorUserData*)(ActorDesc.userData))->OnLeaveFunc = OnLeaveFunc ;
	((ActorUserData*)(ActorDesc.userData))->OnStayFunc  = OnStayFunc  ;
}

////////////////////////////////////////////////////////////////////////
//////////////////////////// MANUAL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// actorDesc.userData = new ActorUserData;
// ((ActorUserData*)(actorDesc.userData))->OnEnterFunc = td->m_OnEnterFunc  ;
// ((ActorUserData*)(actorDesc.userData))->OnLeaveFunc = td->m_OnLeaveFunc  ;
// ((ActorUserData*)(actorDesc.userData))->OnStayFunc  = td->m_OnStayFunc   ; 
//
// NxActor* pActor = scene->createActor( actorDesc );

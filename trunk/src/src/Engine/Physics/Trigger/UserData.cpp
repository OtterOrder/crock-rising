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

void SetTriggerFunctions(NxShapeDesc &ShapeDesc,
						 void (*OnEnterFunc)(void* param), void* paramE,
						 void (*OnLeaveFunc)(void* param), void* paramL, 
						 void (*OnStayFunc)(void* param), void* paramS )
{
	ShapeDesc.userData = new ActorUserData;
	((ActorUserData*)(ShapeDesc.userData))->OnEnterFunc = OnEnterFunc ;
	((ActorUserData*)(ShapeDesc.userData))->paramEnter	= paramE;
	((ActorUserData*)(ShapeDesc.userData))->paramLeave	= paramL;
	((ActorUserData*)(ShapeDesc.userData))->paramStay	= paramS;
	((ActorUserData*)(ShapeDesc.userData))->OnLeaveFunc = OnLeaveFunc ;
	((ActorUserData*)(ShapeDesc.userData))->OnStayFunc  = OnStayFunc  ;
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

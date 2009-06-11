#include "UserData.h"

ActorUserData::ActorUserData()
{
	id = 0;
	OnEnterFunc = NULL; 
	OnLeaveFunc = NULL; 
	OnStayFunc  = NULL; 
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

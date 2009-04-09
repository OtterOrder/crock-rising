// ===============================================================================
//						     PHYSX SDK TRAINING PROGRAMS
//			                       TRIGGER REPORT
//
//						   Written by Bob Schade, 12-15-05
// ===============================================================================

#ifndef TRIGGER_H
#define TRIGGER_H

#include "UserData.h"


class TriggerReport : public NxUserTriggerReport
{
public:
	NxScene* scene;
	//Fonction de gestions des évenements lors des contacts avec les triggers
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
	{
		NxU32 i = 0;

		NxActor** actors = scene->getActors();
		NxU32 nbActors = scene->getNbActors();
		while (nbActors--)
		{
			NxActor* actor = actors[nbActors];
			ActorUserData *ud = (ActorUserData*)(actor->userData);
			if(ud)
			{
				switch( status )
				{
					case NX_TRIGGER_ON_ENTER : if(ud->OnEnterFunc) ud->OnEnterFunc();
						break;
					case NX_TRIGGER_ON_LEAVE : if(ud->OnLeaveFunc) ud->OnLeaveFunc();
						break;
					case NX_TRIGGER_ON_STAY :  if(ud->OnStayFunc) ud->OnStayFunc();
						break;
					default: break;
				}
			}

			// Mark actors in the trigger area to apply forcefield forces to them
			NxActor* triggerActor = &triggerShape.getActor();
			/*if (((ActorUserData*)(triggerActor->userData))->flags & UD_IS_TRIGGER)
			{
				if (status & NX_TRIGGER_ON_STAY)
				{
					NxActor* otherActor = &otherShape.getActor();
					((ActorUserData*)(otherActor->userData))->flags |= UD_IS_INSIDE_TRIGGER;
				}
			}*/
		}
	}
} gTriggerReport;

#endif  // TRIGGER_REPORT_H


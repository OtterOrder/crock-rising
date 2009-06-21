#ifndef TRIGGER_H
#define TRIGGER_H

#include "UserData.h"
#include "../Physicalizer.h"

/////////////////////////////////////////////////////////////////////////////////
// Classe de gestion des triggers. Elle est fournis à l'instance de physX dans //
// de Physicalizer par "m_Scene->setUserTriggerReport(&gTriggerReport);"	   //
// PhysX l'appelera dès qu'une collision aura lieux avec les actors possedant  //
// le flag NX_TRIGGER_ENABLE. Elle lancera les fonctions donnée à l'acteur lors//
// de sa création dans son userdata.										   //
/////////////////////////////////////////////////////////////////////////////////
class TriggerReport : public NxUserTriggerReport
{
public:
	NxScene* scene;
	//Fonction de gestions des évenements lors des contacts avec les triggers
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
	{
		if(scene)
		{
			NxActor** actors = scene->getActors();
			NxU32 nbActors = scene->getNbActors();
			while (nbActors--)
			{
				ActorUserData *ud = (ActorUserData*)(triggerShape.userData);
				if(ud)
				{
					switch( status )
					{
						case NX_TRIGGER_ON_ENTER : if(ud->OnEnterFunc) ud->OnEnterFunc(ud->paramEnter);
							break;
						case NX_TRIGGER_ON_LEAVE : if(ud->OnLeaveFunc) ud->OnLeaveFunc(ud->paramLeave);
							break;
						case NX_TRIGGER_ON_STAY :  if(ud->OnStayFunc) ud->OnStayFunc(ud->paramStay);
							break;
						default: break;
					}
				}
			}
		}
	}
};


#endif  // TRIGGER_REPORT_H


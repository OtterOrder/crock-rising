// ===============================================================================
//						  NVIDIA PHYSX SDK TRAINING PROGRAMS
//			                          USER DATA
//
//						    Written by Bob Schade, 5-1-06
// ===============================================================================

#ifndef _USERDATA_H_
#define _USERDATA_H_

#include "NxPhysics.h"


class Hero;
class Enemy;
enum PhysicalObjectType;

// structure qui permet de passer des informations dans le userdata d'un nxActor

void SetPersoRef(NxActor* pActor, void* Ref);
void SetTriggerFunctions(NxShapeDesc &ShapeDesc,
						 void (*OnEnterFunc)(void* param), void* paramE,
						 void (*OnLeaveFunc)(void* param), void* paramL, 
						 void (*OnStayFunc)(void* param), void* paramS );

struct ActorUserData
{
	ActorUserData();	
	~ActorUserData();

	void *PersoRef; //Pointeur sur un ennemis ou un hero

	void *paramEnter; //parametre a passer au trigger
	void *paramLeave;
	void *paramStay;

	void (*OnEnterFunc)(void* param); 
	void (*OnLeaveFunc)(void* param); 
	void (*OnStayFunc)(void* param); 

	int type;
};


#endif  // USERDATA_H


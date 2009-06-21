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

enum DataType
{
	HERO,
	ENEMY,
	ARME
};

enum UserDataFlag
{
	UD_IS_HW_ACTOR					= (1<<0),
	UD_IS_TRIGGER			        = (1<<1),
    UD_IS_INSIDE_TRIGGER            = (1<<2),
    UD_PASSES_INTERSECTION_TEST     = (1<<3),
    UD_HIT_BY_RAYCAST               = (1<<4),
    UD_NO_RENDER                    = (1<<5),
    UD_IS_DRAIN                     = (1<<6),
	UD_IS_ASLEEP				    = (1<<7),
	UD_RENDER_USING_LIGHT1			= (1<<8),
};

// structure qui permet de passer des informations dans le userdata d'un nxActor

void SetPersoRef(NxActor* pActor, void* Ref);
void SetTriggerFunctions(NxShapeDesc &ShapeDesc,
						 void (*OnEnterFunc)(void* param), void* paramE,
						 void (*OnLeaveFunc)(void* param), void* paramL, 
						 void (*OnStayFunc)(void* param), void* paramS );

struct ActorUserData
{
	ActorUserData();	
	void *PersoRef; //Pointeur sur un ennemis ou un hero

	void *paramEnter; //parametre a passer au trigger
	void *paramLeave;
	void *paramStay;

	void (*OnEnterFunc)(void* param); 
	void (*OnLeaveFunc)(void* param); 
	void (*OnStayFunc)(void* param); 

	Hero* GetHero();
	Enemy* GetEnemy();
};


#endif  // USERDATA_H


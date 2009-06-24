#include "ControledPhysicalCharacter.h"

#include "Physics/Physicalizer.h"
#include "Trigger/UserData.h"
#include "../../CrockRising/Characters/Hero.h"

//AlienHitReport*	gAlienCallBackTest				= new AlienHitReport;
extern HeroHitReport* gCharacterControllerCallback;

NxControllerAction HeroHitReport::onShapeHit(const NxControllerShapeHit& hit)
{
	if(hit.shape)
	{
		NxCollisionGroup group = hit.shape->getGroup();
		
		if( group )
		{
			NxActor& actor = hit.shape->getActor();
			if(hit.dir.y==0.0f)
			{
				NxF32 force = 100.0f;
				NxF32 coeff = actor.getMass() * hit.length * force;
				actor.addForceAtLocalPos( hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE );
			} 
		}
	}
	return NX_ACTION_NONE;
}

NxControllerAction HeroHitReport::onControllerHit(const NxControllersHit& hit)
{ 
	return NX_ACTION_NONE; 
}


void ContactReport::onContactNotify( NxContactPair& pair, NxU32 events )
{
	ActorUserData *UserDataA, *UserDataV;
	NxActor* Arme = pair.actors[0], *Victime = pair.actors[1];

	//Vérification de la présence des acteurs
	if(Arme && Victime)
	{
		UserDataA = (ActorUserData*)(Arme->userData);
		UserDataV = (ActorUserData*)(Victime->userData);
		//Si l'objet a bien un userdata
		if(UserDataA && UserDataV)
		{
			//Si l'objet qui frappe est une arme
			//if( Arme->getGroup() == GROUP_WEAPON )
			if( UserDataA->type == PHYS_WEAPON )
			{
				//Si l'objet frappé est une enemy
				if(UserDataV->type == PHYS_ENEMY)
				{
					Enemy* enemy = UserDataV->GetEnemy();
				}
				//ou le héro
				else if(UserDataV->type == PHYS_HERO)
				{
					Hero* hero = UserDataV->GetHero();
				}//endif hero
			}//endif weapon
		}//endif userdata		
	}//endif actors
}

int physX::CreateControlledCapsule( Vector3f pos, float radius, float height,
								   void* Ref, int &empActor, PhysicalObjectType objType )
{
	Physicalizer* physXInstance = Physicalizer::GetInstance();
	NxCapsuleControllerDesc desc;
	desc.radius			= radius;
	desc.height			= height;

	//spawn position
	desc.position.x = pos.x , 
	desc.position.y = pos.y + desc.height/2.0f + desc.radius, 
	desc.position.z = pos.z ;

	desc.upDirection	= NX_Y;
	desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
	desc.skinWidth		= 0.1f;
	desc.stepOffset		= 2.0f;
	desc.climbingMode	= CLIMB_CONSTRAINED;

	desc.callback		= gCharacterControllerCallback;


	physXInstance->getControllerManager()->createController( physXInstance->getScene(), desc );
	empActor = physX::getPhysicScene()->getNbActors() -1;

	NxActor* pActor = physX::getActor( empActor );
	pActor->userData = new ActorUserData;
	((ActorUserData*)(pActor->userData))->PersoRef = Ref ;
	((ActorUserData*)(pActor->userData))->type = objType ;

	return physXInstance->getControllerManager()->getNbControllers() - 1;
}

int physX::CreateControlledBox( Vector3f const pos, float width, float height, float depth,
							   void* Ref, int &empActor, PhysicalObjectType objType )
{
	Physicalizer* physXInstance = Physicalizer::GetInstance();
	NxBoxControllerDesc desc;
	desc.extents.x		= width,   desc.extents.y  = height,   desc.extents.z  = depth;
	desc.position.x		= pos.x, desc.position.y = pos.y, desc.position.z = pos.z;

	desc.upDirection	= NX_Y;
	desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
	desc.skinWidth		= 0.2f;
	desc.stepOffset		= 1.0f;

	physXInstance->getControllerManager()->createController( physXInstance->getScene(), desc );
	empActor = physX::getPhysicScene()->getNbActors() -1;

	NxActor* pActor = physX::getActor( empActor );
	pActor->userData = new ActorUserData;
	((ActorUserData*)(pActor->userData))->PersoRef = Ref ;
	((ActorUserData*)(pActor->userData))->type = objType ;

	return physXInstance->getControllerManager()->getNbControllers() - 1;
}


void physX::releaseController(int &empActor, int &empController)
{
	getControllerManager()->releaseController(*getController(empController));
	empController = -1;

	releaseActor( empActor );
}
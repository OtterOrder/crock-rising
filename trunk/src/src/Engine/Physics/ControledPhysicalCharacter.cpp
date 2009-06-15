#include "ControledPhysicalCharacter.h"

#include "Physics/Physicalizer.h"

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
	//TODO
	if (pair.actors[0])
	{
		//premi�r acteur
	}

	if (pair.actors[1])
	{
		//etc
	}
}

int physX::CreateControlledCapsule( Vector3f pos, float radius, float height, int &empActor )
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
	//		desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
	desc.slopeLimit		= 0.707f;
	desc.skinWidth		= 0.01f;
	desc.stepOffset		= 0.5f;
	//desc.stepOffset	= gInitialRadius * 0.5 * scale;
	//desc.stepOffset	= 0.01f;
	//desc.stepOffset	= 0;	// Fixes some issues
	//desc.stepOffset	= 10;

	desc.callback		= gCharacterControllerCallback;

	physXInstance->getControllerManager()->createController( physXInstance->getScene(), desc );
	empActor = physX::getPhysicScene()->getNbActors() -1;
	return physXInstance->getControllerManager()->getNbControllers() - 1;
}

int physX::CreateControlledBox( Vector3f pos, Vector3f size, int &empActor )
{
	Physicalizer* physXInstance = Physicalizer::GetInstance();
	NxBoxControllerDesc desc;
	desc.extents.x		= size.x,   desc.extents.y  = size.y,   desc.extents.z  = size.z;
	desc.position.x		= pos.x, desc.position.y = pos.y, desc.position.z = pos.z;

	desc.upDirection	= NX_Y;
	desc.slopeLimit		= 0;
	//	desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
	desc.skinWidth		= 0.2f;
	desc.stepOffset		= 0.5;
	//	desc.stepOffset	= 0.01f;
	//	desc.stepOffset	= 0;
	//	desc.stepOffset	= 10;

//	desc.callback		= gAlienCallBackTest; 

	physXInstance->getControllerManager()->createController( physXInstance->getScene(), desc );
	empActor = physX::getPhysicScene()->getNbActors() -1;
	return physXInstance->getControllerManager()->getNbControllers() - 1;
}
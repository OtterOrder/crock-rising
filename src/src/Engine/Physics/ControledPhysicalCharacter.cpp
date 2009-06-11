#include "ControledPhysicalCharacter.h"

#include "Physics/Physicalizer.h"

HeroHitReport*	gCharacterControllerCallback	= new HeroHitReport;
AlienHitReport*	gAlienCallBackTest				= new AlienHitReport;


NxControllerAction HeroHitReport::onShapeHit(const NxControllerShapeHit& hit)
{
	if(hit.shape)
	{
		NxCollisionGroup group = hit.shape->getGroup();
		if( group ) /*!= 0*/ /*Physicalizer::GROUP_STATIC )*/
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

NxControllerAction  AlienHitReport::onShapeHit(const NxControllerShapeHit& hit)
{ 
	// 1) si hit.shape est l'arme du hero
	//		alors baisser sa vie
	//    sinon 
	//		faire bouger les objets qu'il percutent

	if(hit.shape)
	{
		NxCollisionGroup group = hit.shape->getGroup();
		if( group == Physicalizer::GROUP_WEAPON )
		{
			NxU32 collisionFlags; 
			NxF32 minDistance = 0.001f; 

			hit.controller->move( NxVec3(0, 10, 0), Physicalizer::MASK_OTHER, minDistance, collisionFlags );

			//	NxActor& actor = hit.shape->getActor();
			//		if(hit.dir.y==0.0f)
			//		{
			//			NxF32 force = 100.0f;
			//			NxF32 coeff = actor.getMass() * hit.length * force;
			//			actor.addForceAtLocalPos( hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE );
			//		} 
		}
		/*else
		{
		if( group == Physicalizer::GROUP_STATIC )
		{ 
		hit.controller->getActor()->addLocalForceAtLocalPos( NxVec3( 0, 1, 0), NxVec3() );
		}
		}*/
	}
	return NX_ACTION_NONE;
}



int CreateControlledCapsule( Vector3f pos, float radius, float height )
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
	return physXInstance->getControllerManager()->getNbControllers() - 1;
}
int CreateControlledBox( Vector3f pos, Vector3f size )
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

	desc.callback		= gAlienCallBackTest; 

	physXInstance->getControllerManager()->createController( physXInstance->getScene(), desc );
	return physXInstance->getControllerManager()->getNbControllers() - 1;
}

// NxController* ControledBB::getBBController()
// { 
// 	Physicalizer* physXInstance = Physicalizer::GetInstance();
// 	NxControllerManager* pManager = physXInstance->m_ControllerManager;
// 
// 	pManager->updateControllers();
// 	return pManager->getController( m_empControlerList );
//}


// Vector3f ControledPhysicalCharacter::updateControlledPosition( void )
// { 
// 	//if( !m_isAlien ) //condition temp
// 	moveTo( 0.f, -0.1f, 0.f ); //gravity
// 
// 	NxController* pControler = getBBController();
// 	if( pControler )
// 	{
// 		NxExtendedVec3 pos = pControler->getPosition();
// 
// 		Vector3f offset( 0.f, -78.f, 0.f ); //offset Robot.dae
// 		//offset( 0.f, -2.f, 0.f ); //offset Capsule.dae
// 
// 		if( m_isAlien )  //offset Alien1.dae
// 		{
// 			offset.x =  0.f; 
// 			offset.y =  -16.f;
// 			offset.z =  -4.43f; 
// 		}
// 
// 		m_vPosition = Vector3f( (float)pos.x - offset.x, 
// 			(float)pos.y - offset.y, 
// 			(float)pos.z - offset.z );
// 
// 		return offset;
// 	}
// 
// 	return Vector3f(0.f, 0.f, 0.f);
// }
// 
// void ControledPhysicalCharacter::Update()
// {
// 	Object::Update();
// }
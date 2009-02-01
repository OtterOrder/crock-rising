#include "BoundingBox.h"

//===========================================================================//
//Testera si il y a collision avec l'objet po                                //
//===========================================================================//
bool BoundingBox::IsInCollision(const BoundingBox &po)
{
	return false;
}


BoundingBox::BoundingBox(ShapeDescription *ShapeDesc, ShapeType Type, Material* aMat)
{ 
	m_bDebugMode = true;

	Physicalizer* physXInstance = Physicalizer::GetInstance();

	NxBodyDesc bodyDesc;
	NxActorDesc actorDesc;

	m_pMat = aMat;

	switch( Type )
	{
	case BOX:
		{
			NxBoxShapeDesc boxDesc;
			BoxDescription* bd = (BoxDescription*)ShapeDesc;

			boxDesc.dimensions = NxVec3( bd->dimension.x, bd->dimension.y, bd->dimension.z );
			boxDesc.mass = bd->mass;

			actorDesc.shapes.push_back(&boxDesc);
			actorDesc.density		= (NxReal)bd->density;	
			actorDesc.globalPose.t	= NxVec3( NxVec3(bd->globalPosition.x, bd->globalPosition.y, bd->globalPosition.z) );

		}
		break;

	case SPHERE:
		{
			NxSphereShapeDesc sphereDesc;
			SphereDescription* sd = (SphereDescription*)ShapeDesc;

			sphereDesc.radius = (NxReal)sd->radius ;
			sphereDesc.mass = sd->mass;

			actorDesc.shapes.push_back(&sphereDesc);
			actorDesc.density		= (NxReal)sd->density;	
			actorDesc.globalPose.t	= NxVec3( NxVec3(sd->globalPosition.x, sd->globalPosition.y, sd->globalPosition.z) );


		}
		break;
	case CAPSULE:
		{
			NxCapsuleShapeDesc capsuleDesc;
			CapsuleDescription* cd = (CapsuleDescription*)ShapeDesc;

			capsuleDesc.radius =(NxReal)cd->radius ;
			capsuleDesc.height =(NxReal)cd->height ;
			capsuleDesc.mass = cd->mass;

			actorDesc.shapes.push_back(&capsuleDesc);
			actorDesc.density		= (NxReal)cd->density;	
			actorDesc.globalPose.t	= NxVec3( NxVec3(cd->globalPosition.x, cd->globalPosition.y, cd->globalPosition.z) );

		}
		break;

	}

	actorDesc.body = &bodyDesc;
	actorDesc.userData = (void*)1;

	if(actorDesc.isValid())
	{
		NxScene* scene = physXInstance->getScene();

		NxActor* pActor = scene->createActor( actorDesc );
		//assert( pActor );

		m_empListe = scene->getNbActors() - 1;
	}
}
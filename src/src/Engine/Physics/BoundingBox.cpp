#include "BoundingBox.h"

//===========================================================================//
//Testera si il y a collision avec l'objet po                                //
//===========================================================================//
bool BoundingBox::IsInCollision(const BoundingBox &po)
{
	return false;
}


BoundingBox BoundingBox::BoundingBoxInit(BoundingDescription *Desc, ShapeType Type, MaterialPhysics Mat)
{ 
	m_bDebugMode = true;

	Physicalizer* physXInstance = Physicalizer::GetInstance();

	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.5f;
	bodyDesc.linearVelocity = VecToNxVec(Desc->initialvelocity);

	NxActorDesc actorDesc;

	m_Mat = Mat;

	switch( Type )
	{
	case BOX:
		{
			NxBoxShapeDesc boxDesc;
			//BoxDescription* bd = &BoxDescription(Desc, Desc->dimension);// Vector3f(1.0, 1.0, 1.0));

			boxDesc.dimensions = VecToNxVec( Desc->dimension);
			boxDesc.mass = Desc->mass;

			actorDesc.shapes.push_back(&boxDesc);
			actorDesc.density		= (NxReal)Desc->density;	
			actorDesc.globalPose.t	= VecToNxVec(Desc->globalPosition);

		}
		break;

	case SPHERE:
		{
			NxSphereShapeDesc sphereDesc;
			//SphereDescription* Desc = (SphereDescription*)Desc;

			sphereDesc.radius = (NxReal)Desc->radius ;
			sphereDesc.mass = Desc->mass;

			actorDesc.shapes.push_back(&sphereDesc);
			actorDesc.density		= (NxReal)Desc->density;	
			actorDesc.globalPose.t	= VecToNxVec(Desc->globalPosition);
		}
		break;
	case CAPSULE:
		{
			NxCapsuleShapeDesc capsuleDesc;
			//CapsuleDescription* cd = (CapsuleDescription*)Desc;

			capsuleDesc.radius =(NxReal)Desc->radius ;
			capsuleDesc.height =(NxReal)Desc->height ;
			capsuleDesc.mass = Desc->mass;

			actorDesc.shapes.push_back(&capsuleDesc);
			actorDesc.density		= (NxReal)Desc->density;	
			actorDesc.globalPose.t	= VecToNxVec(Desc->globalPosition);

		}
		break;
	case PLAN:
		{

			// Create ground plane
			NxPlaneShapeDesc planeDesc; //Contient les caracteristiques du plan
			// Créer l'acteur "plan du sol"
			actorDesc.shapes.pushBack(&planeDesc);//On rajoute la boite englobante de ce plan
			actorDesc.globalPose.t = VecToNxVec( Desc->globalPosition);

			NxScene* scene = physXInstance->getScene();
			NxActor* pActor = scene->createActor( actorDesc );
			m_iEmplacement = scene->getNbActors() - 1;

			return *this;
		}
	}

	actorDesc.body = &bodyDesc;

	assert(actorDesc.isValid());

	NxScene* scene = physXInstance->getScene();
	NxActor* pActor = scene->createActor( actorDesc );
	assert(pActor);

	pActor->userData = (void*)size_t(Desc->dimension.x);

	m_iEmplacement = scene->getNbActors() - 1;

	return *this;
}

//////////////////////////////////////////////////////////////////////////
//BOX
BoundingBox::BoundingBox(Vector3f adimension, Vector3f aglobalpos, float adenstity, Vector3f InitVelocity, MaterialPhysics aMat)
{
	BoundingDescription bd(adimension, aglobalpos, adenstity);
	bd.initialvelocity = InitVelocity;
	*this = BoundingBoxInit(&bd, BOX, aMat);
}

//////////////////////////////////////////////////////////////////////////
//SPHERE
BoundingBox::BoundingBox(float aradius, Vector3f aglobalpos, float adenstity, Vector3f InitVelocity, MaterialPhysics aMat)
{
	BoundingDescription sd(aradius, aglobalpos, adenstity);
	sd.initialvelocity = InitVelocity;
	*this = BoundingBoxInit(&sd, SPHERE, aMat);
}

//////////////////////////////////////////////////////////////////////////
//CAPS
BoundingBox::BoundingBox(float aradius, float aheight, Vector3f aglobalpos, float adenstity, Vector3f InitVelocity, MaterialPhysics aMat)
{
	BoundingDescription cd(aradius, aheight, aglobalpos, adenstity);
	cd.initialvelocity = InitVelocity;
	*this = BoundingBoxInit(&cd, CAPSULE, aMat);
}

//////////////////////////////////////////////////////////////////////////
//SOL
BoundingBox::BoundingBox(Vector2f asurface, float Hauteur, MaterialPhysics aMat)
{
	BoundingDescription gd(asurface, Vector3f(0.f, Hauteur, 0.f));
	*this = BoundingBoxInit(&gd, PLAN, aMat);
}

BoundingBox& BoundingBox::operator=( const BoundingBox& bb )
{
	m_bDebugMode = bb.getDebugMode();
 	m_Mat= bb.getMat();
	m_iEmplacement = bb.getEmplacement();
	return *this;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

NxVec3 VecToNxVec(const Vector3f V)
{
	return NxVec3(V.x, V.y, V.z);
}

void Normalize( Vector3f &V )
{
	V = V/Norme(V);
}

float Norme( Vector3f V )
{
	float norme = sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
	return norme > 0 ? norme : 1;
}

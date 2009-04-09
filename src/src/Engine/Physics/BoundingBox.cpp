#include "BoundingBox.h"
#include "Physicalizer.h"
//#include "Trigger/Trigger.h"

//===========================================================================//
//Testera si il y a collision avec l'objet po                                //
//===========================================================================//
bool BoundingBox::IsInCollision(const BoundingBox &po)
{
	return false;
}

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

BoundingBox& BoundingBox::operator=( const BoundingBox& bb )
{
	m_bDebugMode = bb.getDebugMode();
 	m_Mat= bb.getMat();
	m_iEmplacement = bb.getEmplacement();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Création d'une boundingbox + intégration dans le moteur physique		   //
// Il faut passer la description associé au type(, boxdesc pour box etc)   //
/////////////////////////////////////////////////////////////////////////////
BoundingBox::BoundingBox(ShapeDescription* Desc)
{
	m_bDebugMode = true;

	Physicalizer* physXInstance = Physicalizer::GetInstance();

	NxActorDesc actorDesc;
	NxScene* scene = physXInstance->getScene();

	//m_Mat = Mat;

	switch( Desc->m_type )
	{
	case BOX:
		{
			NxBoxShapeDesc boxDesc;
			BoxDescription* bd = (BoxDescription*)Desc;

			boxDesc.dimensions = VecToNxVec( bd->m_dimension);
			boxDesc.mass = bd->m_mass;
			boxDesc.localPose.t = NxVec3( 0,0, 0.5 );

			actorDesc.shapes.push_back(&boxDesc);
			actorDesc.density		= (NxReal)bd->m_density;	
			actorDesc.globalPose.t	= VecToNxVec(bd->m_pos);
		}
		break;

	case SPHERE:
		{
			NxSphereShapeDesc sphereDesc;
			SphereDescription* sd = (SphereDescription*)Desc;

			sphereDesc.radius = (NxReal)sd->m_radius ;
			sphereDesc.mass = sd->m_mass;

			actorDesc.shapes.push_back(&sphereDesc);
			actorDesc.density		= (NxReal)sd->m_density;	
			actorDesc.globalPose.t	= VecToNxVec(sd->m_pos);
		}
		break;
	case CAPSULE:
		{
			NxCapsuleShapeDesc capsuleDesc;
			CapsuleDescription* cd = (CapsuleDescription*)Desc;

			capsuleDesc.radius =(NxReal)cd->m_radius ;
			capsuleDesc.height =(NxReal)cd->m_height ;
			capsuleDesc.mass = cd->m_mass;

			actorDesc.shapes.push_back(&capsuleDesc);
			actorDesc.density		= (NxReal)cd->m_density;	
			actorDesc.globalPose.t	= VecToNxVec(cd->m_pos);

		}
		break;
	case TRIGGER:
		{
			//Paramétrage de l'acteur
			NxBoxShapeDesc boxDesc;
			TriggerDescription* td = (TriggerDescription*)Desc;
			boxDesc.dimensions = VecToNxVec( td->m_dimension );
			boxDesc.shapeFlags |= NX_TRIGGER_ENABLE;

			actorDesc.shapes.pushBack(&boxDesc);
			actorDesc.globalPose.t = VecToNxVec( td->m_pos ) + NxVec3( 0.f, td->m_dimension.y/2, 0.f );

			assert(actorDesc.isValid());

			//Remplissage du userdata => on lui donne ses fonctions
			ActorUserData * userdata = new ActorUserData;
			userdata->OnEnterFunc = td->m_OnEnterFunc;
			userdata->OnLeaveFunc = NULL;//td->OnLeaveFunc;
			userdata->OnStayFunc = NULL;//td->OnStayFunc;
			actorDesc.userData = userdata;
		}
	}

	if( Desc->m_type != TRIGGER )
	{
		NxBodyDesc bodyDesc;
		bodyDesc.angularDamping = 0.5f;
		bodyDesc.linearVelocity = VecToNxVec(Desc->m_linearVelocity);
		actorDesc.body = &bodyDesc;
	}


	NxActor* pActor = scene->createActor( actorDesc );
	assert(pActor);

	m_iEmplacement = scene->getNbActors() - 1;
}

////////////////////////////////////////////////////////////////////////
//////////////////////////// FUCKING MANUAL ////////////////////////////
////////////////////////////////////////////////////////////////////////
/*
//Création Cube
	SceneObject* aCube = NULL;

	aCube = new SceneObject("Cube.DAE", D3DXVECTOR3(0.f,0.f,0.f));
	aCube->InitObject();
	aCube->GetMaterial()->SetTexture("default.jpg", Texture::DIFFUSE);
	aCube->SetShader("blinn.fx");
	BoxDescription desc(Vector3f(demiSize, demiSize, demiSize), density, 1.0f, Pos);
	desc.linearVelocity = initialVel;
	BoundingBox bb = BoundingBox(&desc, BOX);

	Physicalizer::GetInstance()->SetPhysicable(aCube, &bb);

//Création Trigger

	SceneObject* Cube = new SceneObject("Cube.DAE", Vector3f(0.f, 0.f, 0.f));

	Cube->InitObject();
	Cube->GetMaterial()->SetTexture("default.jpg", Texture::DIFFUSE);
	Cube->SetShader("blinn.fx");
	TriggerDescription td;
	td.OnEnterFunc = test;
	BoundingBox bb(&td, TRIGGER);

	Physicalizer::GetInstance()->SetPhysicable(Cube, &bb);//&BoundingBox(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0.f, 1.f, 0.f )));
*/
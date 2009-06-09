#include "BoundingBox.h"
#include "Physicalizer.h"


//ControllerHitReport* gTest = new ControllerHitReport;

//===========================================================================//
//Testera si il y a collision avec l'objet po                                //
//===========================================================================//
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

/////////////////////////////////////////////////////////////////////////////
// Création d'une boundingbox + intégration dans le moteur physique		   //
// Il faut passer la description associé au type(, boxdesc pour box etc)   //
/////////////////////////////////////////////////////////////////////////////


void CreateBox( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger  )
{
	NxBoxShapeDesc *boxDesc = new NxBoxShapeDesc;

	boxDesc->dimensions = VecToNxVec( Pb->bodySize);			//Dimension
	boxDesc->mass = Pb->fMass;									//Masse
	boxDesc->localPose.t = VecToNxVec( Pb->translate );			//Position
	boxDesc->materialIndex = GenMaterial(Pb->restitution, Pb->staticFriction, Pb->dynamiqueFriction);	//Materiel4
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);					
	boxDesc->localPose.M = m;									//Rotation
	if(IsTrigger) boxDesc->shapeFlags |= NX_TRIGGER_ENABLE;		//Trigger
	assert(boxDesc->isValid());

	ActorDesc.shapes.push_back(boxDesc);
	List.ShapeRefList.push_back(boxDesc); //Va permettre la libération des shapesà la destruction de l'objet
}

void CreateSphere( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger  )
{
	NxSphereShapeDesc *sphereDesc = new NxSphereShapeDesc;

	sphereDesc->radius = (NxReal)Pb->bodySize.x ;				//Rayon
	sphereDesc->mass = Pb->fMass;								//Masse
	sphereDesc->localPose.t = VecToNxVec( Pb->translate );		//Position DU SHAPE
	sphereDesc->materialIndex = GenMaterial(Pb->restitution, Pb->staticFriction, Pb->dynamiqueFriction);	//Materiel
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);					
	sphereDesc->localPose.M = m;								//Rotation DU SHAPE
	if(IsTrigger) sphereDesc->shapeFlags |= NX_TRIGGER_ENABLE;	//Trigger
	assert(sphereDesc->isValid());

	ActorDesc.shapes.push_back(sphereDesc);
	List.ShapeRefList.push_back(sphereDesc);
}

void CreateCapsule( NxActorDesc& ActorDesc, ListOfBoundingBox& List, PhysicBody* Pb, bool IsTrigger  )
{
	NxCapsuleShapeDesc *capsuleDesc = new NxCapsuleShapeDesc;

	capsuleDesc->radius =(NxReal)Pb->bodySize.x ;				//Rayon
	capsuleDesc->height =(NxReal)Pb->bodySize.y ;				//Hauteur
	capsuleDesc->mass = Pb->fMass;								//Masse
	capsuleDesc->localPose.t = VecToNxVec( Pb->translate );		//Position
	capsuleDesc->materialIndex = GenMaterial(Pb->restitution, Pb->staticFriction, Pb->dynamiqueFriction);	//Materiel
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);					
	capsuleDesc->localPose.M = m;								//Rotation
	if(IsTrigger) capsuleDesc->shapeFlags |= NX_TRIGGER_ENABLE;	//Trigger
	assert(capsuleDesc->isValid());

	ActorDesc.shapes.push_back(capsuleDesc);
	List.ShapeRefList.push_back(capsuleDesc);
}

void FinalizeActor( NxActorDesc ActorDesc, ListOfBoundingBox List, bool IsTrigger )
{
	ActorDesc.density		= 1.0f;
	ActorDesc.globalPose.t	= VecToNxVec(List.WorldPos);
	PhysicBody* Pb;

	if(IsTrigger)
	{
		Pb = *List.List.begin();
		ActorDesc.userData = new ActorUserData;
		((ActorUserData*)(ActorDesc.userData))->OnEnterFunc = Pb->OnEnterFunc  ;
		((ActorUserData*)(ActorDesc.userData))->OnLeaveFunc = Pb->OnLeaveFunc  ;
		((ActorUserData*)(ActorDesc.userData))->OnStayFunc  = Pb->OnStayFunc   ;
	}
	else
	{
		if(List.List[0]->bDisableCollision)
			ActorDesc.body = NULL;
		else
		{
			NxBodyDesc bodyDesc;
			bodyDesc.angularDamping = 0.5f;
			bodyDesc.linearVelocity = VecToNxVec( List.List[0]->linearVelocity );
			bodyDesc.angularVelocity = VecToNxVec( List.List[0]->angularVelocity );
			assert(bodyDesc.isValid());
			ActorDesc.body = &bodyDesc;
		}
	}

	assert(ActorDesc.isValid());

	NxScene* scene = GetPhysicScene();
	NxActor* pActor = scene->createActor( ActorDesc );
	assert(pActor);
}


NxMaterialIndex GenMaterial( float restitution, float staticFriction, float dynamiqueFriction )
{
	NxScene* Scene = GetPhysicScene();
	NxMaterialDesc materialDesc;
	materialDesc.restitution		= restitution;
	materialDesc.dynamicFriction	= staticFriction;
	materialDesc.staticFriction		= dynamiqueFriction;
	assert(materialDesc.isValid());
	return Scene->createMaterial(materialDesc)->getMaterialIndex();
}

int CreateBoundingBox(ShapeDescription* Desc)
{
	return 0;
// 	Physicalizer* physXInstance = Physicalizer::GetInstance();
// 
// 	NxActorDesc actorDesc;
// 	NxScene* scene = physXInstance->getScene();
// 
// 	switch( Desc->m_type )
// 	{
// 	case BOX:
// 		{
// 			NxBoxShapeDesc boxDesc;
// 			BoxDescription* bd = (BoxDescription*)Desc;
// 
// 			boxDesc.dimensions = VecToNxVec( bd->m_dimension);
// 			boxDesc.group = Physicalizer::GROUP_DYNAMIQUE;
// 			boxDesc.mass = 0.0;//bd->m_mass;
// 			boxDesc.localPose.t = VecToNxVec( bd->m_ReglagePivot );
// 
// 			actorDesc.shapes.push_back(&boxDesc);
// 			actorDesc.density		= (NxReal)bd->m_density;
// 			actorDesc.globalPose.t	= VecToNxVec(bd->m_pos) + NxVec3( 0.f, bd->m_dimension.y/2, 0.f );
// 		}
// 		break;
// 
// 	case SPHERE:
// 		{
// 			NxSphereShapeDesc sphereDesc;
// 			SphereDescription* sd = (SphereDescription*)Desc;
// 
// 			sphereDesc.radius = (NxReal)sd->m_radius ;
// 			sphereDesc.mass = sd->m_mass;
// 			sphereDesc.localPose.t = VecToNxVec( sd->m_ReglagePivot );
// 
// 			actorDesc.shapes.push_back(&sphereDesc);
// 			actorDesc.density		= (NxReal)sd->m_density;	
// 			actorDesc.globalPose.t	= VecToNxVec(sd->m_pos) + NxVec3( 0.f, sd->m_radius/2, 0.f );
// 		} 
// 		break;
// 
// 	case CAPSULE:
// 		{
// 			NxCapsuleShapeDesc capsuleDesc;
// 			CapsuleDescription* cd = (CapsuleDescription*)Desc;
// 
// 			capsuleDesc.radius =(NxReal)cd->m_radius ;
// 			capsuleDesc.height =(NxReal)cd->m_height ;
// 			capsuleDesc.mass = cd->m_mass;
// 			capsuleDesc.localPose.t = VecToNxVec( cd->m_ReglagePivot );
// 
// 			actorDesc.shapes.push_back(&capsuleDesc);
// 			actorDesc.density		= (NxReal)cd->m_density;	
// 			actorDesc.globalPose.t	= VecToNxVec(cd->m_pos);
// 
// 		}
// 		break;
// 	case TRIGGER:
// 		{
// 			//Paramétrage de l'acteur
// 			NxBoxShapeDesc boxDesc;
// 			TriggerDescription* td = (TriggerDescription*)Desc;
// 			boxDesc.dimensions = VecToNxVec( td->m_dimension );
// 			boxDesc.shapeFlags |= NX_TRIGGER_ENABLE;
// 
// 			actorDesc.shapes.pushBack(&boxDesc);
// 			actorDesc.globalPose.t = VecToNxVec( td->m_pos ) + NxVec3( 0.f, td->m_dimension.y/2, 0.f );
// 
// 			assert(actorDesc.isValid());
// 
// 			//Remplissage du userdata => on lui donne ses fonctions
// 			actorDesc.userData = new ActorUserData;
// 			((ActorUserData*)(actorDesc.userData))->OnEnterFunc = td->m_OnEnterFunc  ;
// 			((ActorUserData*)(actorDesc.userData))->OnLeaveFunc = td->m_OnLeaveFunc  ;
// 			((ActorUserData*)(actorDesc.userData))->OnStayFunc  = td->m_OnStayFunc   ;
// 		}
// 	}
// 
// 
// 	if( Desc->m_type != TRIGGER )
// 	{
// 		NxBodyDesc bodyDesc;
// 		bodyDesc.angularDamping = 0.5f;
// 		bodyDesc.linearVelocity = VecToNxVec(Desc->m_linearVelocity);
// 		actorDesc.body = &bodyDesc;
// 	}
// 
// 
// 	NxActor* pActor = scene->createActor( actorDesc );
// 	assert(pActor);
}

int CreateBoundingBox( ListOfBoundingBox &BBList, bool IsTrigger )
{
	NxActorDesc actorDesc;
	PhysicBody* Pb;
	std::vector<PhysicBody*>::iterator it = BBList.List.begin();

	while(it < BBList.List.end())
	{
		Pb = *it++;
		switch( Pb->type )
		{
			case BOX	 : CreateBox(actorDesc, BBList, Pb, IsTrigger);		break;
			case SPHERE	 : CreateSphere(actorDesc, BBList, Pb, IsTrigger);	break;
			case CAPSULE : CreateCapsule(actorDesc, BBList, Pb, IsTrigger); break;
		}
	}

	FinalizeActor(actorDesc, BBList, IsTrigger);
	return GetPhysicScene()->getNbActors() - 1;
}

void ListOfBoundingBox::MajPivot(Mesh* pMesh)
{
	PhysicBody* Pb;
	std::vector<PhysicBody*>::iterator it = List.begin();
	while(it < List.end())
	{
		Pb = *it++;
		Pb->translate += pMesh->m_ReglagePivot;
	}
}

void ListOfBoundingBox::ReleaseList()
{
	NxShapeDesc* Sd;
	std::vector<NxShapeDesc*>::iterator it = ShapeRefList.begin();
	while(it < ShapeRefList.end())
	{
		Sd = *it++;
		delete Sd;
	}	
}

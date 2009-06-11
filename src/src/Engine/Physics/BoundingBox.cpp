#include "BoundingBox.h"
#include "Physicalizer.h"


//ControllerHitReport* gTest = new ControllerHitReport;

//===========================================================================//
//Testera si il y a collision avec l'objet po                                //
//===========================================================================//
NxVec3 VecToNxVec(Vector3f V)
{
	return NxVec3(V.x, V.y, V.z);
}

void Normalize( Vector3f &V )
{
	V = V/Norme(V);
}

float Norme( const Vector3f V )
{
	float norme = sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
	return norme > 0 ? norme : 1;
}

/////////////////////////////////////////////////////////////////////////////
// Création d'une boundingbox + intégration dans le moteur physique		   //
// Il faut passer la description associé au type(, boxdesc pour box etc)   //
/////////////////////////////////////////////////////////////////////////////


void CreateBox( NxActorDesc& ActorDesc, ListOfBoundingBox& List, const PhysicBody* Pb, const bool IsTrigger  )
{
	NxBoxShapeDesc *boxDesc = new NxBoxShapeDesc;
	if(List.getPbList()[0]->bIsDynamic) 
		boxDesc->group = Physicalizer::GROUP_STATIC;
	boxDesc->dimensions = VecToNxVec( Pb->bodySize);			//Dimension
	boxDesc->mass = Pb->fMass;									//Masse
	boxDesc->localPose.t = VecToNxVec( Pb->translate );			//Position
	boxDesc->materialIndex = GenMaterial(Pb->restitution, Pb->staticFriction, Pb->dynamiqueFriction);	//Materiel4
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);					
	boxDesc->localPose.M = m;									//Rotation
	if(IsTrigger) boxDesc->shapeFlags |= NX_TRIGGER_ENABLE;		//Trigger
	assert(boxDesc->isValid());

	ActorDesc.shapes.push_back(boxDesc);
	List.pushShapeRef( boxDesc );
}

void CreateSphere( NxActorDesc& ActorDesc, ListOfBoundingBox& List, const PhysicBody* Pb, const bool IsTrigger  )
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
	List.pushShapeRef( sphereDesc );
}

void CreateCapsule( NxActorDesc& ActorDesc, ListOfBoundingBox& List, const PhysicBody* Pb, const bool IsTrigger  )
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
	List.pushShapeRef( capsuleDesc );
}

void FinalizeActor( NxActorDesc ActorDesc, ListOfBoundingBox List, const bool IsTrigger )
{
	ActorDesc.density		= 1.0f;
	ActorDesc.globalPose.t	= VecToNxVec(List.getInitialWorldPos());
	PhysicBody* Pb = *List.getPbList().begin();

	if(IsTrigger)
	{
		ActorDesc.userData = new ActorUserData;
		((ActorUserData*)(ActorDesc.userData))->OnEnterFunc = Pb->OnEnterFunc  ;
		((ActorUserData*)(ActorDesc.userData))->OnLeaveFunc = Pb->OnLeaveFunc  ;
		((ActorUserData*)(ActorDesc.userData))->OnStayFunc  = Pb->OnStayFunc   ;
	}
	else
	{
		if(!Pb->bIsDynamic)
			ActorDesc.body = NULL;
		else
		{
			NxBodyDesc bodyDesc;
			bodyDesc.angularDamping = 0.5f;
			bodyDesc.linearVelocity = VecToNxVec( Pb->linearVelocity );
			bodyDesc.angularVelocity = VecToNxVec( Pb->angularVelocity );
			assert(bodyDesc.isValid());
			ActorDesc.body = &bodyDesc;
		}
	}

	assert(ActorDesc.isValid());

	NxScene* scene = GetPhysicScene();
	NxActor* pActor = scene->createActor( ActorDesc );
	assert(pActor);
}


NxMaterialIndex GenMaterial( const float restitution, const float staticFriction, const float dynamiqueFriction )
{
	NxScene* Scene = GetPhysicScene();
	NxMaterialDesc materialDesc;
	materialDesc.restitution		= restitution;
	materialDesc.dynamicFriction	= staticFriction;
	materialDesc.staticFriction		= dynamiqueFriction;
	assert(materialDesc.isValid());
	return Scene->createMaterial(materialDesc)->getMaterialIndex();
}

int CreateBoundingBox( ListOfBoundingBox &BBList )
{
	NxActorDesc actorDesc;
	PhysicBody* Pb;
	std::vector<PhysicBody*> PbList = BBList.getPbList();
	std::vector<PhysicBody*>::iterator it = PbList.begin();

	while(it < PbList.end())
	{
		Pb = *it++;
		switch( Pb->type )
		{
			case BOX	 : CreateBox(actorDesc, BBList, Pb, false);		break;
			case SPHERE	 : CreateSphere(actorDesc, BBList, Pb, false);	break;
			case CAPSULE : CreateCapsule(actorDesc, BBList, Pb, false); break;
		}
	}

	FinalizeActor(actorDesc, BBList, false);
	return GetPhysicScene()->getNbActors() - 1;
}


int CreateTrigger(ListOfBoundingBox &BBList,
	void (*OnEnterFunc)(), 
	void (*OnLeaveFunc)(), 
	void (*OnStayFunc)() )
{
	NxActorDesc actorDesc;
	PhysicBody* Pb;
	std::vector<PhysicBody*> PbList = BBList.getPbList();
	std::vector<PhysicBody*>::iterator it = PbList.begin();

	while(it < PbList.end())
	{
		Pb = *it++;
		switch( Pb->type )
		{
			case BOX	 : CreateBox(actorDesc, BBList, Pb, true);		break;
			case SPHERE	 : CreateSphere(actorDesc, BBList, Pb, true);	break;
			case CAPSULE : CreateCapsule(actorDesc, BBList, Pb, true);  break;
		}
	}

	FinalizeActor(actorDesc, BBList, true);
	return GetPhysicScene()->getNbActors() - 1;
}

void ListOfBoundingBox::MajPivot(const Mesh* pMesh)
{
	PhysicBody* Pb;
	std::vector<PhysicBody*>::iterator it = m_PbList.begin();
	while(it < m_PbList.end())
	{
		Pb = *it++;
		Pb->translate += pMesh->m_ReglagePivot;
	}
}

void ListOfBoundingBox::ReleaseList()
{
	NxShapeDesc* Sd;
	std::vector<NxShapeDesc*>::iterator it = m_ShapeRefList.begin();
	while(it < m_ShapeRefList.end())
	{
		Sd = *it++;
		delete Sd;
	}	
}

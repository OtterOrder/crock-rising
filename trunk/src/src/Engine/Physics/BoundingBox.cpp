#include "BoundingBox.h"
#include "Physicalizer.h"


NxShapeDesc* CreateBox( NxActorDesc& ActorDesc, const PhysicBody* Pb );
NxShapeDesc* CreateSphere( NxActorDesc& ActorDesc, const PhysicBody* Pb );
NxShapeDesc* CreateCapsule( NxActorDesc& ActorDesc, const PhysicBody* Pb );
void FinalizeActor(const NxActorDesc ActorDesc, const Vector3f Pos, const PhysicBody* Pb, GroupCollision group);
NxMaterialIndex GenMaterial( const float restitution, const float staticFriction, const float dynamiqueFriction );

NxShapeDesc* CreateBox( NxActorDesc& ActorDesc, const PhysicBody* Pb )
{
	NxBoxShapeDesc *boxDesc = new NxBoxShapeDesc;
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);					

	boxDesc->dimensions		= VecToNxVec( Pb->bodySize);												//Dimension
	boxDesc->mass			= Pb->fMass;																//Masse
	boxDesc->localPose.t	= VecToNxVec( Pb->translate );												//Position
	boxDesc->group			= Pb->bIsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;						//Groupe
	boxDesc->materialIndex	= GenMaterial(Pb->restitution, Pb->staticFriction, Pb->dynamiqueFriction);	//Materiel
	boxDesc->localPose.M	= m;																		//Rotation
	if(Pb->type == TRIGGER) 
		boxDesc->shapeFlags |= NX_TRIGGER_ENABLE;														//Trigger
	assert(boxDesc->isValid());

	ActorDesc.shapes.push_back(boxDesc);
	return boxDesc;
}

NxShapeDesc* CreateSphere( NxActorDesc& ActorDesc, const PhysicBody* Pb )
{
	NxSphereShapeDesc *sphereDesc = new NxSphereShapeDesc;
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);					

	sphereDesc->radius			= (NxReal)Pb->bodySize.x ;													//Rayon
	sphereDesc->mass			= Pb->fMass;																//Masse
	sphereDesc->localPose.t		= VecToNxVec( Pb->translate );												//Position
	sphereDesc->group			= Pb->bIsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;							//Groupe
	//sphereDesc->group			= group;																	//Groupe
	sphereDesc->materialIndex	= GenMaterial(Pb->restitution, Pb->staticFriction, Pb->dynamiqueFriction);	//Materiel
	sphereDesc->localPose.M		= m;																		//Rotation
	if(Pb->type == TRIGGER) 
		sphereDesc->shapeFlags	|= NX_TRIGGER_ENABLE;														//Trigger
	assert(sphereDesc->isValid());

	ActorDesc.shapes.push_back(sphereDesc);
	return sphereDesc;
}

NxShapeDesc* CreateCapsule( NxActorDesc& ActorDesc, const PhysicBody* Pb )
{
	NxCapsuleShapeDesc *capsuleDesc = new NxCapsuleShapeDesc;
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);		

	capsuleDesc->radius			=(NxReal)Pb->bodySize.x ;													//Rayon
	capsuleDesc->height			=(NxReal)Pb->bodySize.y ;													//Hauteur
	capsuleDesc->mass			= Pb->fMass;																//Masse
	capsuleDesc->localPose.t	= VecToNxVec( Pb->translate );												//Position
	capsuleDesc->group			= Pb->bIsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;						//Groupe
	//capsuleDesc->group			= group;																	//Groupe
	capsuleDesc->materialIndex	= GenMaterial(Pb->restitution, Pb->staticFriction, Pb->dynamiqueFriction);	//Materiel			
	capsuleDesc->localPose.M	= m;																		//Rotation
	if(Pb->type == TRIGGER) 
		capsuleDesc->shapeFlags |= NX_TRIGGER_ENABLE;														//Trigger
	assert(capsuleDesc->isValid());

	ActorDesc.shapes.push_back(capsuleDesc);
	return capsuleDesc;
}

void FinalizeActor( NxActorDesc ActorDesc, const Vector3f Pos, const PhysicBody* Pb, GroupCollision group )
{
	ActorDesc.density		= 1.0f;
	ActorDesc.globalPose.t	= VecToNxVec(Pos);

	if(Pb->type == TRIGGER)
	{
		SetTriggerFunctions(ActorDesc, Pb->OnEnterFunc, Pb->OnLeaveFunc, Pb->OnStayFunc);
	}
	else
	{
		if(!Pb->bIsDynamic || group == GROUP_STATIC)
			ActorDesc.body = NULL;
		else
		{
			NxBodyDesc bodyDesc;
			bodyDesc.angularDamping = 0.5f;
			bodyDesc.linearVelocity = VecToNxVec( Pb->linearVelocity );
			bodyDesc.angularVelocity = VecToNxVec( Pb->angularVelocity );
			if( group == GROUP_WEAPON) bodyDesc.flags |= NX_BF_FROZEN;
			assert(bodyDesc.isValid());
			ActorDesc.body = &bodyDesc;
		}
	}

	assert(ActorDesc.isValid());

	NxScene* scene = physX::getPhysicScene();
	NxActor* pActor = scene->createActor( ActorDesc );
	pActor->setGroup( group );
	assert(pActor);
}


NxMaterialIndex GenMaterial( const float restitution, const float staticFriction, const float dynamiqueFriction )
{
	NxScene* Scene = physX::getPhysicScene();
	NxMaterialDesc materialDesc;
	materialDesc.restitution		= restitution;
	materialDesc.dynamicFriction	= staticFriction;
	materialDesc.staticFriction		= dynamiqueFriction;
	assert(materialDesc.isValid());
	return Scene->createMaterial(materialDesc)->getMaterialIndex();
}

//////////////////////////////////////////////////////////////////////////
int physX::CreateBoundingBox( ListOfBoundingBox &BBList, GroupCollision group )
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
			case BOX	 : BBList.pushShapeRef( CreateBox(actorDesc, Pb) );		break;
			case SPHERE	 : BBList.pushShapeRef( CreateSphere(actorDesc, Pb) );	break;
			case CAPSULE : BBList.pushShapeRef( CreateCapsule(actorDesc, Pb) ); break;
		}
	}

	FinalizeActor(actorDesc, BBList.getInitialWorldPos(), Pb, group);
	return physX::getPhysicScene()->getNbActors() - 1;
}


int physX::CreateTrigger(ListOfBoundingBox &BBList,
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
		Pb->OnEnterFunc = OnEnterFunc;
		Pb->OnLeaveFunc = OnLeaveFunc;
		Pb->OnStayFunc = OnStayFunc;

		switch( Pb->type )
		{
			case BOX	 : BBList.pushShapeRef( CreateBox(actorDesc, Pb) );		break;
			case SPHERE	 : BBList.pushShapeRef( CreateSphere(actorDesc, Pb) );	break;
			case CAPSULE : BBList.pushShapeRef( CreateCapsule(actorDesc, Pb) ); break;
		}
	}

	FinalizeActor(actorDesc, BBList.getInitialWorldPos(), Pb, GROUP_DYNAMIQUE);
	return physX::getPhysicScene()->getNbActors() - 1;
}


void physX::releaseActor(int &empActor)
{
	NxActor* pActor = getActor( empActor );
	if(pActor->userData) delete pActor->userData;
	getPhysicScene()->releaseActor(*pActor);

	empActor = -1;
}

NxScene* physX::getPhysicScene()
{
	Physicalizer* physInstance = Physicalizer::GetInstance();
	NxScene* pscene = physInstance->getScene();
	assert(pscene);

	return pscene;
}

NxActor* physX::getActor( int emp )
{
	if(emp == -1) return NULL;
	NxActor* pactor = getPhysicScene()->getActors()[ emp ];
	assert(pactor);

	return pactor; 
}

NxControllerManager* physX::getControllerManager()
{
	Physicalizer* physInstance = Physicalizer::GetInstance();
	NxControllerManager* pControllerManager = physInstance->getControllerManager();
	assert(pControllerManager);

	return pControllerManager;
}

NxController* physX::getController( int emp )
{
	if(emp == -1) return NULL;
	NxController* pController = getControllerManager()->getController( emp );
	assert(pController);

	return pController;
}

void physX::Link( SceneObject* const obj1, SceneObject* const obj2 )
{
	NxActor* pactor1  =  getActor(obj1->getEmpActor());
	NxActor* pactor2  =  getActor(obj2->getEmpActor());
	getPhysicScene()->setActorPairFlags( *pactor1, *pactor2, NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH );
}


//////////////////////////////////////////////////////////////////////////
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
		Sd = NULL;
	}	
	m_ShapeRefList.clear();
}
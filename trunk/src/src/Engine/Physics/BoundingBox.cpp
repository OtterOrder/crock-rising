#include "BoundingBox.h"
#include "Physicalizer.h"


NxShapeDesc* CreateBox( NxActorDesc& ActorDesc, const PhysicBody* Pb, bool bTrigger, void* paramE = NULL, void* paramL = NULL, void* paramS = NULL );
NxShapeDesc* CreateSphere( NxActorDesc& ActorDesc, const PhysicBody* Pb, bool bTrigger, void* paramE = NULL, void* paramL = NULL, void* paramS = NULL );
NxShapeDesc* CreateCapsule( NxActorDesc& ActorDesc, const PhysicBody* Pb, bool bTrigger, void* paramE = NULL, void* paramL = NULL, void* paramS = NULL );
void FinalizeActor(const NxActorDesc ActorDesc, const Vector3f Pos, const PhysicBody* Pb, bool bTrigger);
NxMaterialIndex GenMaterial( const float restitution, const float staticFriction, const float dynamiqueFriction );


NxShapeDesc* CreateBox( NxActorDesc& ActorDesc, const PhysicBody* Pb, bool bTrigger,
					   void* paramE, void* paramL, void* paramS )
{
	NxBoxShapeDesc *boxDesc = new NxBoxShapeDesc;	
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);			

	boxDesc->dimensions		= VecToNxVec( Pb->bodySize);													//Dimension
	boxDesc->mass			= Pb->fMass;																	//Masse
	boxDesc->localPose.t	= NxVec3( Pb->localPos.x, Pb->localPos.z, Pb->localPos.y );	
	boxDesc->group			= Pb->IsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;//Groupe
	boxDesc->materialIndex	= GenMaterial(Pb->frestitution,
										  Pb->fstaticFriction, Pb->fdynamiqueFriction);	//Materiel
	boxDesc->localPose.M	= m;																			//Rotation
	if(bTrigger) 
	{
		boxDesc->shapeFlags |= NX_TRIGGER_ENABLE;															//Trigger
		SetTriggerFunctions(*boxDesc, Pb->OnEnterFunc, paramE, Pb->OnLeaveFunc, paramL, Pb->OnStayFunc, paramS);
	}
	assert(boxDesc->isValid());

	ActorDesc.shapes.push_back(boxDesc);
	return boxDesc;
}

NxShapeDesc* CreateSphere( NxActorDesc& ActorDesc, const PhysicBody* Pb, bool bTrigger,
						  void* paramE, void* paramL, void* paramS )
{
	NxSphereShapeDesc *sphereDesc = new NxSphereShapeDesc;
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);					

	sphereDesc->radius			= (NxReal)Pb->bodySize.x ;													//Rayon
	sphereDesc->mass			= Pb->fMass;																//Masse
	sphereDesc->localPose.t		= VecToNxVec( Pb->localPos );												//Position
	sphereDesc->group			= Pb->IsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;//Groupe
	sphereDesc->materialIndex	= GenMaterial(Pb->frestitution, Pb->fstaticFriction, Pb->fdynamiqueFriction);	//Materiel
	sphereDesc->localPose.M		= m;																		//Rotation
	if(bTrigger) 
	{
		sphereDesc->shapeFlags |= NX_TRIGGER_ENABLE;															//Trigger
		SetTriggerFunctions(*sphereDesc, Pb->OnEnterFunc, paramE, Pb->OnLeaveFunc, paramL, Pb->OnStayFunc, paramS);
	}
	assert(sphereDesc->isValid());

	ActorDesc.shapes.push_back(sphereDesc);
	return sphereDesc;
}

NxShapeDesc* CreateCapsule( NxActorDesc& ActorDesc, const PhysicBody* Pb, bool bTrigger,
						   void* paramE, void* paramL, void* paramS )
{
	NxCapsuleShapeDesc *capsuleDesc = new NxCapsuleShapeDesc;
	NxMat33 m; m.id(); m.fromQuat(Pb->rotate);		

	capsuleDesc->radius			=(NxReal)Pb->bodySize.x ;													//Rayon
	capsuleDesc->height			=(NxReal)Pb->bodySize.y ;													//Hauteur
	capsuleDesc->mass			= Pb->fMass;																//Masse
	capsuleDesc->localPose.t	= VecToNxVec( Pb->localPos );												//Position
	capsuleDesc->group			= Pb->IsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;//Groupe
	capsuleDesc->materialIndex	= GenMaterial(Pb->frestitution, Pb->fstaticFriction, Pb->fdynamiqueFriction);	//Materiel			
	capsuleDesc->localPose.M	= m;																		//Rotation
	if(bTrigger) 
	{
		capsuleDesc->shapeFlags |= NX_TRIGGER_ENABLE;															//Trigger
		SetTriggerFunctions(*capsuleDesc, Pb->OnEnterFunc, paramE, Pb->OnLeaveFunc, paramL, Pb->OnStayFunc, paramS);
	}
	assert(capsuleDesc->isValid());

	ActorDesc.shapes.push_back(capsuleDesc);
	return capsuleDesc;
}

void FinalizeActor( NxActorDesc ActorDesc, const Vector3f Pos, const PhysicBody* Pb, bool bTrigger)
{
	ActorDesc.density		= 1.0f;
	ActorDesc.globalPose.t	= VecToNxVec(Pos);

	if(!bTrigger) 
	{
		if(!Pb->IsDynamic)
			ActorDesc.body = NULL;
		else
		{
			NxBodyDesc bodyDesc;
			bodyDesc.angularDamping = 0.5f;
			bodyDesc.linearVelocity = VecToNxVec( Pb->linearVelocity );
			bodyDesc.angularVelocity = VecToNxVec( Pb->angularVelocity );
			//if( Pb->physObjType == PHYS_WEAPON) bodyDesc.flags |= NX_BF_FROZEN;
			assert(bodyDesc.isValid());
			ActorDesc.body = &bodyDesc;
		}
	}

	assert(ActorDesc.isValid());

	NxScene* scene = physX::getPhysicScene();
	NxActor* pActor = scene->createActor( ActorDesc );
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
int physX::CreateBoundingBox( ListOfBoundingBox &BBList)
{
	NxActorDesc actorDesc;
	PhysicBody* Pb;
	std::vector<PhysicBody*> PbList = BBList.getPbList();
	std::vector<PhysicBody*>::iterator it = PbList.begin();

	while(it < PbList.end())
	{
		Pb = *it++;

		switch( Pb->shapeType )
		{
			case BOX	 : BBList.pushShapeRef( CreateBox(actorDesc, Pb, false) );		break;
			case SPHERE	 : BBList.pushShapeRef( CreateSphere(actorDesc, Pb, false) );	break;
			case CAPSULE : BBList.pushShapeRef( CreateCapsule(actorDesc, Pb, false) ); break;
		}
	}

	FinalizeActor(actorDesc, BBList.getInitialWorldPos(), Pb, false);
	return physX::getPhysicScene()->getNbActors() - 1;
}


int physX::CreateTrigger(ListOfBoundingBox &BBList,
	void (*OnEnterFunc)(void* param),
	void (*OnLeaveFunc)(void* param), 
	void (*OnStayFunc)(void* param),
	void* paramEnter, void* paramLeave, void* paramStay )
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

		switch( Pb->shapeType )
		{
			case BOX	 : BBList.pushShapeRef( CreateBox(actorDesc, Pb, true, paramEnter, paramLeave, paramStay) );		break;
			case SPHERE	 : BBList.pushShapeRef( CreateSphere(actorDesc, Pb, true, paramEnter, paramLeave, paramStay) );	break;
			case CAPSULE : BBList.pushShapeRef( CreateCapsule(actorDesc, Pb, true, paramEnter, paramLeave, paramStay) ); break;
		}
	}

	FinalizeActor(actorDesc, BBList.getInitialWorldPos(), Pb, true);
	return physX::getPhysicScene()->getNbActors() - 1;
}

void physX::releaseActor(int &empActor)
{
	NxActor* pActor = getActor( empActor );
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

void physX::Link( SceneObject* const obj1, SceneObject* const obj2 )
{
	NxActor* pactor1  =  getActor(obj1->getEmpActor());
	NxActor* pactor2  =  getActor(obj2->getEmpActor());
	getPhysicScene()->setActorPairFlags( *pactor1, *pactor2, NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH );
}

void Update( NxActor* const pActor, D3DXMATRIX &WorldMat, Vector3f const reg )
{
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
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

	boxDesc->dimensions		= VecToNxVec( Pb->bodySize);													//Dimension
	boxDesc->mass			= Pb->fMass;																	//Masse
	boxDesc->localPose.t	= NxVec3( Pb->localPos.x, Pb->localPos.z, Pb->localPos.y );	
	//boxDesc->group			= Pb->bIsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;								//Groupe
	boxDesc->materialIndex	= GenMaterial(Pb->frestitution,
										  Pb->fstaticFriction, Pb->fdynamiqueFriction);	//Materiel
	boxDesc->localPose.M	= m;																			//Rotation
	if(Pb->bIsTrigger) 
		boxDesc->shapeFlags |= NX_TRIGGER_ENABLE;															//Trigger
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
	sphereDesc->localPose.t		= VecToNxVec( Pb->localPos );												//Position
	sphereDesc->group			= Pb->bIsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;							//Groupe
	sphereDesc->materialIndex	= GenMaterial(Pb->frestitution, Pb->fstaticFriction, Pb->fdynamiqueFriction);	//Materiel
	sphereDesc->localPose.M		= m;																		//Rotation
	if(Pb->bIsTrigger) 
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
	capsuleDesc->localPose.t	= VecToNxVec( Pb->localPos );											//Position
	capsuleDesc->group			= Pb->bIsDynamic ? GROUP_DYNAMIQUE : GROUP_STATIC;							//Groupe
	capsuleDesc->materialIndex	= GenMaterial(Pb->frestitution, Pb->fstaticFriction, Pb->fdynamiqueFriction);	//Materiel			
	capsuleDesc->localPose.M	= m;																		//Rotation
	if(Pb->bIsTrigger) 
		capsuleDesc->shapeFlags |= NX_TRIGGER_ENABLE;														//Trigger
	assert(capsuleDesc->isValid());

	ActorDesc.shapes.push_back(capsuleDesc);
	return capsuleDesc;
}

void FinalizeActor( NxActorDesc ActorDesc, const Vector3f Pos, const PhysicBody* Pb, GroupCollision group )
{
	ActorDesc.density		= 1.0f;
	ActorDesc.globalPose.t	= VecToNxVec(Pos);

	if(Pb->bIsTrigger) 
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
		Pb->bIsTrigger = false;

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
		Pb->bIsTrigger = true;

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


void physX::UpdateObjectFromActor( int emp, D3DXMATRIX &WorldMat, Vector3f const reg, bool UpdateStatic )
{	
	NxActor* pac = getActor(emp);
	if( !pac->isSleeping() || UpdateStatic ) //On ne met à jour qu'à condition que l'objet bouge ou qu'il soit statique
	{
		D3DXMATRIX mat_PhysX;
		D3DXMatrixIdentity(&mat_PhysX);


		D3DXMATRIX rotX;
		D3DXMatrixIdentity( &rotX );
		rotX._22=0; rotX._23=1;
		rotX._32=1; rotX._33=0;

		NxVec3 v = pac->getGlobalPosition();
		pac->getGlobalPose().getColumnMajor44( mat_PhysX );
		mat_PhysX = rotX * mat_PhysX;


		D3DXMatrixTranslation(&WorldMat, -reg.x, -reg.y, -reg.z);
		D3DXMatrixMultiply(&WorldMat, &WorldMat, &mat_PhysX);
	}
}

void Update( NxActor* const pActor, D3DXMATRIX &WorldMat, Vector3f const reg )
{
}

void physX::UpdateObjectFromController( int emp, D3DXMATRIX &WorldMat, Vector3f regPivotMesh)
{
	NxController* pController = getController(emp);
	NxExtendedVec3 pos = pController->getPosition();

	WorldMat._41 = (float)pos.x + regPivotMesh.x,
	WorldMat._42 = (float)pos.y + regPivotMesh.y,
	WorldMat._43 = (float)pos.z + regPivotMesh.z ;
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
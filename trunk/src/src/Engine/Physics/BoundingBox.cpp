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


void UpdateObjectFromActor( SceneObject* SceObj, bool UpdateStatic )
{	
	//NxActor* pac = getActor(emp);
	//if( !pac->isSleeping() || UpdateStatic ) //On ne met à jour qu'à condition que l'objet bouge ou qu'il soit statique
	//{
	//	D3DXMATRIX mat_PhysX;
	//	D3DXMatrixIdentity(&mat_PhysX);

	//	//Rotation pour redresser l'objet GRAPHIQUE
	//	D3DXMATRIX rotX, rotY, trans;
	//	D3DXMatrixIdentity( &rotX );
	//	D3DXMatrixTranslation(&trans, -reg.x, -reg.y, -reg.z);
	//	NxVec3 v = pac->getGlobalPosition();
	//	pac->getGlobalPose().getColumnMajor44( mat_PhysX );

	//	rotX._22=0; rotX._23=1;
	//	rotX._32=1; rotX._33=0;

	//	mat_PhysX = rotX * mat_PhysX;
	//	D3DXMatrixMultiply(&WorldMat, &trans, &mat_PhysX);
	//}
}

void Update( NxActor* const pActor, D3DXMATRIX &WorldMat, Vector3f const reg )
{
}

void UpdateObjectFromController( SceneObject* SceObj )
{
	//NxController* pController = getController(SceObj->getEmpController());

	//NxExtendedVec3 NxPos = pController->getPosition();
	//Vector3f pos ((float)NxPos.x, (float)NxPos.y, (float)NxPos.z);
	//pos += SceObj->GetMesh()->;

	//D3DXMATRIX trans, rotX, rotY, rotZ, rot, result;
	//D3DXMatrixTranslation( &trans, pos.x, pos.y, pos.z);
	//D3DXMatrixRotationX(&rotX,  D3DXToRadian(SceObj->m_vAngleX -90.f));
	//D3DXMatrixRotationY(&rotY,  D3DXToRadian(SceObj->m_vAngleY ));
	//D3DXMatrixRotationZ(&rotZ,  D3DXToRadian(SceObj->m_vAngleZ ));

	//rot=rotX*rotY;

	//rotX._41 = pos.x;
	//rotX._42 = pos.y;
	//rotX._43 = pos.z;

	//SceObj->GetWorldMatrix() = rot;
	//D3DXMatrixMultiply( &SceObj->GetWorldMatrix(), &rot, &trans);
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
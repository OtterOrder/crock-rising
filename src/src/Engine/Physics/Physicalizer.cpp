// ===============================================================================
//
//
// ===============================================================================
#include "Physicalizer.h"
#include "Trigger/Trigger.h"

extern TriggerReport gTriggerReport;

bool Physicalizer::InitPhysX()
{
	m_PhysicsSDK = NULL;
	m_Scene		 = NULL; 
	m_DeltaTime	 = (NxReal)(1.0 / 60.0);
	m_Gravity	 = Vector3f( 0.f, -9.8f, 0.f );

	m_Allocator = new NxUserAllocatorDefault();
	m_ControllerManager = NxCreateControllerManager( m_Allocator );

	// Initialize PhysicsSDK
	m_PhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION );

	// Initialiser le sdk
	if(m_PhysicsSDK == NULL)
	{
		assert( m_PhysicsSDK );
		return false;
	}

	m_PhysicsSDK->setParameter(NX_SKIN_WIDTH,				  m_AdvancedParam.SkinWidth);
	//m_PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE,		  m_AdvancedParam.VisualisationScale);
	//m_PhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES,		  m_AdvancedParam.VisualizeActorAxe);
	//m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, m_AdvancedParam.VisualizeCollisionShape);
	//m_PhysicsSDK->setParameter(NX_VISUALIZE_CLOTH_SLEEP,	  m_AdvancedParam.VisualizeClothSleep);

	connectToVRD();

    NxSceneDesc sceneDesc;
 	sceneDesc.simType				= NX_SIMULATION_HW; //avec carte accé PhysX
	sceneDesc.gravity               = NxVec3(m_Gravity.x, m_Gravity.y, m_Gravity.z) ;
    m_Scene = m_PhysicsSDK->createScene(sceneDesc);	

	if(!m_Scene)
	{ 
		sceneDesc.simType = NX_SIMULATION_SW; 
		m_Scene = m_PhysicsSDK->createScene(sceneDesc);	
		assert( m_Scene );
		return false;
	}
	

	//Création du manager des objets trigger 
	m_Scene->setUserTriggerReport(&gTriggerReport);
	gTriggerReport.scene = m_Scene;

	// Set default material
	NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.1f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	/*NxPlaneShapeDesc planeDesc;
	planeDesc.group = GROUP_STATIC;
	//planeDesc.d = -25.f;
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	m_Scene->createActor(actorDesc);*/

	// Récupération du temps
	UpdateTime();

	// Lance la première frame
	if (m_Scene)  StartPhysics();

	return true;
}

void Physicalizer::ExitPhysX()
{
	if(m_PhysicsSDK != NULL)
	{
		if(m_Scene != NULL) m_PhysicsSDK->releaseScene(*m_Scene);
		m_Scene = NULL;
		NxReleasePhysicsSDK(m_PhysicsSDK);
		m_PhysicsSDK = NULL;
	}
}

bool Physicalizer::ReloadPhysX()
{
	ExitPhysX();
	return InitPhysX();
}

void Physicalizer::StartPhysics()
{
	// Update the time step
	//m_DeltaTime = UpdateTime();

	//if( m_ControllerManager )
	//	m_ControllerManager->updateControllers();

	// Start collision and dynamics for delta time since the last frame
	m_Scene->simulate( NxReal(0.01667) );//m_DeltaTime
	m_Scene->flushStream();
	
}

void Physicalizer::GetPhysicsResults()
{
	// Get results from gScene->simulate(gDeltaTime)
	m_Scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
}

PhysXResult Physicalizer::RunPhysics()
{
	if( m_Scene )
	{
		StartPhysics();
		GetPhysicsResults();

		return DoTransform();
	}
	return PHYSX_FAILED;
}

void Physicalizer::connectToVRD()
{ 	
#if defined( _DEBUG ) && defined( CONNECT_VRD )
	m_PhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect( "localhost");

	bool connectToVisualRemoteDebugger;
	if( (connectToVisualRemoteDebugger = m_PhysicsSDK->getFoundationSDK().getRemoteDebugger()->isConnected())==0 )
	{
		assert( connectToVisualRemoteDebugger );
	}
#endif
}

PhysXResult Physicalizer::DoTransform()
{	//matrice obtenue de PhysX
	std::list< SceneObject* > List = SceneObject::RefList;
	std::list< SceneObject* >::iterator it = List.begin();
	int Size = List.size();

	while( it != List.end() )
	{
		SceneObject* aSObj = (SceneObject*)*it; 
		PhysicalObjectType type = IsPhysical(aSObj);
		switch (type)
		{		
			case ACTOR :
				{
					int emp = aSObj->getEmpActor();
					if(emp < 0) return PHYSX_FAILED;

					NxActor** ac =  getScene()->getActors(); //La liste des acteurs
					NxActor* pac = ac[ emp ];				 //Pointeur sur l'acteur qui va bien
					D3DXMATRIX WorldMat;
					pac->getGlobalPose().getColumnMajor44( WorldMat );
					//aSObj->BerSetTransform( &WorldMat ); // Je comprends pas la nouvelle manière de faire les transforme !!
					aSObj->ApplyTransform( &WorldMat ); // Je comprends pas la nouvelle manière de faire les transforme !!
					break;
				}
			case CONTROLLER :
				{
					int emp = aSObj->getEmpController();
					if(emp < 0) return PHYSX_FAILED;

					m_ControllerManager->updateControllers();
					NxController* pController = m_ControllerManager->getController( emp );
					NxExtendedVec3 pos = pController->getPosition();


					NxActor** ac =  getScene()->getActors(); //La liste des acteurs
					NxActor* pac = ac[ emp ];				 //Pointeur sur l'acteur qui va bien
					D3DXMATRIX WorldMat;
					D3DXMatrixTranslation(&WorldMat, (float)pos.x, (float)pos.y, (float)pos.z);

					//aSObj->BerSetTransform( &WorldMat ); // Je comprends pas la nouvelle manière de faire les transforme !!
					aSObj->ApplyTransform( &WorldMat ); // Je comprends pas la nouvelle manière de faire les transforme !!
					break;
				}
			default : break;
		}
		++it;
	}
	return PHYSX_SUCCEED;
}

PhysicalObjectType Physicalizer::IsPhysical( SceneObject* SceObj )
{
	if(SceObj) 
	{
		if(SceObj->getEmpActor() >= 0) return ACTOR;
		if(SceObj->getEmpController() >= 0) return CONTROLLER;
	}
	return NOPHYSICAL;
}

NxScene* GetPhysicScene()
{
	Physicalizer* physInstance = Physicalizer::GetInstance();
	return physInstance->getScene();
}
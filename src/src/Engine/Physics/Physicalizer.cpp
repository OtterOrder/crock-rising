// ===============================================================================
//
//
// ===============================================================================
#include "Physicalizer.h"
#include	"Trigger/Trigger.h"

extern TriggerReport gTriggerReport;

bool Physicalizer::InitPhysX()
{
	m_PhysicsSDK = NULL;
	m_Scene		 = NULL; 
	m_DeltaTime	 = (NxReal)(1.0 / 60.0);
	m_Gravity	 = Vector3f( 0.f, -9.8f, 0.f );

	// Initialize PhysicsSDK
	//NxSDKCreateError errorCode = NXCE_NO_ERROR;
	m_PhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);

	// Initialiser le sdk
	if(m_PhysicsSDK == NULL) return false;

	m_PhysicsSDK->setParameter(NX_SKIN_WIDTH,				  m_AdvancedParam.SkinWidth);
	m_PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE,		  m_AdvancedParam.VisualisationScale);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES,		  m_AdvancedParam.VisualizeActorAxe);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, m_AdvancedParam.VisualizeCollisionShape);
	//	m_PhysicsSDK->setParameter(NX_VISUALIZE_CLOTH_SLEEP,	  m_AdvancedParam.VisualizeClothSleep);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_FNORMALS, 1);


	// Create a scene
    NxSceneDesc sceneDesc;
	sceneDesc.gravity               = NxVec3(m_Gravity.x, m_Gravity.y, m_Gravity.z);
 	sceneDesc.simType				= NX_SIMULATION_HW;
    m_Scene = m_PhysicsSDK->createScene(sceneDesc);	

	if(!m_Scene)
	{ 
		sceneDesc.simType = NX_SIMULATION_SW; 
		m_Scene = m_PhysicsSDK->createScene(sceneDesc);	
		assert( m_Scene );
	}

	//Créer une scene (gScene)
	if(m_Scene == NULL) 
	{
		printf("\nError: Unable to create a PhysX scene, exiting the sample.\n\n");
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


	NxPlaneShapeDesc planeDesc;
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	m_Scene->createActor(actorDesc);

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

PhysXResult Physicalizer::DoTransform()
{	//matrice obtenue de PhysX
	std::list< SceneObject* > List = SceneObject::RefList;
	std::list< SceneObject* >::iterator it = List.begin();
	int Size = List.size();

	while( it != List.end() )
	{
		SceneObject* aSObj = *it; 
		if (IsPhysicable(aSObj))
		{
			int emp = (*aSObj->getEmpList()->begin());
			if(emp < 0) return PHYSX_FAILED;

			NxActor** ac =  getScene()->getActors(); //La liste des acteurs
			NxActor* pac = ac[ emp ];				 //Pointeur sur l'acteur qui va bien

			D3DXMATRIX WorldMat;
			pac->getGlobalPose().getColumnMajor44( WorldMat );

			D3DXMATRIX tmp;
		/*	tmp._11 = WorldMat._11, tmp._12 = WorldMat._21, tmp._13 = WorldMat._31;
			tmp._21 = WorldMat._12, tmp._22 = WorldMat._22, tmp._23 = WorldMat._32;
			tmp._31 = WorldMat._13, tmp._32 = WorldMat._23, tmp._33 = WorldMat._33;*/

			tmp._11 = WorldMat._11, tmp._12 = WorldMat._12, tmp._13 = WorldMat._13;
			tmp._21 = WorldMat._21, tmp._22 = WorldMat._22, tmp._23 = WorldMat._23;
			tmp._31 = WorldMat._31, tmp._32 = WorldMat._32, tmp._33 = WorldMat._33;

			tmp._41 = WorldMat._41;
			tmp._42 = WorldMat._42;
			tmp._43 = WorldMat._43;

			tmp._14 = tmp._24 = tmp._34 = 0.0f;
			tmp._44 = 1.0f;


			aSObj->ApplyTransform( &tmp );
			//aSObj->SetTransform( &WorldMat );
		}
		++it;
	}
	return PHYSX_SUCCEED;
}

PhysXResult Physicalizer::SetPhysicable( SceneObject* SceObj, BoundingBox* bb )
{
	if(SceObj) //Le SceneObject est valide, OK
	{
		if(bb) //La BB existe donc on l'applique
			SceObj->getEmpList()->push_front( bb->getEmplacement() );
		else //Elle ne l'est pas donc on retire le SceneObject de la liste si il y est.
			if(IsPhysicable(SceObj))
				SceObj->getEmpList()->clear();

		return PHYSX_SUCCEED;
	}
	return PHYSX_FAILED; // Le SceneObject n'est pas bon, probleme 
}

bool Physicalizer::IsPhysicable( SceneObject* SceObj )
{
	if(SceObj) 
		return !SceObj->getEmpList()->empty();
	return false;
}

// ===============================================================================
//
//
// ===============================================================================
#include "Physicalizer.h"
#include "Trigger/Trigger.h"
#include "Physics/ControledPhysicalCharacter.h"

TriggerReport*  gTriggerReport					= new TriggerReport;

HeroHitReport*	gCharacterControllerCallback	= new HeroHitReport;
ContactReport*  gContactReport					= new ContactReport;

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

	//m_PhysicsSDK->setParameter( NX_SKIN_WIDTH, 0.01 );

	connectToVRD();

	gTriggerReport->scene = m_Scene;

    NxSceneDesc sceneDesc;
 	sceneDesc.simType				= NX_SIMULATION_SW; //avec carte accé PhysX	
	sceneDesc.userContactReport		= gContactReport;
	sceneDesc.userTriggerReport		= gTriggerReport;
	sceneDesc.gravity               = NxVec3(m_Gravity.x, m_Gravity.y, m_Gravity.z) ;
    m_Scene = m_PhysicsSDK->createScene(sceneDesc);	

	if(!m_Scene)
	{ 
		sceneDesc.simType = NX_SIMULATION_SW; 
		m_Scene = m_PhysicsSDK->createScene(sceneDesc);	
		assert( m_Scene );
	}
	
	
	// Set default material
	NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.1f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	// Lance la première frame
	if (m_Scene)  StartPhysics(0.015);

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

void Physicalizer::StartPhysics(float DeltaTime)
{
	m_Scene->simulate( DeltaTime );
	m_Scene->flushStream();
	
}

void Physicalizer::GetPhysicsResults()
{
	m_Scene->fetchResults(NX_ALL_FINISHED, true); //ou NX_RIGID_BODY_FINISHED
}

PhysXResult Physicalizer::RunPhysics(float DeltaTime)
{
	if( m_Scene )
	{
		m_ControllerManager->updateControllers();

		StartPhysics(DeltaTime);
		GetPhysicsResults();
		return PHYSX_SUCCEED;
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
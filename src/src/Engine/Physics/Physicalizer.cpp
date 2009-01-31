// ===============================================================================
//
//
// ===============================================================================
#include "Physicalizer.h"

#include <stdio.h>
#include "NxPhysics.h"


bool Physicalizer::InitPhysX()
{
	m_PhysicsSDK = NULL;
	m_Scene		 = NULL; 
	m_DeltaTime	 = (NxReal)(1.0 / 60.0);

	// Initialize PhysicsSDK
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	m_PhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);

	// Initialiser le sdk
	if(m_PhysicsSDK == NULL) return false;

	m_PhysicsSDK->setParameter(NX_SKIN_WIDTH,				  m_AdvancedParam.SkinWidth);
	m_PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE,		  m_AdvancedParam.VisualisationScale);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES,		  m_AdvancedParam.VisualizeActorAxe);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, m_AdvancedParam.VisualizeCollisionShape);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_CLOTH_SLEEP,	  m_AdvancedParam.VisualizeClothSleep);


	// Create a scene
    NxSceneDesc sceneDesc;
 	sceneDesc.simType				= NX_SIMULATION_SW;
   // sceneDesc.gravity               = gDefaultGravity;
    m_Scene = m_PhysicsSDK->createScene(sceneDesc);	

	// Créer une scene (gScene)
	if(m_Scene == NULL) 
	{
		printf("\nError: Unable to create a PhysX scene, exiting the sample.\n\n");
		return false;
	}
	// Set default material
	NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.5f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	// Create ground plane
	NxPlaneShapeDesc planeDesc; //Contient les caracteristiques du plan
	NxActorDesc actorDesc;		//Acteur qui contiendra le plan
	
	// Créer l'acteur "plan du sol"
    actorDesc.shapes.pushBack(&planeDesc);//On rajoute la boite englobante de ce plan
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
//static void CreatePlane(
void Physicalizer::CreateCube(const NxVec3& pos, int size, const NxVec3* initialVelocity)
{
	if(m_Scene == NULL) return;	

	// Create body
 	/*NxBodyDesc bodyDesc;
	NxBoxShapeDesc boxDesc;
	NxActorDesc actorDesc;

	bodyDesc.angularDamping	= 0.5f;
	if(initialVelocity) bodyDesc.linearVelocity = *initialVelocity;

	boxDesc.mass = 1;
	boxDesc.dimensions.set(size);
	//boxDesc.localPose.t.set(pos);
	// Initialiser la description de la futur boite.

	// Créer l'acteur de la boite
	actorDesc.shapes.push_back(&boxDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.userData = (void*)size;

	actorDesc.density		= 1.0f;
	actorDesc.globalPose.t	= pos;	
	assert(actorDesc.isValid());
	NxActor *pActor = m_Scene->createActor(actorDesc);	
	assert(pActor);*/
}

void Physicalizer::CreateCubeFromEye(int size)
{
	//HeavyCreateCube(gEye, 1.0, &tir);

	// Créer un cube, centré sur gEye, et qui est lancé dans le sens du regard (gDir).
}

void Physicalizer::CreateStack(int size)
{
	float cubeSize = 1.0f;
	float spacing = 1.0f;
	NxVec3 pos(0.0, cubeSize, 0.0);

	int nbligne = size;
	int nbcol = size;

	for(int y = 0; y < size; y++)
	{
		for(int x = y; x < nbligne; x++)
		{
			for(int z = y; z < nbcol; z++)
			{
				pos.set((cubeSize + spacing)*x, cubeSize + (cubeSize + spacing)*y , (cubeSize + spacing)*z);
				CreateCube(pos, (int)cubeSize, NULL);
			}
		}
		nbligne-= 1;
		nbcol-= 1;
	}
	// Créer un tas de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}

void Physicalizer::CreateSin(int size)
{
	float cubeSize = 1.0f;
	float spacing = 1.0f;
	NxVec3 pos(0.0, cubeSize, 0.0);

	int Duree = 200;
	int Periode = size;
	float Intensite;
	for(int x = 0; x < Duree; x++)
	{
		Intensite = (float)( sin(x*(3.14/100))+1 ) /2 * Periode;
		for(int y = 0; y < Intensite; y++)
		{			
			pos.set((cubeSize + spacing)*x, cubeSize + (cubeSize + spacing)*y , 0.0f);
			CreateCube(pos, (int)cubeSize, NULL);
		}
	}
	// Créer un tas de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}

//Hauteur : Hauteur des pics en Y
//Duree	  : Nombre de cube en X et en Z du premier étage
//Période : Nombre de bosses
void Physicalizer::CreateSinSuface(int Hauteur, int Duree, int Periode)
{
	float cubeSize = 1.0f;
	float spacing = 0.01f;
	NxVec3 pos(0.0, cubeSize, 0.0);

	float Intensite1, Intensite2;
	for(int x = 0; x < Duree; x++)
	{
		Intensite1 = (float)( sin((x)*(3.14/Periode))+1 ) /2 * Hauteur;
		for(int z = 0; z < Duree; z++)
		{
			Intensite2 = (float)( sin((z)*(3.14/Periode))+1 ) /2 * Hauteur;
			for(int y = 0; y < Intensite2+Intensite1; y++)
			{			
				pos.set((2*cubeSize + spacing)*x, cubeSize + (2*cubeSize + spacing)*y , (2*cubeSize + spacing)*z);
				CreateCube(pos, (int)cubeSize, NULL);
			}
		}
	} 
	// Créer un tas de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}

void Physicalizer::CreateTower(int size)
{
	float cubeSize = 1.0f;
	float spacing = 1.0f;
	NxVec3 pos(0.0, cubeSize, 0.0);
	for(int i = 1; i <= size; i++)
	{
		CreateCube(pos, (int)cubeSize, NULL);
		pos.set(0.0f, cubeSize + (cubeSize + spacing)*i , 0.0f);
	}
	// Créer une 'tour' de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}

void Physicalizer::StartPhysics()
{
	// Update the time step
	m_DeltaTime = UpdateTime();

	// Start collision and dynamics for delta time since the last frame
	m_Scene->simulate(m_DeltaTime);
	m_Scene->flushStream();
}

void Physicalizer::GetPhysicsResults()
{
	// Get results from gScene->simulate(gDeltaTime)
	m_Scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
}

void Physicalizer::Update()
{
	GetPhysicsResults();
	StartPhysics();
}
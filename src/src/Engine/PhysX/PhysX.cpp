// ===============================================================================
//
//
// ===============================================================================

#include <stdio.h>

#include "NxPhysics.h"

// Physics
static NxPhysicsSDK*	gPhysicsSDK = NULL;
static NxScene*			gScene = NULL;
//static PerfRenderer    gPerfRenderer;
NxVec3            gDefaultGravity(0,-9.8,0);

// Rendering
static NxVec3	gEye(50.0f, 50.0f, 50.0f);
static NxVec3	gDir(-0.6f,-0.2f,-0.7f);
static NxVec3	gViewY;
static int		gMouseX = 0;
static int		gMouseY = 0;


static void CreateCube(const NxVec3& pos, int size=2, const NxVec3* initialVelocity=NULL);

static bool InitNx()
{
	// Initialize PhysicsSDK
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);

	// Initialiser le sdk
	if(gPhysicsSDK == NULL) 
	{
//		printf("\nSDK create error (%d - %s).\nUnable to initialize the PhysX SDK, exiting the sample.\n\n", errorCode, getNxSDKCreateError(errorCode));
		return false;
	}

	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.05f);
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);


	// Create a scene
    NxSceneDesc sceneDesc;
 	sceneDesc.simType				= NX_SIMULATION_SW;
    sceneDesc.gravity               = gDefaultGravity;
    gScene = gPhysicsSDK->createScene(sceneDesc);	

	// Créer une scene (gScene)
	if(gScene == NULL) 
	{
		printf("\nError: Unable to create a PhysX scene, exiting the sample.\n\n");
		return false;
	}

	// Set default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.5f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	// Create ground plane
	NxPlaneShapeDesc planeDesc; //Contient les caracteristiques du plan
	NxActorDesc actorDesc;		//Acteur qui contiendra le plan
	
	// Créer l'acteur "plan du sol"
    actorDesc.shapes.pushBack(&planeDesc);//On rajoute la boite englobante de ce plan
	gScene->createActor(actorDesc);

	return true;
}

static void ExitNx()
{
	if(gPhysicsSDK != NULL)
	{
		if(gScene != NULL) gPhysicsSDK->releaseScene(*gScene);
		gScene = NULL;
		NxReleasePhysicsSDK(gPhysicsSDK);
		gPhysicsSDK = NULL;
	}
}
//static void CreatePlane(
static void CreateCube(const NxVec3& pos, int size, const NxVec3* initialVelocity)
{
	if(gScene == NULL) return;	

	// Create body
 	NxBodyDesc bodyDesc;
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
	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);
}

static void HeavyCreateCube(const NxVec3& pos, int size, const NxVec3* initialVelocity)
{
	if(gScene == NULL) return;	

	// Create body
 	NxBodyDesc bodyDesc;
	NxBoxShapeDesc boxDesc;
	NxActorDesc actorDesc;

	bodyDesc.angularDamping	= 0.5f;
	if(initialVelocity) bodyDesc.linearVelocity = *initialVelocity;

	boxDesc.mass = 100;
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
	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);
}

static void CreateCubeFromEye(int size)
{
	const NxVec3 tir(gDir.x*size, gDir.y*size, gDir.z*size);
	HeavyCreateCube(gEye, 1.0, &tir);

	// Créer un cube, centré sur gEye, et qui est lancé dans le sens du regard (gDir).
}

static void CreateStack(int size)
{
	const float cubeSize = 1.0f;
	const float spacing = 1.0f;
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
				CreateCube(pos, cubeSize);
			}
		}
		nbligne-= 1;
		nbcol-= 1;
	}
	// Créer un tas de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}

static void CreateSin(int size)
{
	const float cubeSize = 1.0f;
	const float spacing = 1.0f;
	NxVec3 pos(0.0, cubeSize, 0.0);

	int Duree = 200;
	int Periode = size;
	float Intensite;
	for(int x = 0; x < Duree; x++)
	{
		Intensite = ( sin(x*(3.14/100))+1 ) /2 * Periode;
		for(int y = 0; y < Intensite; y++)
		{			
			pos.set((cubeSize + spacing)*x, cubeSize + (cubeSize + spacing)*y , 0.0f);
			CreateCube(pos, cubeSize);
		}
	}
	// Créer un tas de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}

//Hauteur : Hauteur des pics en Y
//Duree	  : Nombre de cube en X et en Z du premier étage
//Période : Nombre de bosses
static void CreateSinSuface(int Hauteur, int Duree, int Periode)
{
	const float cubeSize = 1.0f;
	const float spacing = 0.01f;
	NxVec3 pos(0.0, cubeSize, 0.0);

	float Intensite1, Intensite2;
	for(int x = 0; x < Duree; x++)
	{
		Intensite1 = ( sin((x)*(3.14/Periode))+1 ) /2 * Hauteur;
		for(int z = 0; z < Duree; z++)
		{
			Intensite2 = ( sin((z)*(3.14/Periode))+1 ) /2 * Hauteur;
			for(int y = 0; y < Intensite2+Intensite1; y++)
			{			
				pos.set((2*cubeSize + spacing)*x, cubeSize + (2*cubeSize + spacing)*y , (2*cubeSize + spacing)*z);
				CreateCube(pos, cubeSize);
			}
		}
	} 
	// Créer un tas de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}

static void CreateTower(int size)
{
	const float cubeSize = 1.0f;
	const float spacing = 1.f;
	NxVec3 pos(0.0, cubeSize, 0.0);
	for(int i = 1; i <= size; i++)
	{
		CreateCube(pos, cubeSize);
		pos.set(0.0f, cubeSize + (cubeSize + spacing)*i , 0.0f);
	}
	// Créer une 'tour' de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}


static void KeyboardCallback(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:	
			exit(0); 
			break;
		case '0':	
			//gPerfRenderer.toggleEnable(); 
			break;
		case ' ':	
			CreateCube(NxVec3(0.0f, 20.0f, 0.0f),1); 
			break;
		case 's':
		case 'S':
			CreateStack(10);
			break;
		case 'b':	
		case 'B':	
			CreateStack(30); 
			break;
		case 'c':	
		case 'C':	
			CreateSin(100); 
			break;
		case 'v':	
		case 'V':	
			CreateSinSuface(10, 50, 10); 
			break;
		case 't':
		case 'T':
			CreateTower(30);
			break;
		case 'w':
		case 'W':
			CreateCubeFromEye(100);
			break;
		case 'q':
		case 'Q':
			{
				NxActor** actors = gScene->getActors();
				if(gScene->getNbActors() > 1){
					gScene->releaseActor(*actors[gScene->getNbActors()-1]);
				}
			}
			break;		
	}
}

static void ArrowKeyCallback(int key, int x, int y)
{
	KeyboardCallback(key,x,y);
}

static void MouseCallback(int button, int state, int x, int y)
{
	gMouseX = x;
	gMouseY = y;
}

static void MotionCallback(int x, int y)
{
	int dx = gMouseX - x;
	int dy = gMouseY - y;
	
	gDir.normalize();
	gViewY.cross(gDir, NxVec3(0,1,0));

	NxQuat qx(NxPiF32 * dx * 20/ 180.0f, NxVec3(0,1,0));
	qx.rotate(gDir);
	NxQuat qy(NxPiF32 * dy * 20/ 180.0f, gViewY);
	qy.rotate(gDir);

	gMouseX = x;
	gMouseY = y;
}

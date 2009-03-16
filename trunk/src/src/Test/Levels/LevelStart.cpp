#define		NOMINMAX
#include	"LevelStart.h"

#include	<Core/Inputs/InputManager.h>
#include	<Objects/Object2D/Sprite.h>
#include	<Physics/BoundingBox.h>

static float angle=0.0f;

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelStart::LevelStart( crc32 levelID )
: Level( levelID )
{
	m_pDemoRising = NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelStart::~LevelStart( void )
{
	FreeInterface();
	
	/*if ( m_pSObject)
		delete m_pSObject;*/
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelStart::Init( void )
{
	m_pCamera = new Camera( Vector3f(0.0f, -10.0f, -30.0f) );
	Renderer::GetInstance()->SetCamera( m_pCamera );

	skyb=new Skybox("mars.dds");
	skyb->Init();
	Renderer::GetInstance()->SetSkybox(skyb);

	m_pSObjectAnimated = NULL;
	m_pSObjectAnimated = new SceneObjectAnimated("15_Mesh.DAE","15_Anim.DAE","",D3DXVECTOR3(0.f,0.f,0.f));//, NULL, "default.fx");
	m_pSObjectAnimated->InitObjectAnimation();

	m_Alien = NULL;
	m_Alien = new SceneObject("Alien1.DAE","",D3DXVECTOR3(0.f,25.f,0.f), "blinn.fx");
	m_Alien->InitObject();

	SceneObject* obj1 = NULL;
	obj1 = new SceneObject("canyon1.DAE","roche.jpg",D3DXVECTOR3(0.f,25.f,0.f), "default.fx");
	obj1->InitObject();
	Physicalizer::GetInstance()->SetPhysicable(obj1, &BoundingBox(Vector2f(100.f, 100.f), 0.f));

	/*SceneObject* obj2 = NULL;
	obj2 = new SceneObject("de.DAE","roche.jpg",D3DXVECTOR3(0.f,0.f,0.f), "default.fx");
	obj2->InitObject();*/

	InitInterface();
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	
	//OffsetCurseur -> Camera
	InputManager* pInputManager = InputManager::GetInstance();
	Point2f point = pInputManager->GetMouseOffset();
	
	const int sensibiliteSouris = 5;
	const float sensibiliteRoulette = 25.0f;
	const float sensibiliteTranslation = 0.1f;

	
	
	//Mouvement de l'objet
	if( pInputManager->IsKeyPressed( 'Z' ) )
	{
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep;

		xStep = -sin( angleY )*sensibiliteTranslation;
	 	zStep = cos( angleY )*sensibiliteTranslation;

		m_Alien->SetTranslation( xStep, 0.f, zStep );

	}
	if( pInputManager->IsKeyPressed( 'S' ) )
	{
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep;

		xStep = sin( angleY )*sensibiliteTranslation;
	 	zStep = -cos( angleY )*sensibiliteTranslation;

		m_Alien->SetTranslation( xStep, 0.f, zStep );

	}
	if( pInputManager->IsKeyPressed( 'Q' ) )
	{
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep;

		xStep = -cos( angleY )*sensibiliteTranslation;
	 	zStep = -sin( angleY )*sensibiliteTranslation;

		m_Alien->SetTranslation( xStep, 0.f, zStep );
	}
	if( pInputManager->IsKeyPressed( 'D' ) )
	{
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep;

		xStep = cos( angleY )*sensibiliteTranslation;
	 	zStep = sin( angleY )*sensibiliteTranslation;

		m_Alien->SetTranslation( xStep, 0.f, zStep );
	}



	//Mouvement de la souris = mouvement camera
	if( point.x != 0 ) 
	{
		int offsetCursor = (int)point.x%sensibiliteSouris; 
		m_pCamera->SetOrientationY( -offsetCursor );
		
		m_Alien->SetRotation( 0, offsetCursor, 0);
	}
	if( point.y != 0 ) 
	{
		int offsetCursor = (int)point.y%sensibiliteSouris; 
		m_pCamera->SetOrientationX( offsetCursor );
	}


	////Mouvement de la molette  = zoom sur l'alien
	if( pInputManager->GetMouseWheelDelta()>0 )
	{
		Vector3f posCam = m_pCamera->GetPosition();
		if( fabs(posCam.z) < 60 ) posCam.z--;

		m_pCamera->SetPosition( posCam );

		pInputManager->InitMouseWheelDelta();
	}

	if( pInputManager->GetMouseWheelDelta()<0 )
	{
		Vector3f posCam = m_pCamera->GetPosition();
		if( fabs(posCam.z) > 10 ) posCam.z++;

		m_pCamera->SetPosition( posCam );

		pInputManager->InitMouseWheelDelta();
	}


	m_pCamera->SetTarget( m_Alien );

	if( pInputManager->IsKeyReleased( 'O' ) )
		CreateStack(1.f, 5.0f, 10);

	if( pInputManager->IsKeyReleased( 'P' ) )
		CreateCubeFromEye(1.f, 100.f, 100);

	Physicalizer::GetInstance()->RunPhysics();

}

void LevelStart::InitInterface()
{
	m_pDemoRising = new Sprite( "demorising.png" );
	m_pDemoRising->SetPosition( 0.f, 0.f );
	m_pDemoRising->SetAlpha( 0.85f );
}

void LevelStart::FreeInterface()
{
	if( m_pDemoRising ) delete m_pDemoRising;
}

//////////////////////////////////////////////////////////////////////////
void LevelStart::CreateCube(float demiSize, Vector3f Pos, float density, Vector3f initialVel)
{
	SceneObject* aCube = NULL;

	aCube = new SceneObject("Sphere.DAE","",D3DXVECTOR3(0.f,0.f,0.f), "blinn.fx");
	Physicalizer::GetInstance()->SetPhysicable(aCube, &BoundingBox(1.f, Pos, density, initialVel));
	aCube->InitObject();
}

//////////////////////////////////////////////////////////////////////////
void LevelStart::CreateCubeFromEye(float demisize, float density, int Patate)
{
	D3DXMATRIX rotX, rotY;
	D3DXMatrixRotationX(&rotX, -m_pCamera->GetOrientationXRad());
	D3DXMatrixRotationY(&rotY, -m_pCamera->GetOrientationYRad());
	Vector3f Pos = m_pCamera->GetPosition();
	Vector4f Pos4;
	D3DXVec3Transform(&Pos4, &Pos, &rotX);
	Pos = Vector3f(Pos4.x, Pos4.y, Pos4.z);
	D3DXVec3Transform(&Pos4, &Pos, &rotY);
	Pos = Vector3f(Pos4.x, Pos4.y, Pos4.z);
	Vector3f V = m_pCamera->GetTarget()- Pos;
	Normalize(V);
	V *= (float)Patate;

	CreateCube(demisize, Pos, density, V);
}

//////////////////////////////////////////////////////////////////////////
void LevelStart::CreateStack(float demisize, float density, int StackSize)
{
	float cubeSize = demisize*2;
	float spacing = 0.01f;
	Vector3f pos(0.0, demisize, 0.0);

	int nbligne = StackSize;
	int nbcol = StackSize;

	for(int y = 0; y < StackSize; y++)
	{
		for(int x = y; x < nbligne; x++)
		{
			for(int z = y; z < nbcol; z++)
			{
				pos = Vector3f((cubeSize + spacing)*x, cubeSize + (cubeSize + spacing)*y + 10 , (cubeSize + spacing)*z);
				CreateCube(demisize, pos, density);
			}
		}
		nbligne-= 1;
		nbcol-= 1;
	}
	// Créer un tas de cubes (de la forme que vous voulez, il faut au moins 25 cubes).
}
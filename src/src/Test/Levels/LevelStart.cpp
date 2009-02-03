#define		NOMINMAX
#include	"LevelStart.h"

#include	<Core/Inputs/InputManager.h>
#include	<Objects/Object2D/Sprite.h>

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelStart::LevelStart( crc32 levelID )
: Level( levelID )
{
	//CBaseCamera
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelStart::~LevelStart( void )
{
	/*if( m_pSprite )
		delete m_pSprite;*/

	/*if ( m_pSObject)
		delete m_pSObject;*/
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelStart::Init( void )
{
	m_pCamera = new Camera( Vector3f(0.0f, 10.0f, -200.0f) );
	Renderer::GetInstance()->SetCamera( m_pCamera );

	skyb=new Skybox("mars.dds");
	skyb->Init();
	Renderer::GetInstance()->SetSkybox(skyb);

	//m_pSprite = new Sprite( 0 );

	m_pSObjectAnimated = NULL;
	m_pSObjectAnimated = new SceneObjectAnimated("15_Mesh.DAE","15_Anim.DAE","",D3DXVECTOR3(0.f,0.f,0.f), NULL);
	m_pSObjectAnimated->InitObjectAnimation();

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

	int offsetCursor;


	//Mouvement de la moulette :D
	//if( pInputManager->GetMouseWheelDelta() > 0 )
	//{
	//	pInputManager->InitMouseWheelDelta(); //raz

	//	Vector3f pos = m_pCamera->GetPosition();
	//	pos.z =  pos.z + sensibiliteRoulette;		

	//	float limZ = 60.f;
	//	float distance = m_pCamera->GetDistanceWithTarget();
	//	if( distance>limZ )
	//		m_pCamera->SetPosition( pos );
	//}

	//if( pInputManager->GetMouseWheelDelta() < 0  )
	//{
	//	pInputManager->InitMouseWheelDelta(); //raz

	//	Vector3f pos = m_pCamera->GetPosition();
	//	pos.z =  pos.z - sensibiliteRoulette;		

	//	float limZ = 300.f;
	//	float distance = m_pCamera->GetDistanceWithTarget();
	//	if( distance<limZ )
	//		m_pCamera->SetPosition( pos );
	//}

	//Init pos
	if( pInputManager->IsKeyPressed( 'A' ) )
	{
		m_pCamera->SetPosition(Vector3f(0.0f, 10.0f, -100.0f));
	}

	//Mouvement de l'objet
	if( pInputManager->IsKeyPressed( 'Z' ) )
	{
		D3DXMATRIX trans;
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep;

		xStep = -sin( angleY )*sensibiliteTranslation;
	 	zStep = cos( angleY )*sensibiliteTranslation;

		m_pSObjectAnimated->SetTranslation( xStep, 0.f, zStep );
	}
	if( pInputManager->IsKeyPressed( 'S' ) )
	{
		D3DXMATRIX trans;
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep;

		xStep = sin( angleY )*sensibiliteTranslation;
	 	zStep = -cos( angleY )*sensibiliteTranslation;

		m_pSObjectAnimated->SetTranslation( xStep, 0.f, zStep );

	}

	if( pInputManager->IsKeyPressed( 'Q' ) )
	{
		D3DXMATRIX trans;
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep;

		xStep = -cos( angleY )*sensibiliteTranslation;
	 	zStep = -sin( angleY )*sensibiliteTranslation;

		m_pSObjectAnimated->SetTranslation( xStep, 0.f, zStep );
	}
	if( pInputManager->IsKeyPressed( 'D' ) )
	{
		D3DXMATRIX trans;
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep;

		xStep = cos( angleY )*sensibiliteTranslation;
	 	zStep = sin( angleY )*sensibiliteTranslation;

		m_pSObjectAnimated->SetTranslation( xStep, 0.f, zStep );
	}

	//Mouvement de la souris -> mouvement camera
	if( point.x != 0 ) 
	{
		offsetCursor = (int)point.x%sensibiliteSouris; 
		m_pCamera->SetOrientationY( -offsetCursor );
		
		m_pSObjectAnimated->SetRotation( 0, offsetCursor, 0);
		
	}

	if( point.y != 0 ) 
	{
		offsetCursor = (int)point.y%sensibiliteSouris; 
		m_pCamera->SetOrientationX( offsetCursor );
	}

	m_pCamera->SetTarget( m_pSObjectAnimated );
	m_pCamera->UpdateMatrixView();


}

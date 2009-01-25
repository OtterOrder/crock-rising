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
	if( m_pSprite )
		delete m_pSprite;

	/*if ( m_pSObject)
		delete m_pSObject;*/
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelStart::Init( void )
{
	m_pCamera = new Camera( Vector3f(0.0f, 10.0f, -200.0f) );
	//m_pCamera->SetTarget( Vector3f(10.0f, 10.0f, 0.0f) );
	Renderer::GetInstance()->SetCamera( m_pCamera );

	m_pSprite = new Sprite( 0 );

	m_pSObjectAnimated = NULL;
	m_pSObjectAnimated = new SceneObjectAnimated("Skinned_Box.DAE","","",D3DXVECTOR3(0.f,0.f,0.f));
	m_pSObjectAnimated->InitObject();

	// C'est fait dans le constructeur !
	//Renderer::GetInstance()->m_ListObj.push_back(m_pSObjectAnimated);
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	
	//OffsetCurseur -> Camera
	InputManager* pInputManager = InputManager::GetInstance();
	Point2f point = pInputManager->GetMouseOffset();
	const int sensibilite = 15;
	int offsetCursor;

	//Mouvement de la moulette :D
	if( pInputManager->GetMouseWheelDelta() > 0 )
	{
		pInputManager->InitMouseWheelDelta(); //raz

		float sensibilite=25.0f;
		float limZ = 60.f;

		Vector3f pos = m_pCamera->GetPosition();
		pos.z =  pos.z + sensibilite;		

		float distance = m_pCamera->GetDistanceWithTarget();
		if( distance>limZ )
			m_pCamera->SetPosition( pos );
	}

	if( pInputManager->GetMouseWheelDelta() < 0  )
	{
		pInputManager->InitMouseWheelDelta(); //raz

		float sensibilite=25.0f;
		float limZ = 300.f;

		Vector3f pos = m_pCamera->GetPosition();
		pos.z =  pos.z - sensibilite;		

		float distance = m_pCamera->GetDistanceWithTarget();
		if( distance<limZ )
			m_pCamera->SetPosition( pos );
	}

	//Init pos
	if( pInputManager->IsKeyPressed( 'A' ) )
	{
		m_pCamera->SetPosition(Vector3f(0.0f, 10.0f, -100.0f));
	}

	//mouvement de l'objet
	if( pInputManager->IsKeyPressed( 'Z' ) )
	{
		D3DXMATRIX trans;
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep, sensibilite=0.1f ;

		xStep = -sin( angleY )*sensibilite;
	 	zStep = cos( angleY )*sensibilite;

		D3DXMatrixTranslation( &trans, xStep, 0.f, zStep );

		m_pSObjectAnimated->SetTransform( &trans );
	}
	if( pInputManager->IsKeyPressed( 'S' ) )
	{
		D3DXMATRIX trans;
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep, sensibilite=0.1f ;

		xStep = sin( angleY )*sensibilite;
	 	zStep = -cos( angleY )*sensibilite;

		D3DXMatrixTranslation( &trans, xStep, 0.f, zStep );

		m_pSObjectAnimated->SetTransform( &trans );

	}

	if( pInputManager->IsKeyPressed( 'Q' ) )
	{
		D3DXMATRIX trans;
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep, sensibilite=0.1f ;

		xStep = -cos( angleY )*sensibilite;
	 	zStep = -sin( angleY )*sensibilite;

		D3DXMatrixTranslation( &trans, xStep, 0.f, zStep );

		m_pSObjectAnimated->SetTransform( &trans );
	}
	if( pInputManager->IsKeyPressed( 'D' ) )
	{
		D3DXMATRIX trans;
		float angleY = m_pCamera->GetOrientationYRad();

		float xStep, zStep, sensibilite=0.1f ;

		xStep = cos( angleY )*sensibilite;
	 	zStep = sin( angleY )*sensibilite;

		D3DXMatrixTranslation( &trans, xStep, 0.f, zStep );

		m_pSObjectAnimated->SetTransform( &trans );
	}

	//mouvement de la souris -> mouvement camera
	if( point.x != 0 ) 
	{
		offsetCursor = (int)point.x%sensibilite; 
		m_pCamera->SetOrientationY( -offsetCursor );
	}
	if( point.y != 0 ) 
	{
		offsetCursor = (int)point.y%sensibilite; 
		m_pCamera->SetOrientationX( offsetCursor );
	}

	m_pCamera->SetTarget( m_pSObjectAnimated );
	m_pCamera->UpdateMatrixView();
}

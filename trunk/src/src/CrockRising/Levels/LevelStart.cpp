#include	"LevelStart.h"

#include	<Core/Inputs/InputManager.h>

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelStart::LevelStart( crc32 levelID )
: Level( levelID )
{
	//TODO
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelStart::~LevelStart( void )
{
	//TODO
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelStart::Init( void )
{
	m_Camera = new Camera( Vector3f(0.0f, 10.0f, -100.0f) );
	m_Camera->SetTarget( Vector3f(10.0f, 10.0f, 0.0f) );
	Renderer::GetInstance()->SetCamera( m_Camera );	
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	
	//OffsetCurseur -> Camera
	Point2f point = InputManager::GetInstance()->GetMouseOffset();
	const int sensibilite = 15;
	int offsetCursor;

	if( InputManager::GetInstance()->IsKeyPressed( 'Z' ) )
	{
		Vector3f pos = m_Camera->GetPosition();
		Vector3f target	 = m_Camera->GetTarget();
		Vector3f vectDir = target - pos;
		float distance = sqrt( vectDir.x*vectDir.x + vectDir.y*vectDir.y + vectDir.z*vectDir.z );
		float angleY = m_Camera->GetOrientationYRad();
		
		//float s = sin( angleY );
		float c = cos( angleY );

		//pos.x =  pos.x + s;
		pos.z =  pos.z + c;		

		if( distance>60.0f )
			m_Camera->SetPosition( pos );
	}

	if( InputManager::GetInstance()->IsKeyPressed( 'S' ) )
	{
		Vector3f pos = m_Camera->GetPosition();
		Vector3f target	 = m_Camera->GetTarget();
		Vector3f vectDir = target - pos;
		float distance = sqrt( vectDir.x*vectDir.x + vectDir.y*vectDir.y + vectDir.z*vectDir.z );
		float angleY = m_Camera->GetOrientationYRad();
		
		//float s = sin( angleY );
		float c = cos( angleY );

		//pos.x =  pos.x + s;
		pos.z =  pos.z - c;		

		if( distance<300.0f )
			m_Camera->SetPosition( pos );
	}

	if( InputManager::GetInstance()->IsKeyPressed( 'A' ) )
	{
		m_Camera->SetPosition(Vector3f(0.0f, 10.0f, -100.0f));
	}


	if( point.x != 0 ) 
	{
		offsetCursor = (int)point.x%sensibilite; 
		m_Camera->SetOrientationY( -offsetCursor );
	}
	if( point.y != 0 ) 
	{
		offsetCursor = (int)point.y%sensibilite; 
		m_Camera->SetOrientationX( offsetCursor );
	}

	m_Camera->UpdateMatrixView();
}
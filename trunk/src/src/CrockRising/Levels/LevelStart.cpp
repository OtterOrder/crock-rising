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
	m_Camera = new Camera();
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

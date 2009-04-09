#include	"LevelStart.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelStart::LevelStart( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelStart::~LevelStart( void )
{
	if( m_pCamera )
		delete m_pCamera;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelStart::Init( void )
{
	m_pCamera = new Camera( Vector3f( 0.0f, -10.0f, -30.0f ) );
	Renderer::GetInstance()->SetCamera( m_pCamera );
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelStart::Update( void )
{
	//TODO
}

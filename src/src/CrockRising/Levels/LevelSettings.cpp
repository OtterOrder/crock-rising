#include	"LevelSettings.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelSettings::LevelSettings( crc32 levelID )
: Level( levelID )
{
	m_Camera = NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelSettings::~LevelSettings( void )
{
	if( m_Camera )
		delete m_Camera;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelSettings::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();
	
	// Fond noir
	pRenderer->SetClearColor( Color4f( 0.f, 0.f, 0.f, 1.f ) );
	
	// Initialisation de la caméra
	m_Camera = new Camera( Vector3f( 0.0f, 10.0f, -100.0f ) );
	m_Camera->SetTarget( Vector3f( 10.0f, 10.0f, 0.0f ) );
	pRenderer->SetCamera( m_Camera );
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelSettings::Update( void )
{
	//TODO
}

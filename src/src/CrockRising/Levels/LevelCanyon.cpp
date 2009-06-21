#include	"LevelCanyon.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelCanyon::LevelCanyon( crc32 levelID )
: Level( levelID )
{
	m_Camera = NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelCanyon::~LevelCanyon( void )
{
	if( m_Camera )
		delete m_Camera;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelCanyon::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();
	
	// Fond noir
	pRenderer->SetClearColor( Color4f( 0.f, 0.f, 0.f, 1.f ) );
	
	// Initialisation de la caméra
	m_Camera = new Camera( Vector3f( 0.0f, 10.0f, -100.0f ) );
	m_Camera->SetTarget( Vector3f( 10.0f, 10.0f, 0.0f ) );
	pRenderer->SetCamera( m_Camera );

	//TODO
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelCanyon::Update( void )
{
	//TODO
}

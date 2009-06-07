#include	"LevelGameMenu.h"

#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelGameMenu::LevelGameMenu( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelGameMenu::~LevelGameMenu( void )
{
	if( m_pCamera )
		delete m_pCamera;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelGameMenu::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();
	
	// Fond noir
	pRenderer->SetClearColor( Color4f( 0.f, 0.f, 0.f, 1.f ) );
	
	// Initialisation de la cam�ra
	m_pCamera = new Camera( Vector3f( 0.0f, 10.0f, -100.0f ) );
	m_pCamera->SetTarget( Vector3f( 10.0f, 10.0f, 0.0f ) );
	pRenderer->SetCamera( m_pCamera );
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelGameMenu::Update( void )
{
	//TODO
}
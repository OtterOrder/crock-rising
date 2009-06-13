#include	"LevelAlafraise.h"

#include	<Core/System.h>
#include	<Core/Time.h>
#include	<Core/Inputs/InputManager.h>
#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>

//******************************************************************

#define		LEVEL_mainmenu			0x56b55f63

#define		LOGO_APPEARANCE_TIME	1.f		// Temps d'apparition du logo en sec

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelAlafraise::LevelAlafraise( crc32 levelID )
: Level( levelID )
{
	m_pCamera	= NULL;
	m_pLogo		= NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelAlafraise::~LevelAlafraise( void )
{
	if( m_pCamera )
		delete m_pCamera;
	
	if( m_pLogo )
		delete m_pLogo;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelAlafraise::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();
	
	// Fond blanc
	pRenderer->SetClearColor( Color4f( 1.f, 1.f, 1.f, 1.f ) );
	
	// Initialisation de la caméra
	m_pCamera = new Camera( Vector3f( 0.0f, 10.0f, -100.0f ) );
	m_pCamera->SetTarget( Vector3f( 10.0f, 10.0f, 0.0f ) );
	pRenderer->SetCamera( m_pCamera );

	// Logo à la fraise
	m_pLogo = new Sprite( "alafraiseprod.png" );
	m_pLogo->SetAlpha( 0.f );
	m_LogoTimer = 0.f;
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelAlafraise::Update( void )
{
	if( InputManager::GetInstance()->IsKeyReleased( VK_SPACE ) )
	{
		// On passe au level suivant dès qu'on appuie sur Espace..
		Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
		return;
	}

	if( m_LogoTimer < LOGO_APPEARANCE_TIME )
	{
		// Le logo apparait progressivement..
		m_LogoTimer += System::GetInstance()->GetTime()->GetDeltaTime();
		float alpha = m_LogoTimer / LOGO_APPEARANCE_TIME;
		m_pLogo->SetAlpha( MATH_Clamp( alpha, 0.f, 1.f ) );
	}
	
	int logoX, logoY;
	Renderer *pRenderer = Renderer::GetInstance();
	
	// Positionnement au centre (on le fait dans l'update
	// pour rester au centre même en cas de redimmentionnement)
	logoX = pRenderer->GetWindowWidth()/2 - m_pLogo->GetWidth()/2;
	logoY = pRenderer->GetWindowHeight()/2 - m_pLogo->GetHeight()/2;
	m_pLogo->SetPosition( logoX, logoY );
}

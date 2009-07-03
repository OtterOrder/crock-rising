#include	"LevelVictory.h"

#include	<Core/System.h>
#include	<Core/Time.h>
#include	<Core/Inputs/InputManager.h>
#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>

//******************************************************************


#define		LEVEL_mainmenu			0x56b55f63
#define		LOGO_APPEARANCE_TIME	2.f		// Temps d'apparition du logo en sec
#define		NEXT_LEVEL_TIME			4.f		// Temps avant de passer au level suivant

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelVictory::LevelVictory( crc32 levelID )
: Level( levelID )
{
	m_pCamera = NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelVictory::~LevelVictory( void )
{
	if( m_pCamera )
		delete m_pCamera;	
	
	if( m_pLogo )
		delete m_pLogo;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelVictory::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();
	
	// Fond noir
	pRenderer->SetClearColor( Color4f( 0.f, 0.f, 0.f, 1.f ) );
	
	// Initialisation de la caméra
	m_pCamera = new Camera( Vector3f( 0.0f, 10.0f, -100.0f ) );
	m_pCamera->SetTarget( Vector3f( 10.0f, 10.0f, 0.0f ) );
	pRenderer->SetCamera( m_pCamera );
	
	InputManager::GetInstance()->HoldMouseAtCenter( false );

	// Logo à la fraise
	m_pLogo = new Sprite( "alafraiseprod.png" );
	m_pLogo->Init();
	m_pLogo->SetAlpha( 1.f );
	m_pLogo->SetSize( Renderer::GetInstance()->GetWindowWidth(), Renderer::GetInstance()->GetWindowWidth());
	m_pLogo->SetHotPoint( m_pLogo->GetWidth()/2, m_pLogo->GetHeight()/2 ); // Point chaud au centre
	
	m_LogoTimer = 0.f;
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelVictory::Update( void )
{
	m_LogoTimer += System::GetInstance()->GetTime()->GetDeltaTime();

	if( InputManager::GetInstance()->IsKeyReleased( VK_RETURN )	||
		m_LogoTimer >= NEXT_LEVEL_TIME							)
	{
		// Si on a appuyé sur Entrée ou si le temps est
		// écoulé, on passe au level suivant..
		Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
		return;
	}

	if( m_LogoTimer < LOGO_APPEARANCE_TIME )
	{
		// Le logo apparait progressivement..
		float alpha = m_LogoTimer / LOGO_APPEARANCE_TIME;
		m_pLogo->SetAlpha( MATH_Clamp( alpha, 0.f, 1.f ) );
	}
	
	// Positionnement au centre (on le fait dans l'update
	// pour rester au centre même en cas de redimmentionnement)
	m_pLogo->SetPosition(
		Renderer::GetInstance()->GetWindowWidth()/2,
		Renderer::GetInstance()->GetWindowHeight()/2
	);
}

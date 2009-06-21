#include	"LevelMainMenu.h"

#include	<Core/Inputs/InputManager.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>
#include	<Objects/2D/Text.h>

//******************************************************************

#define		MAINMENU_X				64
#define		MAINMENU_Y				48
#define		MAINMENU_SIZE			17
#define		MAINMENU_MARGIN			7
#define		MAINMENU_COLOR			(Color4f( 0.f, 0.f, 0.0f, 0.5f ))
#define		MAINMENU_COLOR_HOVER	(Color4f( 0.f, 0.f, 0.f, 1.f ))

#define		MAINMENU_PLAY			"JOUER"
#define		MAINMENU_HIGHSCORES		"MEILLEURS SCORES"
#define		MAINMENU_SETTINGS		"OPTIONS"
#define		MAINMENU_QUIT			"QUITTER"

//******************************************************************

static char *g_MenuTexts[LevelMainMenu::NB_LINKS] =
{
	MAINMENU_PLAY,
	MAINMENU_HIGHSCORES,
	MAINMENU_SETTINGS,
	MAINMENU_QUIT
};

static u32 g_BubblesWidth[LevelMainMenu::NB_LINKS] =
{
	50, 145, 65, 65
};

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelMainMenu::LevelMainMenu( crc32 levelID )
: Level( levelID )
{
	m_Camera = NULL;
	m_Background = NULL;
	memset( m_Menu, NULL, NB_LINKS*sizeof(Text*) );
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelMainMenu::~LevelMainMenu( void )
{
	if( m_Camera )
		delete m_Camera;

	if( m_Background )
		delete m_Background;
	
	for( int link = 0; link < NB_LINKS; link++ )
		if( m_Menu[link] )
			delete m_Menu[link];
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelMainMenu::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();
	
	// Fond noir
	pRenderer->SetClearColor( Color4f( 0.f, 0.f, 0.f, 1.f ) );
	
	// Initialisation de la caméra
	m_Camera = new Camera( Vector3f( 0.0f, 10.0f, -100.0f ) );
	m_Camera->SetTarget( Vector3f( 10.0f, 10.0f, 0.0f ) );
	pRenderer->SetCamera( m_Camera );

	// Background
	m_Background = new Sprite( "background00_4_3.jpg" );
	m_Background->Init();
	m_Background->SetAsBackground( true );

	// Menu principal
	for( int link = 0; link < NB_LINKS; link++ )
	{
		m_Menu[link] = new Text( g_MenuTexts[link], "Arial", MAINMENU_SIZE, true, false );
		m_Menu[link]->Init();
		m_Menu[link]->SetColor( MAINMENU_COLOR );
		m_Menu[link]->SetPosition( MAINMENU_X, MAINMENU_Y + link*( MAINMENU_SIZE + MAINMENU_MARGIN ) );
		m_Menu[link]->SetBubbleWidth( g_BubblesWidth[link] );
	}
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelMainMenu::Update( void )
{
	InputManager *inputMgr = InputManager::GetInstance();
	Vector2f mousePos = inputMgr->GetMousePosition();
	
	for( int link = 0; link < NB_LINKS; link++ )
	{
		m_Menu[link]->SetColor( MAINMENU_COLOR );
		if( m_Menu[link]->IsCollide( (s32)mousePos.x, (s32)mousePos.y ) )
		{
			m_Menu[link]->SetColor( MAINMENU_COLOR_HOVER );

			if( inputMgr->IsMouseReleased( MOUSE_LEFT ) )
			{
				switch( link )
				{
					case PLAY:
						//TODO
						break;

					case HIGHSCORES:
						//TODO
						break;

					case SETTINGS:
						//TODO
						break;

					case QUIT:
						// Fermeture de l'appli..
						Renderer::GetInstance()->Close();
						break;
				}
			}
		}
	}
}

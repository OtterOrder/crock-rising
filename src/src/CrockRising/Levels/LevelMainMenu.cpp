#include	"LevelMainMenu.h"

#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>
#include	<Sound/SoundObject.h>

//******************************************************************

#define		LEVEL_gamemenu			0x2b3430ac
#define		LEVEL_highscores		0x59f26d0
#define		LEVEL_settings			0xe545a0c5

#define		MAINMENU_X				64
#define		MAINMENU_Y				48
#define		MAINMENU_MARGIN			24

//******************************************************************

static char *g_ButtonsText[LevelMainMenu::NB_LINKS] =
{
	"JOUER",
	"MEILLEURS SCORES",
	"OPTIONS",
	"QUITTER"
};

static u32 g_ButtonsWidth[LevelMainMenu::NB_LINKS] =
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
	m_Camera		= NULL;
	m_Background	= NULL;
	m_Sound			= NULL;

	memset( m_Menu, NULL, NB_LINKS*sizeof(MMButton*) );
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

	if( m_Sound )
		delete m_Sound;
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

	// Son
	m_Sound = new SoundObject( "twt.ogg" );

	// Menu principal
	for( int link = 0; link < NB_LINKS; link++ )
	{
		m_Menu[link] = new MMButton( (Link)link );
		m_Menu[link]->Init();
		m_Menu[link]->SetPosition( MAINMENU_X, MAINMENU_Y + link*MAINMENU_MARGIN );
		m_Menu[link]->SetBubbleWidth( g_ButtonsWidth[link] );
		m_Menu[link]->SetSound( m_Sound );
	}
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelMainMenu::Update( void )
{
	//EMPTY
}

/***********************************************************
 * Constructeur de MMButton.
 **********************************************************/
LevelMainMenu::MMButton::MMButton( Link _linkID )
: ButtonText( g_ButtonsText[_linkID] )
{
	linkID = _linkID;
}

/***********************************************************
 * Action au clic sur un bouton.
 **********************************************************/
void LevelMainMenu::MMButton::OnClic()
{
	switch( linkID )
	{
		case PLAY:
			// Menu de jeu..
			Game::GetInstance()->ChangeLevel( LEVEL_gamemenu );
			return;
		
		case HIGHSCORES:
			// Highscores..
			Game::GetInstance()->ChangeLevel( LEVEL_highscores );
			return;
		
		case SETTINGS:
			// Menu d'options..
			Game::GetInstance()->ChangeLevel( LEVEL_settings );
			return;
		
		case QUIT:
			// Fermeture de l'appli..
			Renderer::GetInstance()->Close();
			return;

		//default:
			// Rien..
	}
}

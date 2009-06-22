#include	"LevelGameMenu.h"

#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>
#include	<Sound/SoundObject.h>

//******************************************************************

#define		LEVEL_mainmenu			0x56b55f63

#define		GAMEMENU_X				64
#define		GAMEMENU_Y				48
#define		GAMEMENU_MARGIN			24

//******************************************************************

static char *g_ButtonsText[LevelGameMenu::NB_LINKS] =
{
	"RETOUR"
};

static u32 g_ButtonsWidth[LevelGameMenu::NB_LINKS] =
{
	60
};

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelGameMenu::LevelGameMenu( crc32 levelID )
: Level( levelID )
{
	m_Camera		= NULL;
	m_Background	= NULL;
	m_Sound			= NULL;

	memset( m_Menu, NULL, NB_LINKS*sizeof(GMButton*) );
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelGameMenu::~LevelGameMenu( void )
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
void LevelGameMenu::Init( void )
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

	// Menu de jeu
	for( int link = 0; link < NB_LINKS; link++ )
	{
		m_Menu[link] = new GMButton( (Link)link );
		m_Menu[link]->Init();
		m_Menu[link]->SetPosition( GAMEMENU_X, GAMEMENU_Y + link*GAMEMENU_MARGIN );
		m_Menu[link]->SetBubbleWidth( g_ButtonsWidth[link] );
		m_Menu[link]->SetSound( m_Sound );
	}
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelGameMenu::Update( void )
{
	//TODO
}

/***********************************************************
 * Constructeur de GMButton.
 **********************************************************/
LevelGameMenu::GMButton::GMButton( Link _linkID )
: ButtonText( g_ButtonsText[_linkID] )
{
	linkID = _linkID;
}

/***********************************************************
 * Action au clic sur un bouton.
 **********************************************************/
void LevelGameMenu::GMButton::OnClic()
{
	switch( linkID )
	{
		case BACK:
			// Retour au menu principal..
			Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
			return;

		//default:
			// Rien..
	}
}

#include	"LevelGameMenu.h"

#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>
#include	<Sound/SoundObject.h>
#include	"../2D/ButtonText.h"

//******************************************************************

#define		LEVEL_sphinx			0xd992ae67
#define		LEVEL_canyon			0x53466a72

#define		BACKBUTTON_X			64
#define		BACKBUTTON_Y			48

#define		GAMEMENU_X				64
#define		GAMEMENU_Y				(BACKBUTTON_Y+24)
#define		GAMEMENU_MARGIN			310

//******************************************************************

static char *g_ButtonsSprite[LevelGameMenu::NB_LINKS] =
{
	"sphinx_preview.png",
	"canyon_preview.png"
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
	m_BackButton	= NULL;
	m_Sound			= NULL;

	memset( m_Menu, NULL, NB_LINKS*sizeof(GMButton*) );
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelGameMenu::~LevelGameMenu( void )
{
	if( m_Camera )		delete m_Camera;
	if( m_Background )	delete m_Background;
	if( m_BackButton )	delete m_BackButton;

	for( int link = 0; link < NB_LINKS; link++ )
		if( m_Menu[link] )
			delete m_Menu[link];

	if( m_Sound )		delete m_Sound;
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

	// Bouton retour
	m_BackButton = new BackButton;
	m_BackButton->Init();
	m_BackButton->SetPosition( BACKBUTTON_X, BACKBUTTON_Y );
	m_BackButton->SetSound( m_Sound );

	// Menu de jeu
	for( int link = 0; link < NB_LINKS; link++ )
	{
		m_Menu[link] = new GMButton( (Link)link );
		m_Menu[link]->Init();
		m_Menu[link]->SetPosition( GAMEMENU_X + link*GAMEMENU_MARGIN, GAMEMENU_Y );
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
: ButtonSprite( g_ButtonsSprite[_linkID] )
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
		case SPHINX:
			// Level Sphinx..
			Game::GetInstance()->ChangeLevel( LEVEL_sphinx );
			return;

		case CANYON:
			// Level Canyon..
			Game::GetInstance()->ChangeLevel( LEVEL_canyon );
			return;
	}
}

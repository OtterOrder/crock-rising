#include	"LevelHighScores.h"

#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>
#include	<Sound/SoundObject.h>

//******************************************************************

#define		LEVEL_mainmenu			0x56b55f63

#define		BACKBUTTON_X			64
#define		BACKBUTTON_Y			48

#define		SCORESBG_X				64
#define		SCORESBG_Y				(BACKBUTTON_Y+24)

#define		SCORE_X					10	// Relatif à SCORESBG_X
#define		SCORE_Y					10	// Relatif à SCORESBG_Y
#define		SCORE_MARGIN			24

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelHighScores::LevelHighScores( crc32 levelID )
: Level( levelID )
{
	m_Camera		= NULL;
	m_Background	= NULL;
	m_BackButton	= NULL;
	m_Sound			= NULL;
	m_ScoresBG		= NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelHighScores::~LevelHighScores( void )
{
	if( m_Camera )		delete m_Camera;
	if( m_Background )	delete m_Background;
	if( m_BackButton )	delete m_BackButton;
	if( m_Sound )		delete m_Sound;
	if( m_ScoresBG )	delete m_ScoresBG;

	//delete m_Scores;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelHighScores::Init( void )
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

	// Fond des scores
	u32 bgWidth = Renderer::GetInstance()->GetWindowWidth() - SCORESBG_X*2;
	u32 bgHeight = Renderer::GetInstance()->GetWindowHeight() - SCORESBG_Y*2;

	m_ScoresBG = new Quad( bgWidth, bgHeight, Color4f( 1.f, 1.f, 1.f, 0.75f ) );
	m_ScoresBG->Init();
	m_ScoresBG->SetPosition( SCORESBG_X, SCORESBG_Y );

	//TODO: récupérer les highscores dans la backup ?

	//m_Scores = new Text( "", "Arial", 17, true, false );
	//m_Scores->Init();
	/*m_Scores->SetBubbleSize( 300, 300 );
	m_Scores->SetText( "OLOL, pouf tagada pouet lol.\nBonjour ça fart, yoho cool\npouf, mohahaha.." );
	m_Scores->SetFormat( DT_TOP | DT_CENTER );*/
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelHighScores::Update( void )
{
	//TODO
}

/***********************************************************
 * Constructeur de BackButton.
 **********************************************************/
LevelHighScores::BackButton::BackButton()
: ButtonText( "RETOUR" )
{
	SetBubbleWidth( 60 );
}

/***********************************************************
 * Action au clic sur un bouton.
 **********************************************************/
void LevelHighScores::BackButton::OnClic()
{
	// Retour au menu principal..
	Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
}


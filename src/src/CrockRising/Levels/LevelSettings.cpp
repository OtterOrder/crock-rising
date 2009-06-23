#include	"LevelSettings.h"

#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>
#include	<Sound/SoundObject.h>

//******************************************************************

#define		LEVEL_mainmenu			0x56b55f63

#define		MENU_X					64
#define		MENU_Y					48
#define		MENU_MARGIN				24

#define		VALUES_X				400
#define		VALUES_FONT				"Arial"
#define		VALUES_SIZE				17
#define		VALUES_COLOR			(Color4f( 0.f, 0.f, 0.0f, 0.5f ))
#define		VALUES_WIDTH			150

//******************************************************************

static char *g_ButtonsText[LevelSettings::NB_LINKS] =
{
	"RATIO",
	"RESOLUTION",
	"APPLIQUER",
	"RETOUR"
};

static u32 g_ButtonsWidth[LevelSettings::NB_LINKS] =
{
	45, 95, 80, 60
};

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelSettings::LevelSettings( crc32 levelID )
: Level( levelID )
{
	m_Camera		= NULL;
	m_Background	= NULL;
	m_Sound			= NULL;

	memset( m_Menu, NULL, NB_LINKS*sizeof(SettingButton*) );
	memset( m_Values, NULL, NB_SETTINGS*sizeof(Text*) );
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelSettings::~LevelSettings( void )
{
	if( m_Camera )		delete m_Camera;
	if( m_Background )	delete m_Background;

	for( int link = 0; link < NB_LINKS; link++ )
		if( m_Menu[link] )
			delete m_Menu[link];

	for( int value = 0; value < NB_SETTINGS; value++ )
		if( m_Values[value] )
			delete m_Values[value];

	if( m_Sound )		delete m_Sound;
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

	// Background
	m_Background = new Sprite( "background00_4_3.jpg" );
	m_Background->Init();
	m_Background->SetAsBackground( true );

	// Son
	m_Sound = new SoundObject( "twt.ogg" );

	// Menu
	for( int link = 0; link < NB_LINKS; link++ )
	{
		m_Menu[link] = new SettingButton( (Link)link );
		m_Menu[link]->Init();
		m_Menu[link]->SetPosition( MENU_X, MENU_Y + link*MENU_MARGIN );
		m_Menu[link]->SetBubbleWidth( g_ButtonsWidth[link] );
		m_Menu[link]->SetSound( m_Sound );
	}

	// Options
	//TODO: lire les infos dans le Renderer
	m_Settings[RATIO]		= "4:3";
	m_Settings[RESOLUTION]	= "1280x1024";

	// Valeurs
	for( int setting = 0; setting < NB_SETTINGS; setting++ )
	{
		m_Values[setting] = new Text( m_Settings[setting], VALUES_FONT, VALUES_SIZE, true, false );
		m_Values[setting]->Init();
		m_Values[setting]->SetColor( VALUES_COLOR );
		m_Values[setting]->SetFormat( DT_TOP | DT_RIGHT );
		m_Values[setting]->SetPosition( VALUES_X, MENU_Y + setting*MENU_MARGIN );
		m_Values[setting]->SetHotPoint( VALUES_WIDTH, 0 );
		m_Values[setting]->SetBubbleWidth( VALUES_WIDTH );
	}
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelSettings::Update( void )
{
	//TODO
}

/***********************************************************
 * Constructeur de SettingButton.
 **********************************************************/
LevelSettings::SettingButton::SettingButton( Link _linkID )
: ButtonText( g_ButtonsText[_linkID] )
{
	linkID = _linkID;
}

/***********************************************************
 * Action au clic sur un bouton.
 **********************************************************/
void LevelSettings::SettingButton::OnClic()
{
	switch( linkID )
	{
		case RATIO:
			//TODO
			return;
		
		case RESOLUTION:
			//TODO
			return;
		
		case APPLY:
			//TODO
			return;
		
		case BACK:
			// Menu principal..
			Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
			return;

		//default:
			// Rien..
	}
}

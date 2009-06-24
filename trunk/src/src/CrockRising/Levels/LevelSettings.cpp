#include	"LevelSettings.h"

#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Sprite.h>
#include	<Sound/SoundObject.h>
#include	"../Backup.h"

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

#define		NB_RESOLUTIONS			3

//******************************************************************

static const char *g_ButtonsText[LevelSettings::NB_LINKS] =
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

static const char *g_Ratios[LevelSettings::NB_RATIOS] =
{
	"4:3",
	"16:10"
};

static int g_Resolutions[LevelSettings::NB_RATIOS][NB_RESOLUTIONS][2] =
{
	{ { 800, 600 }, { 1024, 768 }, { 1280, 1024 } },
	{ { 960, 600 }, { 1280, 800 }, { 1440, 900 } }
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

	m_CurrentRatio		= 0;
	m_CurrentResolution	= 0;
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

	// Valeurs
	for( int setting = 0; setting < NB_SETTINGS; setting++ )
	{
		m_Values[setting] = new Text( "", VALUES_FONT, VALUES_SIZE, true, false );
		m_Values[setting]->Init();
		m_Values[setting]->SetColor( VALUES_COLOR );
		m_Values[setting]->SetFormat( DT_TOP | DT_RIGHT );
		m_Values[setting]->SetPosition( VALUES_X, MENU_Y + setting*MENU_MARGIN );
		m_Values[setting]->SetHotPoint( VALUES_WIDTH, 0 );
		m_Values[setting]->SetBubbleSize( VALUES_WIDTH, VALUES_SIZE );
	}

	// Initialisation des options..
	ReadFromBackup();
	RefreshValues();
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelSettings::Update( void )
{
	//TODO
}

/***********************************************************
 * Avance d'un ratio..
 **********************************************************/
void LevelSettings::IncRatio()
{
	if( ++m_CurrentRatio == NB_RATIOS )
		m_CurrentRatio = 0;

	RefreshValues();
}

/***********************************************************
 * Avance d'une résolution..
 **********************************************************/
void LevelSettings::IncResolution()
{
	if( ++m_CurrentResolution == NB_RESOLUTIONS )
		m_CurrentResolution = 0;

	RefreshValues();
}

/***********************************************************
 * Initialise les données membres d'après la backup.
 **********************************************************/
void LevelSettings::ReadFromBackup()
{
	CRBackup *pBackup = static_cast<CRBackup*>(Game::GetInstance()->GetBackup());

	// On cherche la résolution et le ratio correspondant
	for( int ratio = 0; ratio < NB_RATIOS; ratio++ ){
		for( int res = 0; res < NB_RESOLUTIONS; res++ )
		{
			if( g_Resolutions[ratio][res][0] == pBackup->GetResolution().x	&&
				g_Resolutions[ratio][res][1] == pBackup->GetResolution().y	)
			{
				m_CurrentRatio		= ratio;
				m_CurrentResolution	= res;
				break;
			}
		}
	}
}

/***********************************************************
 * Sauve les options dans la backup.
 **********************************************************/
void LevelSettings::SaveInBackup() const
{
	CRBackup *pBackup = static_cast<CRBackup*>(Game::GetInstance()->GetBackup());

	// Sauve la résolution
	pBackup->SetResolution( Vector2i(
		g_Resolutions[m_CurrentRatio][m_CurrentResolution][0],
		g_Resolutions[m_CurrentRatio][m_CurrentResolution][1] )
	);
}

/***********************************************************
 * Rafraichi les valeurs affichées des options.
 **********************************************************/
void LevelSettings::RefreshValues()
{
	// Ratio
	m_Values[RATIO]->SetText( g_Ratios[m_CurrentRatio] );
	
	// Resolution
	char buffer[5];
	m_Values[RESOLUTION]->SetText( _itoa( g_Resolutions[m_CurrentRatio][m_CurrentResolution][0], buffer, 10 ) );
	m_Values[RESOLUTION]->AddText( "x" );
	m_Values[RESOLUTION]->AddText( _itoa( g_Resolutions[m_CurrentRatio][m_CurrentResolution][1], buffer, 10 ) );
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
	Game *pGame = Game::GetInstance();
	LevelSettings *pLevel = static_cast<LevelSettings*>(pGame->GetLevel());
	
	switch( linkID )
	{
		case RATIO:
			pLevel->IncRatio();
			return;
		
		case RESOLUTION:
			pLevel->IncResolution();
			return;
		
		case APPLY:
			// On sauve et on recharge le level..
			pLevel->SaveInBackup();
			pGame->ChangeLevel( LEVEL_settings );
			return;
		
		case BACK:
			// Menu principal..
			pGame->ChangeLevel( LEVEL_mainmenu );
			return;

		//default:
			// Rien..
	}
}

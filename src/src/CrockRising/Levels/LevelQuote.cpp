#include	"LevelQuote.h"

#include	<Core/System.h>
#include	<Core/Time.h>
#include	<Core/Inputs/InputManager.h>
#include	<Game/Game.h>
#include	<Renderer/Renderer.h>
#include	<Objects/Camera.h>
#include	<Objects/2D/Text.h>

//******************************************************************

#define		LEVEL_mainmenu			0x56b55f63

#define		QUOTE_TEXT_FONT			"Times New Roman"
#define		QUOTE_TEXT_SIZE			18
#define		QUOTE_TEXT_COLOR		(Color4f( 0.f, 0.f, 0.f, 0.8f ))

#define		QUOTE_WIDTH				340
#define		QUOTE_HEIGHT			80
#define		SIGNATURE_WIDTH			90

#define		CHAR_DELAY				0.04f	// Délai entre l'apparition des caractères
#define		TEXT_DELAY				1.f		// Délai entre l'apparition des textes
#define		NEXT_LEVEL_TIME			2.5f	// Délai avant le changement de level

//******************************************************************

/***********************************************************
 * Constructeur.
 * @param[in]	levelID	: ID du niveau
 **********************************************************/
LevelQuote::LevelQuote( crc32 levelID )
: Level( levelID )
{
	m_Camera	= NULL;
	m_Quote		= NULL;
	m_Signature	= NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
LevelQuote::~LevelQuote( void )
{
	if( m_Camera )		delete m_Camera;
	if( m_Quote )		delete m_Quote;
	if( m_Signature )	delete m_Signature;
}

/***********************************************************
* Initialisation du niveau.
**********************************************************/
void LevelQuote::Init( void )
{
	Renderer *pRenderer = Renderer::GetInstance();
	
	// Fond blanc
	pRenderer->SetClearColor( Color4f( 1.f, 1.f, 1.f, 1.f ) );
	
	// Initialisation de la caméra
	m_Camera = new Camera( Vector3f( 0.0f, 10.0f, -100.0f ) );
	m_Camera->SetTarget( Vector3f( 10.0f, 10.0f, 0.0f ) );
	pRenderer->SetCamera( m_Camera );

	// Quote
	m_Quote = new Text( "", QUOTE_TEXT_FONT, QUOTE_TEXT_SIZE, true, false );
	m_Quote->Init();
	m_Quote->SetColor( QUOTE_TEXT_COLOR );
	m_Quote->SetBubbleSize( QUOTE_WIDTH, QUOTE_HEIGHT );
	m_Quote->SetHotPoint( QUOTE_WIDTH/2, QUOTE_HEIGHT/2 );

	// Signature
	m_Signature = new Text( "", QUOTE_TEXT_FONT, QUOTE_TEXT_SIZE, true, false );
	m_Signature->Init();
	m_Signature->SetColor( QUOTE_TEXT_COLOR );
	m_Signature->SetBubbleSize( SIGNATURE_WIDTH, QUOTE_TEXT_SIZE );
	m_Signature->SetHotPoint( SIGNATURE_WIDTH, 0 );

	// Textes
	m_Texts[0] = "\"L'espace.\n";
	m_Texts[1] = "On pourrait croire qu'il est infini...\n";
	m_Texts[2] = "Mais quand on arrive au bout un gorille commence\nà nous balancer des tonneaux.\"";
	m_Texts[3] = "Philip J. Fry";

	m_CurrentText	= QUOTE_LINE1;
	m_CurrentChar	= 0;
	m_Timer			= CHAR_DELAY;
}

/***********************************************************
 * Update du niveau.
 **********************************************************/
void LevelQuote::Update( void )
{
	m_Timer -= System::GetInstance()->GetTime()->GetDeltaTime();
	
	if( InputManager::GetInstance()->IsKeyReleased( VK_RETURN )	||
		( m_CurrentText == NB_TEXTS && m_Timer <= 0 )			)
	{
		// Si on a appuyé sur Entrée ou si le temps est
		// écoulé, on passe au menu principal..
		Game::GetInstance()->ChangeLevel( LEVEL_mainmenu );
		return;
	}

	// Positionnement au centre
	m_Quote->SetPosition(
		Renderer::GetInstance()->GetWindowWidth()/2,
		Renderer::GetInstance()->GetWindowHeight()/2
	);
	m_Signature->SetPosition(
		m_Quote->GetPosition().x + QUOTE_WIDTH/2,
		m_Quote->GetPosition().y + QUOTE_HEIGHT/2
	);

	// Gestion de l'apparition du texte
	if( m_CurrentText < NB_TEXTS && m_Timer <= 0 )
	{
		char addedChar[2] = { ' ', '\0' };
		Text *pText = ( m_CurrentText == SIGNATURE ) ? m_Signature : m_Quote;
		
		// On ajoute un caractère
		addedChar[0] = m_Texts[m_CurrentText][m_CurrentChar++];
		pText->AddText( addedChar );

		// Si on est à la fin du texte courant..
		if( m_CurrentChar >= (s32)m_Texts[m_CurrentText].size() )
		{
			if( ++m_CurrentText == NB_TEXTS )
			{
				m_Timer = NEXT_LEVEL_TIME;
				return;
			}
			m_CurrentChar = 0;
			m_Timer = TEXT_DELAY;
			return;
		}
		m_Timer = CHAR_DELAY;
	}
}